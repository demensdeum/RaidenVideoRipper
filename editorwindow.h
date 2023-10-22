#pragma once

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QToolBar>
#include <QCheckBox>
#include <QSlider>
#include <QPushButton>
#include <QProcess>
#include <QString>
#include <QVBoxLayout>
#include <QSettings>
#include <QThreadPool>
#include <QLabel>
#include <QMediaPlayer>
#include "editorvideowidget.h"
#include <videowidget.h>
#include <timelinewidget.h>
#include <progressindicatorwidget.h>

class EditorWindow : public QMainWindow
{
    Q_OBJECT

    enum State {
        IDLE = 0,
        VIDEO_PROCESSING,
        GIF_PROCESSING,
        EnumCount
    };

public:
    EditorWindow();

private:
    void open();
    void setupProgressIndicator();
    void startButtonClicked();
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
    void handleLeftKeyPress();
    void handleRightKeyPress();
    void startSliderDraggingStarted();
    void startSliderDraggingFinished();
    void playbackSliderDraggingStarted();
    void playbackSliderDraggingFinished();
    void endSliderDraggingStarted();
    void endSliderDraggingFinished();
    void openArgumentsFileIfNeeded();
    void handleOpenFile(QUrl url);
    void updateWindowTitle();
    void createWidgets();
    void setupActions();
    void createLayout();
    void initializePlayer();
    void createUI();
    void previewCheckboxStateChange(bool checked);
    void playToggleButtonClicked();
    void showStatusMessage(const QString &message);
    void handlePlayerError(QMediaPlayer::Error error, const QString &errorString);
    void checkboxVideoStateChanged(int _state);
    void checkboxGifStateChanged(int _state);
    void handleDropUrl(QUrl url);
    void updateDurationLabel();
    void savePlaybackState();
    void restorePlaybackState();

    static void showAlert(const QString &title, const QString &message);

    bool userForcedStop;
    
    QWidget *transparentWidget;

    TimelineWidget *timelineIndicator;

    QThreadPool threadPool;
    QSettings settings;

    QToolBar *toolBar;
    QAction *previewCheckboxAction;
    QAction *convertToVideoCheckboxAction;
    QAction *convertToGifCheckboxAction;

    QSlider *volumeSlider;

    EditorVideoWidget *editorVideoWidget;

    QAction *openAction;

    QString videoPath;
    QString ffmpegPath;
    State state;

    QPushButton *playbackButton;
    QPushButton *stopButton;
    QLabel *durationLabel;

    QIcon playIcon;
    QIcon pauseIcon;
    QIcon stopIcon;
    
    std::tuple<QMediaPlayer::PlaybackState, int> savedPlaybackState;

    std::map<int, QString> stateToString;

    const int primaryPanelHeight = 40;
    const int secondaryPanelHeight = 40;
};
