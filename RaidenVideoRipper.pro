VERSION = "1.0.0.0"
APPLICATION_NAME = "Raiden\\ Video\\ Ripper"
COMPANY_NAME = "DemensDeum"
COMPANY_DOMAIN = "demensdeum.com"

DEFINES += RAIDEN_VIDEO_RIPPER_APPLICATION_VERSION=\\\"$$VERSION\\\" \
            RAIDEN_VIDEO_RIPPER_APPLICATION_NAME=\\\"$$APPLICATION_NAME\\\" \
            RAIDEN_VIDEO_RIPPER_COMPANY_NAME=\\\"$$COMPANY_NAME\\\" \
            RAIDEN_VIDEO_RIPPER_COMPANY_DOMAIN=\\\"$$COMPANY_DOMAIN\\\"

QT       += core gui

equals(QT_MAJOR_VERSION, 6): QT += widgets multimedia multimediawidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

#FORMS += \
#    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
