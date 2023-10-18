#include "videoprocessor.h"

extern "C" {
#include "ffmpeg_headless.h"
}

#include <QDebug>

VideoProcessor::VideoProcessor(
    int startPosition,
    int endPosition,
    QString videoPath,
    QString outputVideoPath
)
{
    this->startPosition = startPosition;
    this->endPosition = endPosition;
    this->videoPath = videoPath;
    this->outputVideoPath = outputVideoPath;
}

void VideoProcessor::run()
{
    std::vector<std::string> arguments;
    arguments.emplace_back("ffmpeg");
    arguments.emplace_back("-y");
    arguments.emplace_back("-i");
    arguments.emplace_back(videoPath.toStdString());
    arguments.emplace_back("-ss");
    arguments.emplace_back(std::to_string(startPosition) + "ms");
    arguments.emplace_back("-to");
    arguments.emplace_back(std::to_string(endPosition) + "ms");
    arguments.emplace_back(outputVideoPath.toStdString());

    std::vector<char*> argv;
    for (const auto& arg : arguments) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }

    auto result = headless_main(argv.size(), argv.data());
    emit videoProcessingDidFinish(result);
}
