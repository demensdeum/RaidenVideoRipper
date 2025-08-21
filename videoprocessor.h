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
        const QString& videoPath,
        const std::vector<QString> customArguments,
        const QString& outputVideoPath
        );

    void run() override;

signals:
    void videoProcessingDidFinish(int result);

public slots:
    void cancel();

private:
    // class beign called with setAutoDelete(true), so that's better to keep those private
    const int startPosition;
    const int endPosition;
    const QString videoPath;
    const std::vector<QString> customArguments;
    const QString outputVideoPath;
};
