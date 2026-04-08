#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CameraWidget;  // 前置声明

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectBtn_clicked();

    void on_disconnectBtn_clicked();

    void on_startCameraBtn_clicked();

    void on_stopCameraBtn_clicked();
    void on_motionDetectCheck_stateChanged(int arg1);

    void on_faceDetectCheck_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    int faceflat = 0;  // 将 faceflat 作为 MainWindow 的私有成员变量
    CameraWidget *cameraWidget = nullptr;  // 摄像头部件
};
#endif // MAINWINDOW_H
