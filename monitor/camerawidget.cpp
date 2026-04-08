#include "camerawidget.h"

CameraWidget::CameraWidget(QLabel *videoLabel, QWidget *parent)
    : QWidget(parent), videoLabel(videoLabel), timer(nullptr)
{
    cap.open(0, cv::CAP_V4L2);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

     if(!cap.isOpened()) {
        qDebug() << "摄像头打开失败！";
        return;
    }
    qDebug() << "后端:" << QString::fromStdString(cap.getBackendName());

    qDebug() << "是否打开:" << cap.isOpened();


    if (videoLabel) {
        videoLabel->setAlignment(Qt::AlignCenter);
        videoLabel->setScaledContents(true); // 图像自适应 Label 大小
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CameraWidget::fetchFrame);
    timer->start(33);  //30fps
}

CameraWidget::~CameraWidget()
{
    if (timer && timer->isActive()) {
        timer->stop();
    }
}

void CameraWidget::startCamera()
{
    if (timer && !timer->isActive()) {
        timer->start(33);  // 约30fps
    }
}

void CameraWidget::stopCamera()
{
    if (timer && timer->isActive()) {
        timer->stop();
    }
}

void CameraWidget::setFaceDetectionEnabled(bool enabled)
{
    enableFaceDetection = enabled;
    
    if (enableFaceDetection) {
        //加载的人脸分类器路径
        QStringList cascadePaths = {
            "/usr/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml"
        };
        
        bool loaded = false;
        
        if (faceCascade.load(cascadePaths[0].toStdString())) {
            qDebug() << "人脸分类器加载成功:" << cascadePaths[0];
            loaded = true;
        }
        
        
        if (!loaded) {
            qDebug() << "所有路径都无法加载人脸分类器";
        }
    }
}

void CameraWidget::fetchFrame()
{
    if (!videoLabel) {
        qDebug() << "视频标签未设置";
        return;
    }
    
    cv::Mat frame;
    cap >> frame;
    if(frame.empty()) {
        qDebug() << "帧为空";
        return;
    }

    //人脸检测
    if(enableFaceDetection)
    {
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY); // 转灰度
        cv::equalizeHist(gray, gray); // 均衡化直方图\

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(
            gray,
            faces,
            1.1,    // scaleFactor：每次缩小比例，越小越准但越慢
            3,      // minNeighbors：候选框至少被检测到几次才算人脸，越大误检越少
            0,
            cv::Size(60, 60)  // 最小人脸尺寸，过滤掉太小的噪声
        );

        // 在每个检测到的人脸上画绿色矩形框
        for (const auto& face : faces) {
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);

             // 在框上方显示文字
            cv::putText(frame, "Face", cv::Point(face.x, face.y - 5),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
        }


    }
    
    //Mat->QImage
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    //显示到传入的 videoLabel
    videoLabel->setPixmap(QPixmap::fromImage(img));
}