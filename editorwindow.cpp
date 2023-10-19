#include "editorwindow.h"
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
#include <QShortcut>
#include <videoprocessor.h>

#include "constants.h"

EditorWindow::EditorWindow()
{
    for (int state = 0; state < State::EnumCount; state++) {
        switch (state) {
        case VIDEO:
            stateToString[state] = "VIDEO";
            break;
        case GIF:
            stateToString[state] = "GIF";
            break;
        case EnumCount:
            break;
        }
    }
    userForcedStop = false;
    state = VIDEO;
    createUI();
    createLayout();
    initializePlayer();
    setupActions();
    setupToolBar();
}

void EditorWindow::createUI()
{
    setWindowTitle("Raiden Video Ripper " + QString(applicationVersion));
}

void EditorWindow::setupActions()
{
    openAction = new QAction("Open...", this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &EditorWindow::open);

    auto exitAction = new QAction("Exit", this);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    auto fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(openAction);
    fileMenu->addAction(exitAction);
}

void EditorWindow::createLayout()
{
    videoWidget = new QVideoWidget();
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
    videoWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    auto layout = new QVBoxLayout();
    layout->addWidget(videoWidget);

    workspaceIndicator = new WorkspaceIndicator(this, 100);
    layout->addWidget(workspaceIndicator);

    connect(workspaceIndicator, &WorkspaceIndicator::startValueChanged, this, &EditorWindow::startPositionSliderMoved);
    connect(workspaceIndicator, &WorkspaceIndicator::playbackValueChanged, this, &EditorWindow::playbackSliderMoved);
    connect(workspaceIndicator, &WorkspaceIndicator::endValueChanged, this, &EditorWindow::endPositionSliderMoved);

    auto cutButton = new QPushButton("Cut");
    connect(cutButton, &QPushButton::clicked, this, &EditorWindow::cutButtonClicked);
    layout->addWidget(cutButton);

    auto widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);

    auto availableGeometry = QApplication::primaryScreen()->availableGeometry();
    auto width = availableGeometry.width() * 0.4;
    auto height = width * 0.8;

    resize(width, height);
}

void EditorWindow::startPositionSliderMoved(qint64 position) {
    if (!previewCheckbox->isChecked()) return;
    player->setPosition(position);
}

void EditorWindow::endPositionSliderMoved(qint64 position) {
    if (!previewCheckbox->isChecked()) return;
    player->setPosition(position);
}

void EditorWindow::initializePlayer()
{
    audioOutput = new QAudioOutput();
    player = new QMediaPlayer();
    player->setAudioOutput(audioOutput);

    connect(player, &QMediaPlayer::positionChanged, this, &EditorWindow::playbackChanged);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &EditorWindow::playbackStateChanged);
    connect(player, &QMediaPlayer::errorOccurred, this, &EditorWindow::handlePlayerError);
    player->setVideoOutput(videoWidget);
}

void EditorWindow::playbackStateChanged(QMediaPlayer::PlaybackState state)
{
    updateButtons(state);

    if (previewCheckbox->isChecked()) {
        if (state == QMediaPlayer::StoppedState && userForcedStop == false) {
            player->play();
        }
    }
}

