#include "webrtcstreamer.h"

webrtcstreamer::webrtcstreamer(QObject *parent) :QObject(parent)
{

}

webrtcstreamer::~webrtcstreamer()
{
    stop();
}

void webrtcstreamer::start(const std::string &signnalUrl)
{
    if(!initEncoder()) {emit logMessage("H264 编码器初始化失败"); return;}

    ws=std::make_shared<rtc::WebSocket>();
    ws->onOpen([this](){
        emit logMessage("[WebRTC] 信令服务器连接成功");
        
        rtc::Configuration config;
        pc=std::make_shared<rtc::PeerConnection>(config);

        auto video = rtc::Description::Video("video", rtc::Description::Direction::SendOnly);
        video.addH264Codec(96);
        videoTrack = pc->addTrack(video);

        pc->onLocalCandidate([this](const rtc::Candidate c){
        json msg ={
            {"type", "candidate"},
            {"candidate", std::string(c)},
            {"sdpMid", c.mid()}
        };
        ws->send(msg.dump());
        pcReady = true;
        
        });

        pc->onLocalDescription([this](const rtc::Description d){
            json msg={
                {"type", std::string(d.typeString())},
                {"sdp", std::string(d)}
            };
            ws->send(msg.dump());
            emit logMessage("[WebRTC] 已发送 SDP offer");
        });


        pc->onStateChange([this](rtc::PeerConnection::State s){
            if(s==rtc::PeerConnection::State::Connected){
                emit logMessage("[WebRTC] 已连接到对端");
                running=true;
            }
            else if (s == rtc::PeerConnection::State::Disconnected) {
                emit logMessage("[WebRTC] 对端已断开");
                 running = false;
                }
        });

        pc->setLocalDescription();
    });

    ws->onMessage([this](std::variant<rtc::binary, rtc::string> msg) {
    if (!pcReady) return;  // ← onOpen 还没完成，不处理消息
    if (std::holds_alternative<rtc::string>(msg)) {
        handleSignalingMessage(std::get<rtc::string>(msg));
    }
    });

    ws->onError([this](const std::string err){
        emit logMessage("[WebRTC] 错误: "+QString::fromStdString(err));
    });
    ws->open(signnalUrl);
}

void webrtcstreamer::stop()
{
    running=false;
    if(pc) pc->close();
    if(ws) ws->close();
    releaseEncoder();

}

void webrtcstreamer::handleSignalingMessage(const std::string &rawMsg)
{
    try{
        json msg=json::parse(rawMsg);
    std::string type=msg["type"]; 

    if(type=="answer"){
         // 浏览器回复了 SDP answer
        rtc::Description answer(msg["sdp"].get<std::string>(),rtc::Description::Type::Answer);
        pc->setRemoteDescription(answer);
        emit logMessage("[WebRTC] 收到 SDP answer");
    } else if(type=="candidate"){
         // 收到对端的 ICE candidate

         rtc::Candidate candidate(msg["candidate"].get<std::string>(),msg["sdpMid"].get<std::string>());
        pc->addRemoteCandidate(candidate);

        }
    } catch (const std::exception &e) {
        emit logMessage("[WebRTC] 解析 JSON 失败: "+QString::fromStdString(e.what()));
    }
}

// ============ 推帧 ============
void webrtcstreamer::pushFrame(const cv::Mat &frame)
{
 if (!running || !videoTrack || !videoTrack->isOpen()) return;


auto encoded = encodeFrame(frame);
 // 调试
    static int cnt = 0;
    if (++cnt % 30 == 0) {
        emit logMessage(QString("[WebRTC] 第%1帧, 编码大小=%2 bytes, running=%3")
            .arg(cnt).arg(encoded.size()).arg(running));
    }

 
 if (encoded.empty()) return;

 timestamp += 90000 / fps;

 try {
 videoTrack->send(reinterpret_cast<const std::byte*>(encoded.data()),
 encoded.size());
 } catch (...) {}
}


bool webrtcstreamer::initEncoder()
{
    releaseEncoder();

    const AVCodec *codec=avcodec_find_encoder(AV_CODEC_ID_H264);
    if(!codec) {emit logMessage("H264 编码器未找到"); return false;}

    codecCtx=avcodec_alloc_context3(codec);
    if(!codecCtx) {emit logMessage("H264 编码器上下文分配失败"); return false;}
    codecCtx->width=frameWidth;
    codecCtx->height=frameHeight;
    codecCtx->time_base={1, fps};
    codecCtx->framerate={fps, 1};
    codecCtx->pix_fmt=AV_PIX_FMT_YUV420P;
    codecCtx->bit_rate=2000000;
    codecCtx->gop_size=30;
    codecCtx->max_b_frames=0;

    av_opt_set(codecCtx->priv_data,"preset","ultrafast",0);
    av_opt_set(codecCtx->priv_data,"tune","zerolatency",0);

    if(avcodec_open2(codecCtx, codec, nullptr)<0) {emit logMessage("H264 编码器打开失败"); return false;}

    swsCtx=sws_getContext(frameWidth, frameHeight, AV_PIX_FMT_BGR24, frameWidth, frameHeight, AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr,nullptr);

    avFrame=av_frame_alloc();
    avFrame->format=AV_PIX_FMT_YUV420P;
    avFrame->width=frameWidth;
    avFrame->height=frameHeight;
    av_image_alloc(avFrame->data, avFrame->linesize, frameWidth, frameHeight, AV_PIX_FMT_YUV420P, 32);
    if(!avFrame->data[0]) {emit logMessage("H264 编码器分配失败"); return false;}

    avPacket=av_packet_alloc();
    return true;
}

// ============ 编码一帧 ============
void webrtcstreamer::releaseEncoder()
{
    if(codecCtx) {avcodec_free_context(&codecCtx); codecCtx=nullptr;}
    if(swsCtx) {sws_freeContext(swsCtx); swsCtx=nullptr;}
    if(avFrame) {av_frame_free(&avFrame); avFrame=nullptr;}
    if(avPacket) {av_packet_free(&avPacket); avPacket=nullptr;}
}
std::vector<std::byte> webrtcstreamer::encodeFrame(const cv::Mat &frame)
{
 if (frame.empty() || !codecCtx) return {};

 const uint8_t *srcData[1] = {frame.data};
 int srcLinesize[1] = {(int)frame.step};
 sws_scale(swsCtx, srcData, srcLinesize, 0, frameHeight,
 avFrame->data, avFrame->linesize);

 avFrame->pts = timestamp;
 timestamp += 90000 / fps;

 std::vector<std::byte> result;

 int ret = avcodec_send_frame(codecCtx, avFrame);
 if (ret < 0) return {};

 // 正常编码循环：取走所有可用的 packet
 while (true) {
 int ret2 = avcodec_receive_packet(codecCtx, avPacket);
 if (ret2 < 0) break; // EAGAIN = 编码器还在缓冲，正常
 auto *data = reinterpret_cast<std::byte*>(avPacket->data);
 result.insert(result.end(), data, data + avPacket->size);
 av_packet_unref(avPacket);
 }

 return result;
}
