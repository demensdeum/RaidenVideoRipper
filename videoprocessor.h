#pragma once

#include <QRunnable>
#include <QString>
#include <QObject>

class VideoProcessor : public QObject, public QRunnable
{
    Q_OBJECT
public:
    VideoProcessor(
        const int startPosition,
        const int endPosition,
        const QString videoPath,
        const QString outputVideoPath
        );

    void run() override;

    int startPosition;
    int endPosition;
    QString videoPath;
    QString outputVideoPath;

signals:
    void videoProcessingDidFinish(int result);

public slots:
    void cancel();

};
