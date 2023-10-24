#pragma once

#include <QString>
#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsVideoItem>

class EditorVideoWidget: public QWidget
{
    Q_OBJECT
public:
    EditorVideoWidget(QWidget *parent);

    void setSource(QUrl url);

    void play();
    void stop();
    void pause();

    void setVolume(float volume);
    void setPosition(int position);

    int position();
    int duration();

    bool isPlaying();

    void showProgressIndicator(QString text, int progress);
    void hideProgressIndicator();

    void resizeEvent(QResizeEvent* event);

    QMediaPlayer::PlaybackState getPlaybackState();

signals:
    void positionChanged(int position);
    void playbackStateChanged(QMediaPlayer::PlaybackState state);
    void errorOccured(QMediaPlayer::Error error, const QString &errorString);
    void dragDidDropUrl(QUrl url);

private:
    QGraphicsVideoItem *item;
    QAudioOutput audioOutput;
    QMediaPlayer mediaPlayer;
    QGraphicsScene *scene;
    QGraphicsView *graphicsView;

    const int bottomMargin = 100;
};
