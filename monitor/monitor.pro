QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += multimedia multimediawidgets core

# FFmpeg
LIBS += -lavcodec -lavformat -lavutil -lswscale

# libdatachannel
LIBS += -L/usr/local/lib -ldatachannel
INCLUDEPATH += /usr/local/include

# OpenCV
INCLUDEPATH += $$system(pkg-config --cflags-only-I opencv4 | sed 's/-I//g')
LIBS += $$system(pkg-config --libs opencv4)

SOURCES += \
 camerawidget.cpp \
 main.cpp \
 mainwindow.cpp \
 webrtcstreamer.cpp \

HEADERS += \
 camerawidget.h \
 mainwindow.h \
 webrtcstreamer.h \
 webrtcstreamer.h

FORMS += \
 mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
