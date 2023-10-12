#include "mainwindow.h"

#include <QApplication>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QFileDialog>
#include <QAction>
#include <QSlider>
#include <QToolBar>
#include <QMenuBar>
#include <QMessageBox>
#include <QSize>
#include <QStandardPaths>
#include <QStyle>
#include <QStatusBar>
#include <QScreen>

MainWindow::MainWindow()
{
    state = VIDEO_STATE;

    createUI();
    createLayout();
    initializePlayer();
    setupActions();
    setupToolBar();

    update_buttons(player->playbackState());

    ffmpegPath = qgetenv("FFMPEG_BINARY");
    if (ffmpegPath.isEmpty())
    {
        showAlert("WHAT!", "INSTALL FFMPEG and add FFMPEG_BINARY Environment variable to ffmpeg.exe!!!");
        qWarning() << "WUT NO FFMPEG_BINARY ENVIRONMENT VARIABLE?! EXIT!";
        exit(1);
    }
}

void MainWindow::createUI()
{
    setWindowTitle("Raiden Video Ripper");
}

void MainWindow::setupActions()
{
    openAction = new QAction("Open...", this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::open);

    QAction *exitAction = new QAction("Exit", this);
    //exitAction->setShortcut("Ctrl+Q");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    // Добавьте элементы меню "File" только к главному меню
    QMenu *file_menu = menuBar()->addMenu("&File");
    file_menu->addAction(openAction);
    file_menu->addAction(exitAction);
}

void MainWindow::createLayout()
{
    videoWidget = new QVideoWidget();
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatio);  // Установите желаемый режим соотношения сторон
    videoWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(videoWidget);

    playbackSlider = new QSlider(Qt::Horizontal);
    layout->addWidget(playbackSlider);
    startPositionSlider = new QSlider(Qt::Horizontal);
    layout->addWidget(startPositionSlider);
    endPositionSlider = new QSlider(Qt::Horizontal);
    layout->addWidget(endPositionSlider);

    ripButton = new QPushButton("RIP EM!");
    connect(ripButton, &QPushButton::clicked, this, &MainWindow::ripButtonClicked);
    layout->addWidget(ripButton);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);

    this->resize(800, 600);
}

void MainWindow::initializePlayer()
{
    audioOutput = new QAudioOutput();
    player = new QMediaPlayer();
    player->setAudioOutput(audioOutput);

    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::playbackChanged);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &MainWindow::update_buttons);
    connect(player, &QMediaPlayer::errorOccurred, this, &MainWindow::handle_player_error);
    player->setVideoOutput(videoWidget);
}

void MainWindow::setupToolBar()
{
    toolBar = new QToolBar();
    addToolBar(toolBar);

    toolBar->addAction(openAction);

    // Создание меню "Play"
    QMenu* playMenu = menuBar()->addMenu("&Play");
    QStyle* style = this->style();

    // Создание действия "Play"
    playAction = new QAction("Play", this);
    QIcon playIcon = QIcon::fromTheme("media-playback-start.png", style->standardIcon(QStyle::SP_MediaPlay));
    playAction->setIcon(playIcon);
    connect(playAction, &QAction::triggered, player, &QMediaPlayer::play);
    toolBar->addAction(playAction);
    playMenu->addAction(playAction);

    // About
    QMenu* about_menu = menuBar()->addMenu("&About");
    about_qt_action = new QAction("About &Qt", this);
    connect(about_qt_action, &QAction::triggered, qApp, &QApplication::aboutQt);
    about_menu->addAction(about_qt_action);

    // Создание действия "Pause"
    pauseAction = new QAction("Pause", this);
    QIcon pauseIcon = QIcon::fromTheme("media-playback-pause.png", style->standardIcon(QStyle::SP_MediaPause));
    pauseAction->setIcon(pauseIcon);
    toolBar->addAction(pauseAction);
    playMenu->addAction(pauseAction);
    connect(pauseAction, &QAction::triggered, player, &QMediaPlayer::pause);

    // Создание действия "Stop"
    stopAction = new QAction("Stop", this);
    QIcon stopIcon = QIcon::fromTheme("media-playback-stop.png", style->standardIcon(QStyle::SP_MediaStop));
    stopAction->setIcon(stopIcon);
    toolBar->addAction(stopAction);
    playMenu->addAction(stopAction);
    connect(stopAction, &QAction::triggered, this, &MainWindow::ensure_stopped);

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setMinimum(0);
    volumeSlider->setMaximum(100);
    volumeSlider->setTickInterval(10);
    volumeSlider->setTickPosition(QSlider::TicksBelow);
    volumeSlider->setToolTip("Volume");
    volumeSlider->setValue(volumeSlider->maximum());
    connect(volumeSlider, &QSlider::valueChanged, this, &MainWindow::volumeSliderMoved);
    toolBar->addWidget(volumeSlider);

    QRect availableGeometry = QApplication::primaryScreen()->availableGeometry();
    int availableWidth = availableGeometry.width();
    int volumeSliderWidth = availableWidth / 10;
    volumeSlider->setFixedWidth(volumeSliderWidth);

    toolBar->addSeparator();

    convertToVideoCheckbox = new QCheckBox("mp4", this);
    convertToVideoCheckbox->setChecked(true);
    toolBar->addWidget(convertToVideoCheckbox);

    convertToGifCheckbox = new QCheckBox("gif", this);
    convertToGifCheckbox->setChecked(true);
    toolBar->addWidget(convertToGifCheckbox);
}

