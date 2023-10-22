#pragma once

#include <QString>
#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>

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

    QMediaPlayer::PlaybackState getPlaybackState();

signals:
    void positionChanged(int position);
    void playbackStateChanged(QMediaPlayer::PlaybackState state);
    void errorOccured(QMediaPlayer::Error error, const QString &errorString);
    void dragDidDropUrl(QUrl url);

private:
    QAudioOutput audioOutput;
    QMediaPlayer mediaPlayer;

    float maximalAudioOutputValue = 1;
};
