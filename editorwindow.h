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
#include <QLabel>
#include <QMediaPlayer>
#include <queue>
#include <progressbarwindow.h>
#include <videowidget.h>
#include <timelinewidget.h>
#include <videoprocessor.h>
#include <outputformat.h>
#include <videoprocessorprogresspoller.h>

class EditorWindow : public QMainWindow
{
    Q_OBJECT

    enum State {
        IDLE = 0,
        FILE_PROCESSING,
        CANCELLED,
        EnumCount
    };

public:
    EditorWindow();
    void closeEvent(QCloseEvent *event);

private:
    void open();
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
    void outputFormatCheckboxStateChanged(bool isChecked);
    void handleDropUrl(QUrl url);
    void updateDurationLabel();
    void savePlaybackState();
    void restorePlaybackState();
    void showProgressbarWindow(QString text);
    void cleanupBeforeExit();
    void cancelInProgess();
    void restoreWindowSize();
    void didPollProgress(int progress);
    QString outputFormatIsSelectedKey(QString identifier);

    std::vector<OutputFormat> getSelectedOutputFormats();
    std::queue<OutputFormat> currentOutputFormats;

    static void showAlert(const QString &title, const QString &message);

    bool userForcedStop;
    
    TimelineWidget *timelineIndicator;

    QThreadPool threadPool;
    QSettings settings;

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QToolBar *toolBar;
    QAction *previewCheckboxAction;

    QSlider *volumeSlider;

    VideoWidget *videoWidget;

    QAction *openAction;

    std::optional<OutputFormat> currentOutputFormat;
    QString filePath;
    State state;

    QPushButton *playbackButton;
    QPushButton *stopButton;
    QLabel *durationLabel;

    QIcon playIcon;
    QIcon pauseIcon;
    QIcon stopIcon;

    std::optional<ProgressBarWindow *> progressBarWindow;
    std::optional<VideoProcessor *> videoProcessor;
    std::optional<VideoProcessorProgressPoller *> videoProcessorProgressPoller;

    std::tuple<QMediaPlayer::PlaybackState, int> playbackState;

    std::map<int, QString> stateToString;
    std::vector<OutputFormat> outputFormats;

    const int primaryPanelHeight = 40;
    const int secondaryPanelHeight = 40;
};
