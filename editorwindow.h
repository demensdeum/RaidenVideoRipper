#pragma once

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
#include <workspaceindicator.h>

class EditorWindow : public QMainWindow
{
    Q_OBJECT

    enum State {
        VIDEO = 0,
        GIF,
        EnumCount
    };

public:
    EditorWindow();

private slots:
    void open();
    void cutButtonClicked();
    void volumeChanged(qint64 position);
    void cut();
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
    void convertingDidFinish(bool result);

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
    void previewCheckboxStateChange(int state);

    static void showAlert(const QString &title, const QString &message);

    bool userForcedStop;

    WorkspaceIndicator *workspaceIndicator;

    QThreadPool threadPool;
    QSettings settings;

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QToolBar *toolBar;
    QCheckBox *convertToVideoCheckbox;
    QCheckBox *convertToGifCheckbox;
    QCheckBox *previewCheckbox;

    QSlider *volumeSlider;

    QVideoWidget *videoWidget;

    QAction *openAction;
    QAction *playAction;
    QAction *pauseAction;
    QAction *stopAction;

    QString videoPath;
    QString ffmpegPath;
    State state;

    std::map<int, QString> stateToString;
};
