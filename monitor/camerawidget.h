#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <QLabel>
#include <QDebug>
#include <opencv2/objdetect.hpp>   // 人脸检测头文件
#include "webrtcstreamer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidget(QLabel *videoLabel, QWidget *parent = nullptr);
    ~CameraWidget();
    
    void startCamera();
    void stopCamera();
    void setFaceDetectionEnabled(bool enabled);  // 设置人脸检测开关    
    void setMotionDetectionEnabled(bool enabled); // 设置运动检测开关
    void setEnhancementEnabled(bool enabled); //设置画面增强开关
    void applyEnhancement(cv::Mat &frame); //画面增强函数
    void switchCamera(int cameraIndex); //切换摄像头
    void startWebRTC(const std::string &signnalUrl); //启动WebRTC流媒体器
    void stopWebRTC(); //停止WebRTC流媒体器

signals:
    void logMessage(const QString &msg);  // 日志信号

private slots:
    void fetchFrame();

private:
    cv::VideoCapture cap;
    QLabel *videoLabel;  // 传入的 QLabel 指针
    QTimer *timer;
    cv::CascadeClassifier faceCascade;  // 人脸分类器
    bool enableFaceDetection = false;  // 人脸检测开关
    cv::Ptr<cv::BackgroundSubtractor> pBackSub;  // 背景减除器
    bool enableMotionDetection = false;  // 运动检测开关
    bool enableEnhancement = false;  // 画面增强开关
    webrtcstreamer *webrtcStreamer=nullptr;  // WebRTC 流媒体器指针
};

#endif // CAMERAWIDGET_H