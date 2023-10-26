QT       += core gui

equals(QT_MAJOR_VERSION, 6): QT += widgets multimedia multimediawidgets

CONFIG += c++17

QMAKE_CXXFLAGS += -Werror

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    editorwindow.cpp \
    main.cpp \
    progressbarwindow.cpp \
    timelinesliderwidget.cpp \
    timelinewidget.cpp \
    videoprocessor.cpp \
    videoprocessorprogresspoller.cpp \
    videowidget.cpp

HEADERS += \
    constants.h \
    editorwindow.h \
    ffmpeg_headless.h \
    progressbarwindow.h \
    timelinesliderwidget.h \
    timelinewidget.h \
    utils.h \
    videoprocessor.h \
    videoprocessorprogresspoller.h \
    videowidget.h

LIBS += $$PWD/dullahan_ffmpeg.dll.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = resources\icons\applicationIcon.ico

RESOURCES += \
    resources.qrc

DISTFILES +=
