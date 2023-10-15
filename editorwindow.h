#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QToolBar>
#include <QCheckBox>
#include <QSlider>
#include <QPushButton>
#include <QVideoWidget>
#include <QProcess>
#include <QString>
#include <QVBoxLayout>
#include <QSettings>
#include <QThreadPool>

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    EditorWindow();

private slots:
    void open();
    void cutButtonClicked();
    void volumeChanged(qint64 position);
    void cut();
    void processStarted();
    void processReadyReadStandardOutput();
    void processStateChanged();
    void stopButtonClicked();
    void playbackSliderMoved(qint64 position);
    void playbackChanged(qint64 position);
    void ensureStopped();
    void playbackStateChanged(QMediaPlayer::PlaybackState state);
    void updateButtons(QMediaPlayer::PlaybackState state);
    void togglePlayback();
    void startPositionSliderMoved(qint64 position);
    void endPositionSliderMoved(qint64 position);
    void showAboutApplication();
    void processDidFinish(bool isSuccess);

private:
    void createWidgets();
    void setupActions();
    void createLayout();
    void initializePlayer();
    void createUI();
    void setupToolBar();
    void playButtonClicked();
    void showStatusMessage(const QString &message);
    void handlePlayerError(QMediaPlayer::Error error, const QString &errorString);

    static void showAlert(const QString &title, const QString &message);

    bool userForcedStop;
    QThreadPool threadPool;
    QSettings settings;

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QToolBar *toolBar;
    QCheckBox *convertToVideoCheckbox;
    QCheckBox *convertToGifCheckbox;
    QCheckBox *previewCheckbox;

    QSlider *volumeSlider;
    QSlider *playbackSlider;
    QSlider *startPositionSlider;
    QSlider *endPositionSlider;
    QVideoWidget *videoWidget;

    QAction *openAction;
    QAction *playAction;
    QAction *pauseAction;
    QAction *stopAction;

    QString videoPath;
    QString ffmpegPath;
    QString state;

    const QString VIDEO_STATE = "VIDEO";
    const QString GIF_STATE = "GIF";
};
#endif // EDITORWINDOW_H