void EditorWindow::setupToolBar()
{
    toolBar = new QToolBar();
    addToolBar(toolBar);

    toolBar->addAction(openAction);

    auto playMenu = menuBar()->addMenu("&Play");
    auto style = this->style();

    playAction = new QAction("Play", this);
    QIcon playIcon = QIcon::fromTheme("media-playback-start.png", style->standardIcon(QStyle::SP_MediaPlay));
    playAction->setIcon(playIcon);
    connect(playAction, &QAction::triggered, this, &EditorWindow::playButtonClicked);
    toolBar->addAction(playAction);
    playMenu->addAction(playAction);

    auto aboutMenu = menuBar()->addMenu("&About");

    auto aboutApplicationAction = new QAction("About Raiden Video Ripper", this);
    connect(aboutApplicationAction, &QAction::triggered, qApp, [this] { showAboutApplication(); });
    aboutMenu->addAction(aboutApplicationAction);

    auto aboutQtAction = new QAction("About &Qt", this);
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
    aboutMenu->addAction(aboutQtAction);

    pauseAction = new QAction("Pause", this);
    QIcon pauseIcon = QIcon::fromTheme("media-playback-pause.png", style->standardIcon(QStyle::SP_MediaPause));
    pauseAction->setIcon(pauseIcon);
    connect(pauseAction, &QAction::triggered, player, &QMediaPlayer::pause);
    toolBar->addAction(pauseAction);
    playMenu->addAction(pauseAction);

    stopAction = new QAction("Stop", this);
    QIcon stopIcon = QIcon::fromTheme("media-playback-stop.png", style->standardIcon(QStyle::SP_MediaStop));
    stopAction->setIcon(stopIcon);
    connect(stopAction, &QAction::triggered, this, &EditorWindow::stopButtonClicked);
    toolBar->addAction(stopAction);
    playMenu->addAction(stopAction);

    QKeySequence keySequence(Qt::Key_Space);
    QShortcut *shortcut = new QShortcut(keySequence, this);
    shortcut->setEnabled(true);
    QObject::connect(shortcut, &QShortcut::activated, this, &EditorWindow::togglePlayback);

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setMinimum(0);
    volumeSlider->setMaximum(100);
    volumeSlider->setTickInterval(10);
    volumeSlider->setTickPosition(QSlider::TicksBelow);
    volumeSlider->setToolTip("Volume");
    auto savedVolume = settings.value(volumeSettingsKey, volumeSlider->maximum()).value<qint64>();
    volumeSlider->setValue(savedVolume);
    connect(volumeSlider, &QSlider::valueChanged, this, &EditorWindow::volumeChanged);
    toolBar->addWidget(volumeSlider);
    this->volumeChanged(savedVolume);

    QRect availableGeometry = QApplication::primaryScreen()->availableGeometry();
    int availableWidth = availableGeometry.width();
    int volumeSliderWidth = availableWidth / 10;
    volumeSlider->setFixedWidth(volumeSliderWidth);

    toolBar->addSeparator();

    previewCheckbox = new QCheckBox("Preview", this);
    previewCheckbox->setChecked(true);
    connect(previewCheckbox, &QCheckBox::stateChanged, this, &EditorWindow::previewCheckboxStateChange);
    toolBar->addWidget(previewCheckbox);

    convertToVideoCheckbox = new QCheckBox("mp4", this);
    convertToVideoCheckbox->setChecked(true);
    toolBar->addWidget(convertToVideoCheckbox);

    convertToGifCheckbox = new QCheckBox("gif", this);
    convertToGifCheckbox->setChecked(true);
    toolBar->addWidget(convertToGifCheckbox);
}

void EditorWindow::previewCheckboxStateChange(int _state) {
    Qt::CheckState state = (Qt::CheckState)_state;
    switch (state) {
    case Qt::Unchecked:
        workspaceIndicator->setFreeplayMode(true);
        update();
        break;
    case Qt::Checked:
        workspaceIndicator->setFreeplayMode(false);
        update();
        break;
    default:
        break;
    }

    qDebug() << "previewChecboxStateChange" << state;
}

void EditorWindow::playButtonClicked() {
    userForcedStop = false;
    player->play();
}

void EditorWindow::stopButtonClicked() {
    userForcedStop = true;
    player->stop();
}

void EditorWindow::togglePlayback() {
    userForcedStop = false;
    if (player->playbackState() == QMediaPlayer::PlayingState) {
        player->pause();
    }
    else {
        player->play();
    }
}

void EditorWindow::showAboutApplication()
{
    const auto copyright =
        tr("Copyright &copy; 2023 <a href=\"https://www.demensdeum.com/\">Ilia Prokhorov (%1)</a>")
            .arg(applicationName);
    const auto license =
        QStringLiteral("<a href=\"https://opensource.org/license/mit/\">MIT License</a>");
    const auto sourceCode =
        QStringLiteral("<a href=\"https://github.com/demensdeum/RaidenVideoRipper/\">https://github.com/demensdeum/RaidenVideoRipper</a>");

    QMessageBox::about(
        this,
        tr("About %1").arg(qApp->applicationName()),
        tr(
            "<h1>Version %1 %2</h1>"
            "<p><a href=\"%3\">%1</a> is an open-source project designed for video editing and format conversion. It's built using Qt 6 (Qt Creator) and allows you to trim and convert videos to MP4 or GIF formats.</p>"
            "<small><p>%4</p>"
            "<p>Licensed under the %5</p>"
            "<p>This program proudly uses the following projects:<ul>"
            "<li><a href=\"https://www.qt.io/\">Qt</a> application and UI framework</li>"
            "<li><a href=\"https://www.ffmpeg.org/\">FFmpeg</a> multimedia format and codec libraries</li>"
            "<li><a href=\"https://github.com/demensdeum/Headless-FFmpeg/\">Headless-FFmpeg</a> FFmpeg CLI as library </li>"
            "</ul></p>"
            "<p>The source code used to build this program can be downloaded from "
            "%6</p>"
            "This program is distributed in the hope that it will be useful, "
            "but WITHOUT ANY WARRANTY; without even the implied warranty of "
            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.</small>"
            )
            .arg(
                qApp->applicationName(),
                qApp->applicationVersion(),
                "https://github.com/demensdeum/RaidenVideoRipper",
                copyright,
                license,
                sourceCode
                )
        );
}

