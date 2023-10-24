#include "editorvideowidget.h"
#include <QGraphicsVideoItem>
#include <QResizeEvent>

EditorVideoWidget::EditorVideoWidget(QWidget *parent) : QWidget(parent) {
    scene = new QGraphicsScene();
    graphicsView = new QGraphicsView(parent);
    graphicsView->setContentsMargins(0, 0, 0, 0);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    graphicsView->setBackgroundBrush(Qt::red);
    graphicsView->setScene(scene);

    item = new QGraphicsVideoItem;
    //item->setAspectRatioMode(Qt::IgnoreAspectRatio);
    mediaPlayer.setAudioOutput(&audioOutput);
    mediaPlayer.setVideoOutput(item);
    connect(&mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &EditorVideoWidget::playbackStateChanged);
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &EditorVideoWidget::positionChanged);
    connect(&mediaPlayer, &QMediaPlayer::errorOccurred, this, &EditorVideoWidget::errorOccured);
    graphicsView->scene()->addItem(item);
    graphicsView->setGeometry(0, 0, parent->width(), parent->height());
}

void EditorVideoWidget::resizeEvent(QResizeEvent* event) {
    auto width = event->size().width();
    auto height = event->size().height();
    graphicsView->setGeometry(0, 0, width, height);
    item->setSize(QSize(width, height));
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
