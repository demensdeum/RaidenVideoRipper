#include "editorvideowidget.h"

EditorVideoWidget::EditorVideoWidget(QWidget *parent) : QWidget(parent) {

}

void EditorVideoWidget::setPosition(int position) {
    mediaPlayer.setPosition(position);
}

QMediaPlayer::PlaybackState EditorVideoWidget::getPlaybackState()
{
    return mediaPlayer.playbackState();
}

bool EditorVideoWidget::isPlaying()
{
    return mediaPlayer.isPlaying();
}


void EditorVideoWidget::setSource(QUrl url) {
    mediaPlayer.setSource(url);
}

void EditorVideoWidget::play() {
    mediaPlayer.play();
}

void EditorVideoWidget::stop() {
    mediaPlayer.stop();
}

void EditorVideoWidget::pause() {
    mediaPlayer.pause();
}

void EditorVideoWidget::setVolume([[maybe_unused]]float volume) {
    if (volume > 1) {
        return;
    }
    if (volume < 0) {
        return;
    }
    auto audioOutputValue = float(volume) / float(maximalAudioOutputValue);
    audioOutput.setVolume(audioOutputValue);
}

int EditorVideoWidget::position() {
    return 0;
}

int EditorVideoWidget::duration() {
    return 0;
}

void EditorVideoWidget::showProgressIndicator([[maybe_unused]]QString text, [[maybe_unused]]int progress) {

}

void EditorVideoWidget::hideProgressIndicator() {

}