void EditorWindow::open()
{
    QFileDialog fileDialog(this);

    QString moviesLocation = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    auto path = settings.value(lastWorkingPathKey, moviesLocation).value<QString>();

    fileDialog.setDirectory(path);

    if (fileDialog.exec() == QDialog::Accepted)
    {
        state = VIDEO;

        QUrl url = fileDialog.selectedUrls().at(0);
        videoPath = QDir::toNativeSeparators(url.toLocalFile());
        auto videoPathDirectory = QFileInfo(videoPath).absolutePath();

        settings.setValue(lastWorkingPathKey, videoPathDirectory);

        player->setSource(url);
        player->play();

        workspaceIndicator->setMaximumValue(player->duration());

        workspaceIndicator->setStartValue(0);
        workspaceIndicator->setPlaybackValue(0);
        workspaceIndicator->setEndValue(player->duration());
    }
}

void EditorWindow::cutButtonClicked()
{
    if (state == VIDEO && !convertToVideoCheckbox->isChecked())
    {
        state = GIF;
    }

    cut();
}

void EditorWindow::volumeChanged(qint64 position)
{
    auto volume = static_cast<float>(position) / static_cast<float>(volumeSlider->maximum());
    qDebug() << position;
    qDebug() << volume;
    audioOutput->setVolume(volume);
    settings.setValue(volumeSettingsKey, position);
}

void EditorWindow::cut()
{
    int startPosition = workspaceIndicator->getStartValue();
    int endPosition = workspaceIndicator->getEndValue();

    if (endPosition < startPosition)
    {
        showAlert("ERROR!", "You can't rip upside down! End position must be greater than start position!");
        return;
    }

    QString outputVideoPath;
    if (state == VIDEO)
    {
        outputVideoPath = videoPath + "_output.mp4";
    }
    else if (state == GIF)
    {
        outputVideoPath = videoPath + "_output.gif";
    }
    else
    {
        showAlert("WUT!", "UNKNOWN STATE!");
        qDebug() << "WUT! UNKNOWN STATE!!";
        return;
    }

    QString text = "Rippin " + stateToString[state];
    showAlert("WOW!", text);

    auto videoProcessor = new VideoProcessor(startPosition, endPosition, videoPath, outputVideoPath);
    videoProcessor->setAutoDelete(true);

    connect(
        videoProcessor,
        &VideoProcessor::videoProcessingDidFinish,
        this,
        &EditorWindow::convertingDidFinish
        );
    threadPool.start(videoProcessor);
}

void EditorWindow::showAlert(const QString &title, const QString &message)
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(title);
    messageBox.setText(message);
    messageBox.setIcon(QMessageBox::Information);
    messageBox.exec();
}

void EditorWindow::convertingDidFinish(bool result)
{
    qDebug("Process Finished");

    auto isSuccess = result == 0;

    if (isSuccess)
    {
        qDebug("SUCCESS!!!");
        showAlert("WOW!", stateToString[state] + " Ripped Successfully!");
        if (state == VIDEO)
        {
            if (convertToVideoCheckbox->isChecked())
            {
                state = GIF;
                cut();
            }
        }
        else if (state == GIF)
        {
            state = VIDEO;
        }
    }
    else
    {
        showAlert("Ugh!!", "Cut Failed! :-(");
        qDebug("Not normal FFmpeg-Headless exit: %d", result);
    }
}

void EditorWindow::playbackSliderMoved(qint64 position)
{
    player->setPosition(position);
}

void EditorWindow::playbackChanged(qint64 position)
{
    auto sliderUpdate = [this] (int position) {
        workspaceIndicator->blockSignals(true);
        workspaceIndicator->setPlaybackValue(position);
        workspaceIndicator->blockSignals(false);
    };

    if (player->isPlaying() && this->previewCheckbox->isChecked()) {
        auto startPosition = workspaceIndicator->getStartValue();
        auto endPosition = workspaceIndicator->getEndValue();
        if (position > endPosition) {
            player->setPosition(startPosition);
        }
        else if (position < startPosition) {
            player->setPosition(startPosition);
        }
    }
    sliderUpdate(player->position());
}

void EditorWindow::ensureStopped()
{
    if (player->playbackState() != QMediaPlayer::StoppedState)
    {
        player->stop();
    }
}

void EditorWindow::updateButtons(QMediaPlayer::PlaybackState state)
{
    playAction->setEnabled(state != QMediaPlayer::PlayingState);
    pauseAction->setEnabled(state == QMediaPlayer::PlayingState);
    stopAction->setEnabled(state != QMediaPlayer::StoppedState);
}

void EditorWindow::showStatusMessage(const QString &message)
{
    statusBar()->showMessage(message, 5000);
}

void EditorWindow::handlePlayerError(QMediaPlayer::Error error, const QString &errorString)
{
    Q_UNUSED(error);
    showStatusMessage(errorString);
}
