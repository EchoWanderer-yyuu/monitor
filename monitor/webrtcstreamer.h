#ifndef WEBRTCSTREAMER_H
#define WEBRTCSTREAMER_H

#include <rtc/rtc.hpp>
#include <opencv2/opencv.hpp>
#include <QObject>
#include <QString>
#include <memory>
#include <thread>
#include <mutex>

// FFmpeg
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include <nlohmann/json.hpp>
using json = nlohmann::json;


class webrtcstreamer
{
    Q_OBJECT
public:
    explicit webrtcstreamer(QObject *parent = nullptr);
    ~webrtcstreamer();

    void start(const std::string &signnalUrl); // 连接信令服务器
    void stop(); // 断开连接
    void pushFrame(const cv::Mat &frame);

signals:
    void logMessage(const QString &message);

private:
    //WebRTC
    std::shared_ptr<rtc::WebSocket> ws; // 信令 WebSocket
    std::shared_ptr<rtc::PeerConnection> pc; // WebRTC 连接
    std::shared_ptr<rtc::Track> videoTrack; // 视频轨道
    rtc::SSRC ssrc=42;
    uint32_t timestamp=0;

    // FFmpeg 编译器
    AVCodecContext* codecCtx = nullptr;
    SwsContext* swsCtx = nullptr;
    AVFrame* avFrame = nullptr;
    AVPacket* avPacket = nullptr;
    int frameWidth = 640;
    int frameHeight = 480;
    int fps=30;

    bool running =false;
    std::mutex frameMutex;

    // 内部方法
    bool initEncoder();
    void releaseEncoder();
    void handleSignalingMessage(const std::string &msg);
    void sendSignalingMessage(const std::string &type, const std::string &sdp);
    std::vector<std::byte> encodeFrame(const cv::Mat &frame);
};

#endif // WEBRTCSTREAMER_H
