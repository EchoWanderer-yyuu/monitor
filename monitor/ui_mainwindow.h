/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *startCameraBtn;
    QPushButton *stopCameraBtn;
    QLabel *peerLabel;
    QPushButton *connectBtn;
    QPushButton *disconnectBtn;
    QLabel *cameraLabel;
    QLabel *voidLabel;
    QGroupBox *featureGroup;
    QVBoxLayout *vboxLayout;
    QCheckBox *motionDetectCheck;
    QCheckBox *faceDetectCheck;
    QCheckBox *enhanceCheck;
    QLineEdit *serverInput;
    QLabel *statusLabel;
    QTextEdit *logText;
    QLabel *label_2;
    QLineEdit *peerInput;
    QLabel *logLabel;
    QLabel *label;
    QComboBox *comboBox;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(623, 430);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        startCameraBtn = new QPushButton(centralwidget);
        startCameraBtn->setObjectName(QString::fromUtf8("startCameraBtn"));
        startCameraBtn->setGeometry(QRect(300, 140, 81, 27));
        stopCameraBtn = new QPushButton(centralwidget);
        stopCameraBtn->setObjectName(QString::fromUtf8("stopCameraBtn"));
        stopCameraBtn->setGeometry(QRect(460, 140, 101, 27));
        peerLabel = new QLabel(centralwidget);
        peerLabel->setObjectName(QString::fromUtf8("peerLabel"));
        peerLabel->setGeometry(QRect(290, 30, 111, 16));
        connectBtn = new QPushButton(centralwidget);
        connectBtn->setObjectName(QString::fromUtf8("connectBtn"));
        connectBtn->setGeometry(QRect(300, 70, 91, 27));
        disconnectBtn = new QPushButton(centralwidget);
        disconnectBtn->setObjectName(QString::fromUtf8("disconnectBtn"));
        disconnectBtn->setGeometry(QRect(460, 70, 101, 27));
        cameraLabel = new QLabel(centralwidget);
        cameraLabel->setObjectName(QString::fromUtf8("cameraLabel"));
        cameraLabel->setGeometry(QRect(290, 110, 301, 16));
        voidLabel = new QLabel(centralwidget);
        voidLabel->setObjectName(QString::fromUtf8("voidLabel"));
        voidLabel->setGeometry(QRect(10, 0, 281, 261));
        voidLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        featureGroup = new QGroupBox(centralwidget);
        featureGroup->setObjectName(QString::fromUtf8("featureGroup"));
        featureGroup->setGeometry(QRect(300, 170, 211, 104));
        vboxLayout = new QVBoxLayout(featureGroup);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        motionDetectCheck = new QCheckBox(featureGroup);
        motionDetectCheck->setObjectName(QString::fromUtf8("motionDetectCheck"));
        motionDetectCheck->setEnabled(true);
        motionDetectCheck->setMouseTracking(true);
        motionDetectCheck->setCheckable(true);
        motionDetectCheck->setChecked(false);

        vboxLayout->addWidget(motionDetectCheck);

        faceDetectCheck = new QCheckBox(featureGroup);
        faceDetectCheck->setObjectName(QString::fromUtf8("faceDetectCheck"));

        vboxLayout->addWidget(faceDetectCheck);

        enhanceCheck = new QCheckBox(featureGroup);
        enhanceCheck->setObjectName(QString::fromUtf8("enhanceCheck"));

        vboxLayout->addWidget(enhanceCheck);

        serverInput = new QLineEdit(centralwidget);
        serverInput->setObjectName(QString::fromUtf8("serverInput"));
        serverInput->setGeometry(QRect(400, 0, 161, 21));
        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(10, 280, 532, 15));
        logText = new QTextEdit(centralwidget);
        logText->setObjectName(QString::fromUtf8("logText"));
        logText->setGeometry(QRect(80, 310, 391, 71));
        logText->setReadOnly(true);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(290, 0, 121, 21));
        peerInput = new QLineEdit(centralwidget);
        peerInput->setObjectName(QString::fromUtf8("peerInput"));
        peerInput->setGeometry(QRect(400, 30, 161, 21));
        logLabel = new QLabel(centralwidget);
        logLabel->setObjectName(QString::fromUtf8("logLabel"));
        logLabel->setGeometry(QRect(10, 310, 71, 16));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(516, 170, 81, 20));
        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(530, 190, 86, 25));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 623, 28));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        startCameraBtn->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #2196F3; color: white; padding: 6px; border-radius: 4px;", nullptr));
        startCameraBtn->setText(QCoreApplication::translate("MainWindow", "\345\220\257\345\212\250\346\221\204\345\203\217\345\244\264", nullptr));
        stopCameraBtn->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FF9800; color: white; padding: 6px; border-radius: 4px;", nullptr));
        stopCameraBtn->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\346\221\204\345\203\217\345\244\264", nullptr));
        peerLabel->setText(QCoreApplication::translate("MainWindow", "\345\257\271\347\253\257 ID:", nullptr));
        connectBtn->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #4CAF50; color: white; padding: 6px; border-radius: 4px;", nullptr));
        connectBtn->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245", nullptr));
        disconnectBtn->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #f44336; color: white; padding: 6px; border-radius: 4px;", nullptr));
        disconnectBtn->setText(QCoreApplication::translate("MainWindow", "\346\226\255\345\274\200", nullptr));
        cameraLabel->setText(QCoreApplication::translate("MainWindow", "\346\234\254\345\234\260\346\221\204\345\203\217\345\244\264:", nullptr));
        voidLabel->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-weight:600; color:#ffffff;\">\347\255\211\345\276\205\350\247\206\351\242\221\346\265\201</span></p></body></html>", nullptr));
        featureGroup->setTitle(QCoreApplication::translate("MainWindow", "\345\212\237\350\203\275\345\274\200\345\205\263", nullptr));
        motionDetectCheck->setText(QCoreApplication::translate("MainWindow", "\350\277\220\345\212\250\346\243\200\346\265\213", nullptr));
        faceDetectCheck->setText(QCoreApplication::translate("MainWindow", "\344\272\272\350\204\270\346\243\200\346\265\213", nullptr));
        enhanceCheck->setText(QCoreApplication::translate("MainWindow", "\345\233\276\345\203\217\345\242\236\345\274\272", nullptr));
        serverInput->setText(QCoreApplication::translate("MainWindow", "ws://localhost:8080", nullptr));
        statusLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "color: gray; font-weight: bold;", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201: \346\234\252\350\277\236\346\216\245", nullptr));
        logText->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #1e1e1e; color: #00ff00; font-family: monospace; font-size: 11px;", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\344\277\241\344\273\244\346\234\215\345\212\241\345\231\250\345\234\260\345\235\200:", nullptr));
        peerInput->setText(QCoreApplication::translate("MainWindow", "peer-1", nullptr));
        logLabel->setText(QCoreApplication::translate("MainWindow", "\350\277\220\350\241\214\346\227\245\345\277\227:", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\345\210\207\346\215\242\346\221\204\345\203\217\345\244\264", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\2641", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\2642", nullptr));

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
