#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void showAlert(const QString &title, const QString &message);
    void processFinished();
    void playbackSliderMoved(qint64 position);
    void playbackChanged(qint64 position);
    void ensure_stopped();
    void update_buttons(QMediaPlayer::PlaybackState state);

private:
    void createWidgets();
    void setupActions();
    void createLayout();
    void initializePlayer();
    void createUI();
    void setupToolBar();
    void show_status_message(const QString &message);
    void handle_player_error(QMediaPlayer::Error error, const QString& errorString);

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
