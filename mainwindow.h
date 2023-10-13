#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void open();
    void ripButtonClicked();
    void volumeSliderMoved(qint64 position);
    void rip();
    void processStarted();
    void processReadyReadStandardOutput();
    void processStateChanged();
    void processFinished();
    void playbackSliderMoved(qint64 position);
    void playbackChanged(qint64 position);
    void ensureStopped();
    void updateButtons(QMediaPlayer::PlaybackState state);

    void showAboutApplication();

private:
    void createWidgets();
    void setupActions();
    void createLayout();
    void initializePlayer();
    void createUI();
    void setupToolBar();
    void showStatusMessage(const QString &message);
    void handlePlayerError(QMediaPlayer::Error error, const QString &errorString);

    static void showAlert(const QString &title, const QString &message);

    QSettings settings;

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QToolBar *toolBar;
    QCheckBox *convertToVideoCheckbox;
    QCheckBox *convertToGifCheckbox;

    QSlider *volumeSlider;
    QSlider *playbackSlider;
    QSlider *startPositionSlider;
    QSlider *endPositionSlider;
    QPushButton *ripButton;
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

    QProcess *process;
};
#endif // MAINWINDOW_H
