#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <QLabel>
#include <QDebug>

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

private slots:
    void fetchFrame();

private:
    cv::VideoCapture cap;
    QLabel *videoLabel;  // 传入的 QLabel 指针
    QTimer *timer;
};

#endif // CAMERAWIDGET_H