void MainWindow::open()
{
    QFileDialog file_dialog(this);
    QString movies_location = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

    file_dialog.setDirectory(movies_location);
    if (file_dialog.exec() == QDialog::Accepted)
    {
        state = VIDEO_STATE;

        QUrl url = file_dialog.selectedUrls().at(0);
        videoPath = QDir::toNativeSeparators(url.toLocalFile());

        player->setSource(url);
        player->play();

        playbackSlider->setMinimum(0);
        playbackSlider->setMaximum(player->duration());
        connect(playbackSlider, &QSlider::valueChanged, this, &MainWindow::playbackSliderMoved);
        playbackSlider->setSliderPosition(0);

        startPositionSlider->setMinimum(0);
        startPositionSlider->setMaximum(player->duration());
        startPositionSlider->setSliderPosition(0);

        endPositionSlider->setMinimum(startPositionSlider->minimum());
        endPositionSlider->setMaximum(startPositionSlider->maximum());
        endPositionSlider->setSliderPosition(endPositionSlider->maximum());
        endPositionSlider->setSliderPosition(endPositionSlider->maximum());

        qDebug() << player->duration();
    }
}

void MainWindow::ripButtonClicked()
{
    if (state == VIDEO_STATE && !convertToVideoCheckbox->isChecked())
    {
        state = GIF_STATE;
    }

    rip();
}

void MainWindow::volumeSliderMoved(qint64 position)
{
    auto volume = static_cast<float>(position) / static_cast<float>(this->volumeSlider->maximum());
    qDebug() << position;
    qDebug() << volume;
    this->audioOutput->setVolume(volume);
}

void MainWindow::rip()
{
    int startPosition = startPositionSlider->sliderPosition();
    int endPosition = endPositionSlider->sliderPosition();

    if (endPosition < startPosition)
    {
        showAlert("ERROR!", "You can't rip upside down! End position must be greater than start position!");
        return;
    }

    QString outputVideoPath;
    if (state == VIDEO_STATE)
    {
        outputVideoPath = videoPath + "_output.mp4";
    }
    else if (state == GIF_STATE)
    {
        outputVideoPath = videoPath + "_output.gif";
    }
    else
    {
        showAlert("WUT!", "UNKNOWN STATE!");
        qDebug() << "WUT! UNKNOWN STATE!!";
        return;
    }

    QString commandLine = QString("%1 -ss %2ms -to %3ms -y -i \"%4\" \"%5\"")
                              .arg(ffmpegPath)
                              .arg(startPosition)
                              .arg(endPosition)
                              .arg(videoPath)
                              .arg(outputVideoPath);

    qDebug() << commandLine;

    process = new QProcess();
    process->setProcessChannelMode(QProcess::MergedChannels);
    connect(process, &QProcess::started, this, &MainWindow::processStarted);
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::processReadyReadStandardOutput);
    connect(process, &QProcess::stateChanged, this, &MainWindow::processStateChanged);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::processFinished);
    process->start(commandLine);
}


void MainWindow::processStarted()
{
    qDebug("Process Started");
    showAlert("Started!", QString("Rippin %1!!").arg(state));
}

void MainWindow::processReadyReadStandardOutput()
{
    QByteArray outputData = process->readAllStandardOutput();
    qDebug() << "Standard Output: " << QString(outputData);
}

void MainWindow::processStateChanged()
{
    qDebug() << "State changed";

    QByteArray outputData = process->readAllStandardOutput();
    QByteArray errorData = process->readAllStandardError();

    qDebug() << "Standard Output: " << QString(outputData);
    qDebug() << "Standard Error: " << QString(errorData);
}

void MainWindow::showAlert(const QString &title, const QString &message)
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(title);
    messageBox.setText(message);
    messageBox.setIcon(QMessageBox::Information);
    messageBox.exec();
}

void MainWindow::processFinished()
{
    qDebug("Process Finished");
    QProcess::ExitStatus exitStatus = process->exitStatus();

    if (exitStatus == QProcess::NormalExit)
    {
        qDebug("SUCCESS!!!");
        showAlert("WOW!", state + " Ripped Successfully!");
        if (state == VIDEO_STATE)
        {
            if (convertToVideoCheckbox->isChecked())
            {
                state = GIF_STATE;
                rip();
            }
        }
        else if (state == GIF_STATE)
        {
            state = VIDEO_STATE;
        }
    }
    else
    {
        showAlert("Ugh!!", "Rip Failed! :-(");
        qDebug("Not normal exit: %d", static_cast<int>(exitStatus));
    }
}

void MainWindow::playbackSliderMoved(qint64 position)
{
    player->setPosition(position);
}

void MainWindow::playbackChanged(qint64 position)
{
    playbackSlider->blockSignals(true);
    playbackSlider->setSliderPosition(position);
    playbackSlider->blockSignals(false);
}

void MainWindow::ensure_stopped()
{
    if (player->playbackState() != QMediaPlayer::StoppedState)
    {
        player->stop();
    }
}

void MainWindow::update_buttons(QMediaPlayer::PlaybackState state)
{
    playAction->setEnabled(state != QMediaPlayer::PlayingState);
    pauseAction->setEnabled(state == QMediaPlayer::PlayingState);
    stopAction->setEnabled(state != QMediaPlayer::StoppedState);
}

void MainWindow::show_status_message(const QString &message)
{
    statusBar()->showMessage(message, 5000);
}

void MainWindow::handle_player_error(QMediaPlayer::Error error, const QString& errorString)
{
    Q_UNUSED(error);
    show_status_message(errorString);
}


