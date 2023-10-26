#pragma once

#include <QRunnable>
#include <QString>
#include <QObject>
#include <QThread>
#include <QTimer>

class VideoProcessorProgressPoller: public QObject
{
    Q_OBJECT
public:
    VideoProcessorProgressPoller(int duration);

    void start();
    void pollProgress();
    void stop();

private:
    QTimer *timer;
    int duration;

signals:
    void didPollProgress(int progress);
};
