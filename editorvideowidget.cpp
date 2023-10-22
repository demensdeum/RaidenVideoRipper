#include "editorvideowidget.h"
#include <QGraphicsVideoItem>

EditorVideoWidget::EditorVideoWidget(QWidget *parent) : QWidget(parent) {
    scene = new QGraphicsScene();
    graphicsView = new QGraphicsView(parent);
    graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    graphicsView->setBackgroundBrush(Qt::black);
    graphicsView->setScene(scene);
    graphicsView->resetTransform();
    graphicsView->scale(3, 3);

    QGraphicsVideoItem *item = new QGraphicsVideoItem;
    mediaPlayer.setAudioOutput(&audioOutput);
    mediaPlayer.setVideoOutput(item);
    graphicsView->scene()->addItem(item);
    graphicsView->scene()->addText("Заебок");
    graphicsView->setGeometry(0,0,640,480);

    // Load the video file
    mediaPlayer.setSource(QUrl::fromLocalFile("C:/msys64/home/Demensdeum/Videos/Microsoft Windows 95 video Guide with Jennifer Aniston and Matthew Perry.mp4"));
    mediaPlayer.play();
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

void EditorVideoWidget::setVolume(float volume) {
    if (volume > 1) {
        return;
    }
    if (volume < 0) {
        return;
    }
    audioOutput.setVolume(volume);
}

int EditorVideoWidget::position() {
    return mediaPlayer.position();
}

int EditorVideoWidget::duration() {
    return mediaPlayer.duration();
}

void EditorVideoWidget::showProgressIndicator([[maybe_unused]]QString text, [[maybe_unused]]int progress) {

}

void EditorVideoWidget::hideProgressIndicator() {

}
