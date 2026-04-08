#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <QLabel>
#include <QDebug>
#include <opencv2/objdetect.hpp>   // 人脸检测头文件

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
    void setFaceDetectionEnabled(bool enabled);  // 新增方法

private slots:
    void fetchFrame();

private:
    cv::VideoCapture cap;
    QLabel *videoLabel;  // 传入的 QLabel 指针
    QTimer *timer;
    cv::CascadeClassifier faceCascade;  // 人脸分类器
    bool enableFaceDetection = false;  // 人脸检测开关
};

#endif // CAMERAWIDGET_H