#include "videoprocessor.h"

extern "C" {
#include <dullahan_ffmpeg.h>
}

#include <QDebug>

VideoProcessor::VideoProcessor(
    const int startPosition,
    const int endPosition,
    const QString videoPath,
    const QString outputVideoPath
)
: startPosition(startPosition),
endPosition(endPosition),
videoPath(videoPath),
outputVideoPath(outputVideoPath)
{
}

void VideoProcessor::cancel()
{
    dullahan_ffmpeg_cancel();
}

void VideoProcessor::run()
{
    auto arguments = std::vector<std::string> {
        "ffmpeg",
        "-y",
        "-i",
        videoPath.toStdString(),
        "-ss",
        std::to_string(startPosition) + "ms",
        "-to",
        std::to_string(endPosition) + "ms",
        outputVideoPath.toStdString()
    };

    std::vector<char*> argv;
    for (const auto& arg : arguments) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }

    auto result = dullahan_ffmpeg_main(argv.size(), argv.data());
    emit videoProcessingDidFinish(result);
}
