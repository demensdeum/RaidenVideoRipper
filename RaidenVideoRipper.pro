QT       += core gui

equals(QT_MAJOR_VERSION, 6): QT += widgets multimedia multimediawidgets

CONFIG += c++17

QMAKE_CXXFLAGS += -Werror

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    videoprocessor.cpp

HEADERS += \
    constants.h \
    ffmpeg_headless.h \
    mainwindow.h \
    videoprocessor.h

LIBS += C:/Users/Demensdeum/Documents/Sources/RaidenVideoRipper/headless_ffmpeg.dll.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target