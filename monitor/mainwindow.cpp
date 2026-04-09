#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "camerawidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if (cameraWidget) {
        delete cameraWidget;
        cameraWidget = nullptr;
    }
    delete ui;
}

void MainWindow::on_connectBtn_clicked()
{
   
}
void MainWindow::on_disconnectBtn_clicked()
{


}
void MainWindow::on_startCameraBtn_clicked()
{
 // 如果摄像头部件不存在，则创建它
    if (!cameraWidget) {
        cameraWidget = new CameraWidget(ui->voidLabel, this);
    }
    
    // 启动摄像头
    cameraWidget->startCamera();
    
    // 更新状态
    ui->statusLabel->setText("状态: 摄像头已连接");
}


void MainWindow::on_stopCameraBtn_clicked()
{
    if (cameraWidget) {
        cameraWidget->stopCamera();
        
        // 重置 voidLabel 为初始状态
        ui->voidLabel->clear();  // 清除当前图像
        ui->voidLabel->setStyleSheet("background-color: black;");  // 设置黑色背景
        ui->voidLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-weight:600; color:#ffffff;\">等待视频流</span></p></body></html>");  // 显示等待文字
        ui->voidLabel->setAlignment(Qt::AlignCenter);  // 文字居中
    }

    ui->statusLabel->setText("状态: 摄像头已断开");
}


void MainWindow::on_motionDetectCheck_stateChanged(int arg1)
{
   if (cameraWidget) {
        cameraWidget->setMotionDetectionEnabled(arg1 != 0);
    }
}
void MainWindow::on_faceDetectCheck_stateChanged(int arg1)
{
     this->faceflat = arg1;
    
    // 如果摄像头正在运行，通知 cameraWidget 更新人脸检测状态
    if (cameraWidget) {
        cameraWidget->setFaceDetectionEnabled(arg1 != 0);
    }
}


void MainWindow::on_enhanceCheck_stateChanged(int arg1)
{
    if (cameraWidget) {
        cameraWidget->setEnhancementEnabled(arg1 != 0);
    }
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(cameraWidget)
    {
        cameraWidget->switchCamera(index);
    }
}

