#pragma once

#include <QRunnable>
#include <QString>
#include <QObject>

class VideoProcessor : public QObject, public QRunnable
{
    Q_OBJECT
public:
    VideoProcessor(
        int startPosition,
        int endPosition,
        QString videoPath,
        QString outputVideoPath
    );

    void run() override;

signals:
    void videoProcessingDidFinish(int result);

private:
    int startPosition;
    int endPosition;
    QString videoPath;
    QString outputVideoPath;
};
