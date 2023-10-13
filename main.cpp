#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName(RAIDEN_VIDEO_RIPPER_APPLICATION_NAME);
    application.setApplicationVersion(RAIDEN_VIDEO_RIPPER_APPLICATION_VERSION);
    MainWindow window;
    window.show();
    return application.exec();
}
