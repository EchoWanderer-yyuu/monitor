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
    
    //Mat->QImage
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    //显示到传入的 videoLabel
    videoLabel->setPixmap(QPixmap::fromImage(img));
}