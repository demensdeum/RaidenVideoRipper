#include <videoprocessorprogresspoller.h>
#include <dullahan_ffmpeg.h>
#include <QTimer>
#include <QDebug>
#include <QThread>

VideoProcessorProgressPoller::VideoProcessorProgressPoller(int duration)
{
    this->duration = duration;

    timer = new QTimer(this);
    connect(
        timer,
        &QTimer::timeout,
        this,
        &VideoProcessorProgressPoller::pollProgress
    );
    timer->start(tick);
}

void VideoProcessorProgressPoller::pollProgress()
{
    qDebug() << "PTS: " << dullahan_ffmpeg_conversion_pts();

    auto pts = dullahan_ffmpeg_conversion_pts() / 1000;
    auto ratio = float(pts) / float(duration);
    auto progress = int(float(ratio) * 100);

    emit didPollProgress(progress);
}

void VideoProcessorProgressPoller::stop()
{
    timer->stop();
}
