#include "camerawidget.h"

CameraWidget::CameraWidget(QLabel *videoLabel, QWidget *parent)
    : QWidget(parent), videoLabel(videoLabel), timer(nullptr)
{
    cap.open(0, cv::CAP_V4L2);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

     if(!cap.isOpened()) {
        qDebug() << "摄像头打开失败！";
        emit logMessage("[错误] 摄像头打开失败");
        return;
    }
    emit logMessage("[信息] 摄像头已启动，后端: " + QString::fromStdString(cap.getBackendName()));
    qDebug() << "后端:" << QString::fromStdString(cap.getBackendName());

    qDebug() << "是否打开:" << cap.isOpened();

    // 初始化背景减除器
    pBackSub=cv::createBackgroundSubtractorMOG2(500,16,true);


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
            emit logMessage("[信息] 人脸分类器加载成功:" + cascadePaths[0]);
        }
        
        
        if (!loaded) {
            qDebug() << "路径都无法加载人脸分类器";
            emit logMessage("[错误] 路径都无法加载人脸分类器");
        }
    }
}

void CameraWidget::setMotionDetectionEnabled(bool enabled)
{
    enableMotionDetection = enabled;
    emit logMessage(enabled ? "[信息] 运动检测已开启" : "[信息] 运动检测已关闭");
}

void CameraWidget::setEnhancementEnabled(bool enabled)
{
    enableEnhancement = enabled;
    emit logMessage(enabled ? "[信息] 画面增强已开启" : "[信息] 画面增强已关闭");
}

void CameraWidget::switchCamera(int cameraIndex)
{
    // 先停止当前的
    if(timer&&timer->isActive())
    {
        timer->stop();
    }
    cap.release();
    // 打开新摄像头
    cap.open(cameraIndex,cv::CAP_V4L2);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    if(!cap.isOpened()) {
        qDebug() << "摄像头打开失败！";
        emit logMessage("[错误] 摄像头 " + QString::number(cameraIndex) + " 打开失败");
        // 修改 videoLabel 为错误文字
        videoLabel->clear();  // 清除当前图像
        videoLabel->setStyleSheet("background-color: black;");  // 设置黑色背景
        videoLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-weight:600; color:#ffffff;\">摄像头打开错误</span></p></body></html>");  // 显示等待文字
        videoLabel->setAlignment(Qt::AlignCenter);  // 文字居中
        return;
    }
    qDebug() << "摄像头打开成功！";
    emit logMessage("[信息] 已切换到摄像头 " + QString::number(cameraIndex));
    // 重新启动定时器
    if(timer)
    {
        timer->start(33);
    }
    
}

void CameraWidget::applyEnhancement(cv::Mat &frame)
{
    // 转灰度，做 CLAHE（局部对比度增强）
    // 灰度图用于计算直方图，处理完再合并回彩色
    cv::Mat lab;
    cv::cvtColor(frame, lab, cv::COLOR_BGR2Lab);  // BGR转Lab色彩空间

    std::vector<cv::Mat> channel;
    cv::split(lab, channel);  // 分离通道：L、a、b
    // 创建CLAHE：clipLimit=2.0（对比度限制），tileGridSize=8x8（局部块大小）
    cv::Ptr<cv::CLAHE> clahe=cv::createCLAHE(2.0,cv::Size(8,8));
    clahe->apply(channel[0], channel[0]); // 只对L通道（亮度）处理
    cv::merge(channel, lab);  // 合并通道：L、a、b
    cv::cvtColor(lab, frame, cv::COLOR_Lab2BGR);  // 转回BGR
    // 降噪（弱光环境噪点多，这个效果好但有性能开销）
    //cv::fastNlMeansDenoisingColored(frame, frame, 10, 10, 7, 21);
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

    //运动检测
    if(enableMotionDetection && !pBackSub.empty())
    {
        // 背景减除，得到前景掩码
        cv::Mat fgMask;
        pBackSub->apply(frame, fgMask);   // fgMask: 白色(255)=运动，灰色(127)=阴影，黑色(0)=背景

         // 二值化，把阴影也变成黑色，只留运动
        cv::threshold(fgMask, fgMask, 200, 255, cv::THRESH_BINARY_INV);
        // 形态学去噪
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
        cv::morphologyEx(fgMask, fgMask, cv::MORPH_OPEN, kernel);   // 开运算：去小噪点
        cv::dilate(fgMask, fgMask, kernel, cv::Point(-1, -1), 2);   // 膨胀：连断裂区域

        // 找轮廓
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(fgMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // 过滤小面积，画框
        for(const auto& contour : contours) {
            double area=cv::contourArea(contour);
            if(area<100) {   // 太小就跳过
                continue;
            }

            cv::Rect boundrect=cv::boundingRect(contour);
            cv::rectangle(frame, boundrect, cv::Scalar(0, 255, 0), 2);
            cv::putText(frame, "Motion", cv::Point(boundrect.x, boundrect.y - 5),   
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
        }


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
        static int frameCount = 0;
        if (++frameCount % 30 == 0 && !faces.empty()) {
            emit logMessage("[检测] 发现 " + QString::number(faces.size()) + " 张人脸");
        }

        // 在每个检测到的人脸上画绿色矩形框
        for (const auto& face : faces) {
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);

             // 在框上方显示文字
            cv::putText(frame, "Face", cv::Point(face.x, face.y - 5),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
        }


    }

    //图像增强
    if(enableEnhancement)
    {
        applyEnhancement(frame);
    }
    //Mat->QImage
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    //显示到传入的 videoLabel
    videoLabel->setPixmap(QPixmap::fromImage(img));
}
