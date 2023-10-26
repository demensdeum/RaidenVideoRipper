#include "editorwindow.h"
#include <QApplication>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QFileDialog>
#include <QAction>
#include <QSlider>
#include <QToolBar>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QSize>
#include <QStandardPaths>
#include <QStyle>
#include <QStatusBar>
#include <QScreen>
#include <QShortcut>
#include <QPushButton>
#include <QDial>
#include <QString>
#include <videoprocessor.h>
#include "constants.h"
#include "utils.h"

EditorWindow::EditorWindow()
{
    auto style = this->style();
    playIcon = QIcon::fromTheme(
        "media-playback-start.png",
        style->standardIcon(QStyle::SP_MediaPlay)
        );
    pauseIcon = QIcon::fromTheme(
        "media-playback-pause.png",
        style->standardIcon(QStyle::SP_MediaPause)
        );
    stopIcon = QIcon::fromTheme(
        "media-playback-stop.png",
        style->standardIcon(QStyle::SP_MediaStop)
        );
    for (int state = 0; state < State::EnumCount; state++) {
        switch (state) {
        case VIDEO_PROCESSING:
            stateToString[state] = "video";
            break;
        case GIF_PROCESSING:
            stateToString[state] = "gif";
            break;
        case EnumCount:
            break;
        }
    }
    playbackState = std::make_tuple(QMediaPlayer::StoppedState, 0);
    userForcedStop = false;
    state = IDLE;
    audioOutput = nullptr;
    createLayout();
    initializePlayer();
    setupActions();
    updateWindowTitle();
    openArgumentsFileIfNeeded();
    updateDurationLabel();
    progressBarWindow = nullptr;
    restoreWindowSize();
}

void EditorWindow::restoreWindowSize()
{
    restoreGeometry(settings.value(mainWindowGeometryKey).toByteArray());
    restoreState(settings.value(mainWindowStateKey).toByteArray());
}

void EditorWindow::openArgumentsFileIfNeeded()
{
    auto application = QCoreApplication::instance();
    auto arguments = application->arguments();
    if (arguments.length() == 2) {
        auto filepath = arguments.at(1);
        auto url = QUrl::fromLocalFile(filepath);
        handleOpenFile(url);
    }
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

    auto optionsMenu = menuBar()->addMenu("&Options");

    auto isPreviewChecked = settings.value(previewCheckboxStateKey, true).value<bool>();
    previewCheckboxAction = new QAction("Preview", this);
    previewCheckboxAction->setCheckable(true);
    previewCheckboxAction->setChecked(isPreviewChecked);
    connect(
        previewCheckboxAction,
        &QAction::triggered,
        this,
        &EditorWindow::previewCheckboxStateChange
        );
    optionsMenu->addAction(previewCheckboxAction);

    convertToVideoCheckboxAction = new QAction("Convert to Video", this);
    convertToVideoCheckboxAction->setCheckable(true);
    convertToVideoCheckboxAction->setChecked(settings.value(convertToVideoCheckboxStateKey, true).value<bool>());
    connect(
        convertToVideoCheckboxAction,
        &QAction::triggered,
        this,
        &EditorWindow::checkboxVideoStateChanged
        );
    optionsMenu->addAction(convertToVideoCheckboxAction);

    convertToGifCheckboxAction = new QAction("Convert to Gif", this);
    convertToGifCheckboxAction->setCheckable(true);
    convertToGifCheckboxAction->setChecked(settings.value(convertToGifCheckboxStateKey, true).value<bool>());
    connect(
        convertToVideoCheckboxAction,
        &QAction::triggered,
        this,
        &EditorWindow::checkboxGifStateChanged
        );
    optionsMenu->addAction(convertToGifCheckboxAction);

    auto aboutMenu = menuBar()->addMenu("&About");

    auto aboutApplicationAction = new QAction("About Raiden Video Ripper", this);
    connect(aboutApplicationAction, &QAction::triggered, qApp, [this] { showAboutApplication(); });
    aboutMenu->addAction(aboutApplicationAction);

    auto aboutQtAction = new QAction("About &Qt", this);
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
    aboutMenu->addAction(aboutQtAction);
}

void EditorWindow::updateDurationLabel()
{
    auto format = QString("hh:mm:ss.zzz");
    auto start = timelineIndicator->getStartValue();
    auto playback = timelineIndicator->getPlaybackValue();
    auto end = timelineIndicator->getEndValue();

    auto startTime = QTime(0, 0);
    startTime = startTime.addMSecs(start);
    auto startFormattedTime = startTime.toString(format);

    auto endTime = QTime(0, 0);
    endTime = endTime.addMSecs(end);
    auto endFormattedTime = endTime.toString(format);

    auto playbackTime = QTime(0, 0);
    playbackTime = playbackTime.addMSecs(playback);
    auto durationFormattedTime = playbackTime.toString(format);

    auto text = QString("%1 - %2 - %3")
                    .arg(
                        startFormattedTime,
                        durationFormattedTime,
                        endFormattedTime
                        );

    durationLabel->setText(text);
}

void EditorWindow::cancelInProgess()
{
    qDebug() << "cancel in progress";
    progressBarWindow.value()->close();
    videoProcessor.value()->cancel();
    videoProcessorProgressPoller.value()->stop();
}

void EditorWindow::cleanupBeforeExit()
{
    settings.setValue(mainWindowGeometryKey, saveGeometry());
    settings.setValue(mainWindowStateKey, saveState());

    switch (state) {
    case IDLE:
        break;
    case VIDEO_PROCESSING:
    case GIF_PROCESSING:
        cancelInProgess();
        break;
    case EnumCount:
        break;
    }
}

void EditorWindow::closeEvent(QCloseEvent *event)
{
    cleanupBeforeExit();
    QMainWindow::closeEvent(event);
}

void EditorWindow::createLayout()
{
    auto bottomPrimaryHorizontalPanel = new QWidget();
    bottomPrimaryHorizontalPanel->setFixedHeight(primaryPanelHeight);

    playbackButton = new QPushButton();
    playbackButton->setFixedWidth(40);
    playbackButton->setIcon(playIcon);
    connect(playbackButton, &QPushButton::clicked, this, &EditorWindow::playToggleButtonClicked);

    stopButton = new QPushButton();
    stopButton->setFixedWidth(40);
    stopButton->setIcon(stopIcon);
    connect(stopButton, &QPushButton::clicked, this, &EditorWindow::stopButtonClicked);

    auto bottomPrimaryHorizontalPanelLayout = new QHBoxLayout(bottomPrimaryHorizontalPanel);
    bottomPrimaryHorizontalPanelLayout->setContentsMargins(0, 0, 0, 0);
    bottomPrimaryHorizontalPanelLayout->addWidget(playbackButton);
    bottomPrimaryHorizontalPanelLayout->addWidget(stopButton);

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setMinimum(0);
    volumeSlider->setMaximum(100);
    volumeSlider->setToolTip("Volume");
    volumeSlider->setFixedWidth(80);
    auto savedVolume = settings.value(volumeSettingsKey, volumeSlider->maximum()).value<qint64>();
    volumeSlider->setValue(savedVolume);
    connect(volumeSlider, &QSlider::valueChanged, this, &EditorWindow::volumeChanged);
    bottomPrimaryHorizontalPanelLayout->addWidget(volumeSlider);
    this->volumeChanged(savedVolume);

    auto bottomSecondaryPanel = new QWidget();
    bottomSecondaryPanel->setFixedHeight(secondaryPanelHeight);

    auto startButton = new QPushButton("START");
    startButton->setFixedWidth(160);
    startButton->setFixedHeight(30);
    startButton->setStyleSheet("QPushButton {"
                               "color: white;"
                               "background-color: #007ad9;"
                               "border: none;"
                               "font-weight: bold;"
                               "}");
    connect(startButton, &QPushButton::clicked, this, &EditorWindow::startButtonClicked);

    durationLabel = new QLabel("00:00:00.00 - 00:00:00.00 - 00:00:00.00");
    durationLabel->setAlignment(Qt::AlignCenter);

    auto emptySpace = new QWidget();
    emptySpace->setFixedWidth(1);

    auto leftEmptySpace = new QWidget();
    leftEmptySpace->setFixedWidth(1);

    auto rightEmptySpace = new QWidget();
    rightEmptySpace->setFixedWidth(1);

    auto bottomSecondaryHorizontalPanelLayout = new QHBoxLayout(bottomSecondaryPanel);
    bottomSecondaryHorizontalPanelLayout->setContentsMargins(0, 0, 0, 0);
    bottomSecondaryHorizontalPanelLayout->addWidget(leftEmptySpace);
    bottomSecondaryHorizontalPanelLayout->addWidget(playbackButton);
    bottomSecondaryHorizontalPanelLayout->addWidget(stopButton);
    bottomSecondaryHorizontalPanelLayout->addWidget(emptySpace);
    bottomSecondaryHorizontalPanelLayout->addWidget(volumeSlider);
    bottomSecondaryHorizontalPanelLayout->addWidget(durationLabel);
    bottomSecondaryHorizontalPanelLayout->addWidget(startButton);
    bottomSecondaryHorizontalPanelLayout->addWidget(rightEmptySpace);

    videoWidget = new VideoWidget(this);
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
    videoWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    connect(
        videoWidget,
        &VideoWidget::dragDidDropUrl,
        this,
        &EditorWindow::handleDropUrl
        );

    auto layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(videoWidget);

    timelineIndicator = new TimelineWidget(this, 100);

    connect(
        timelineIndicator,
        &TimelineWidget::startSliderDraggingStarted,
        this,
        &EditorWindow::startSliderDraggingStarted
        );

    connect(
        timelineIndicator,
        &TimelineWidget::startSliderDraggingFinished,
        this,
        &EditorWindow::startSliderDraggingFinished
        );

    connect(
        timelineIndicator,
        &TimelineWidget::playbackSliderDraggingStarted,
        this,
        &EditorWindow::playbackSliderDraggingStarted
        );

    connect(
        timelineIndicator,
        &TimelineWidget::playbackSliderDraggingFinished,
        this,
        &EditorWindow::playbackSliderDraggingFinished
        );

    connect(
        timelineIndicator,
        &TimelineWidget::endSliderDraggingStarted,
        this,
        &EditorWindow::endSliderDraggingStarted
        );

    connect(
        timelineIndicator,
        &TimelineWidget::endSliderDraggingFinished,
        this,
        &EditorWindow::endSliderDraggingFinished
        );

    connect(
        timelineIndicator,
        &TimelineWidget::startValueChanged,
        this,
        &EditorWindow::startPositionSliderMoved
        );

    connect(
        timelineIndicator,
        &TimelineWidget::playbackValueChanged,
        this,
        &EditorWindow::playbackSliderMoved
        );

    connect(
        timelineIndicator,
        &TimelineWidget::endValueChanged,
        this,
        &EditorWindow::endPositionSliderMoved
        );

    auto isPreviewChecked = settings.value(previewCheckboxStateKey, true).value<bool>();
    timelineIndicator->setFreeplayMode(!isPreviewChecked);

    auto horizontalIndicators = new QWidget();
    auto horizontalIndicatorsLayout = new QHBoxLayout(horizontalIndicators);
    horizontalIndicatorsLayout->setContentsMargins(0, 0, 0, 0);
    horizontalIndicatorsLayout->addWidget(timelineIndicator);
    layout->addWidget(horizontalIndicators);
    layout->addWidget(bottomSecondaryPanel);

    auto widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);

    QRect availableGeometry = QApplication::primaryScreen()->availableGeometry();
    auto width = availableGeometry.width() * 0.7038043478260869;
    auto height = width * 0.5625;

    resize(width, height);
}

void EditorWindow::playbackSliderDraggingStarted()
{
    auto state = player->playbackState();
    // TODO: switch
    if (
        state == QMediaPlayer::PlayingState
        ||
        state == QMediaPlayer::PausedState
        ) {
        playbackState = std::make_tuple(player->playbackState(), player->position());
        player->pause();
    }
}

void EditorWindow::playbackSliderDraggingFinished()
{
    auto state = std::get<0>(playbackState);

    switch (state) {
    case QMediaPlayer::PlayingState:
        player->play();
        break;
    case QMediaPlayer::PausedState:
        player->pause();
        break;
    default:
        break;
    }

    playbackState = std::make_tuple(QMediaPlayer::StoppedState, 0);
}

void EditorWindow::handleDropUrl(QUrl url)
{
    if (state != IDLE) {
        return;
    }
    handleOpenFile(url);
}

void EditorWindow::startPositionSliderMoved(qint64 position) {
    if (!previewCheckboxAction->isChecked()) return;
    player->setPosition(position);
    this->updateDurationLabel();
}

void EditorWindow::endPositionSliderMoved(qint64 position) {
    if (!previewCheckboxAction->isChecked()) return;
    player->setPosition(position);
    this->updateDurationLabel();
}

void EditorWindow::initializePlayer()
{
    audioOutput = new QAudioOutput();
    player = new QMediaPlayer();
    player->setAudioOutput(audioOutput);

    connect(
        player,
        &QMediaPlayer::positionChanged,
        this,
        &EditorWindow::playbackChanged
    );
    connect(
        player,
        &QMediaPlayer::playbackStateChanged,
        this,
        &EditorWindow::playbackStateChanged
    );
    connect(
        player,
        &QMediaPlayer::errorOccurred,
        this,
        &EditorWindow::handlePlayerError
    );
    player->setVideoOutput(videoWidget);
}

void EditorWindow::playbackStateChanged(QMediaPlayer::PlaybackState state)
{
    updateButtons(state);

    if (previewCheckboxAction->isChecked()) {
        if (state == QMediaPlayer::StoppedState && userForcedStop == false) {
            player->play();
        }
    }
}

void EditorWindow::handleLeftKeyPress()
{
    timelineIndicator->moveLeft();
}

void EditorWindow::handleRightKeyPress()
{
    timelineIndicator->moveRight();
}

void EditorWindow::checkboxVideoStateChanged(bool isChecked)
{
    settings.setValue(convertToVideoCheckboxStateKey, isChecked);
}

void EditorWindow::checkboxGifStateChanged(bool isChecked)
{
    settings.setValue(convertToGifCheckboxStateKey, isChecked);
}

void EditorWindow::previewCheckboxStateChange(bool isChecked)
{
    timelineIndicator->setFreeplayMode(!isChecked);
    settings.setValue(previewCheckboxStateKey, isChecked);
    update();
}

void EditorWindow::playToggleButtonClicked() {
    userForcedStop = false;
    togglePlayback();
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
            "<li><a href=\"https://github.com/demensdeum/Dullahan-FFmpeg/\">Dullahan-FFmpeg</a> FFmpeg CLI as shared library </li>"
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

void EditorWindow::savePlaybackState()
{
    auto state = player->playbackState();
    switch (state) {
    case QMediaPlayer::PlayingState:
    case QMediaPlayer::PausedState:
        playbackState = std::make_tuple(player->playbackState(), player->position());
        player->pause();
        break;
    default:
        break;
    }
}

void EditorWindow::restorePlaybackState()
{
    auto state = std::get<0>(playbackState);
    auto position = std::get<1>(playbackState);

    if (
        position >= timelineIndicator->getStartValue()
        &&
        position <= timelineIndicator->getEndValue()
        )
    {
        player->setPosition(position);
    }
    else {
        player->setPosition(timelineIndicator->getStartValue());
    }
    switch (state) {
    case QMediaPlayer::PlayingState:
        player->play();
        break;
    case QMediaPlayer::PausedState:
        player->pause();
        break;
    default:
        break;
    }

    playbackState = std::make_tuple(QMediaPlayer::StoppedState, 0);
}

void EditorWindow::startSliderDraggingStarted()
{
    savePlaybackState();
}

void EditorWindow::startSliderDraggingFinished()
{
    restorePlaybackState();
}

void EditorWindow::endSliderDraggingStarted()
{
    savePlaybackState();
}

void EditorWindow::endSliderDraggingFinished()
{
    restorePlaybackState();
}

void EditorWindow::open()
{
    QFileDialog fileDialog(this);

    QString moviesLocation = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    auto path = settings.value(lastWorkingPathKey, moviesLocation).value<QString>();

    fileDialog.setDirectory(path);

    if (fileDialog.exec() == QDialog::Accepted)
    {
        QUrl url = fileDialog.selectedUrls().at(0);
        handleOpenFile(url);
    }
}

void EditorWindow::handleOpenFile(QUrl url)
{
    auto incomingFilepath = QDir::toNativeSeparators(url.toLocalFile());

    if (incomingFilepath == videoPath) {
        qDebug() << "User trying to open same file, are they nuts?";
        return;
    }
    videoPath = incomingFilepath;

    switch (state) {
    case IDLE:
        break;
    case VIDEO_PROCESSING:
    case GIF_PROCESSING:
        cancelInProgess();
    case EnumCount:
        break;
    }

    if (state != IDLE) {
        return;
    }

    auto videoPathDirectory = QFileInfo(videoPath).absolutePath();
    settings.setValue(lastWorkingPathKey, videoPathDirectory);
    player->setSource(url);
    player->play();
    volumeChanged(volumeSlider->value());
    timelineIndicator->setMaximumValue(player->duration());
    timelineIndicator->setStartValue(0);
    timelineIndicator->setPlaybackValue(0);
    timelineIndicator->setEndValue(player->duration());
    updateWindowTitle();
}

void EditorWindow::updateWindowTitle()
{
    auto applicationTitle = QString(applicationName) + " " + QString(applicationVersion);
    auto title = applicationTitle;
    if (!videoPath.isEmpty()) {
        title = QFileInfo(videoPath).fileName() + " - " + applicationTitle;
    }
    this->setWindowTitle(title);
}

void EditorWindow::startButtonClicked()
{
    switch (state) {
    case IDLE:
        if (videoPath.isEmpty()) {
            showAlert("WUT!", "Open file first!");
            return;
        }
        else if (
            !convertToVideoCheckboxAction->isChecked() &&
            !convertToGifCheckboxAction->isChecked()
            ) {
            showAlert("WUT!", "Select video/gif checkboxes first!");
            return;
        }
        if (convertToVideoCheckboxAction->isChecked()) {
            state = VIDEO_PROCESSING;
        }
        else if (convertToGifCheckboxAction->isChecked()) {
            state = GIF_PROCESSING;
        }
        break;

    case VIDEO_PROCESSING:
        if (convertToGifCheckboxAction->isChecked()) {
            state = GIF_PROCESSING;
        }
        break;

    case GIF_PROCESSING:
        break;

    case EnumCount:
        break;
    }

    cut();
}

void EditorWindow::volumeChanged(qint64 position)
{
    auto volume = static_cast<float>(position) / static_cast<float>(volumeSlider->maximum());
    if (audioOutput) {
        audioOutput->setVolume(volume);
    }
    settings.setValue(volumeSettingsKey, position);
}

void EditorWindow::showProgressbarWindow(QString text)
{
    progressBarWindow = new ProgressBarWindow(text);
    connect(
        progressBarWindow.value(),
        &ProgressBarWindow::cancelButtonPressed,
        this,
        &EditorWindow::cancelInProgess
        );
    progressBarWindow.value()->show();

    if (videoProcessorProgressPoller.has_value()) {
        videoProcessorProgressPoller.value()->stop();
    }
    videoProcessorProgressPoller = new VideoProcessorProgressPoller(player->duration());
    connect(
        videoProcessorProgressPoller.value(),
        &VideoProcessorProgressPoller::didPollProgress,
        this,
        &EditorWindow::didPollProgress
    );
}

void EditorWindow::cut()
{
    int startPosition = timelineIndicator->getStartValue();
    int endPosition = timelineIndicator->getEndValue();

    if (endPosition < startPosition)
    {
        showAlert("ERROR!", "You can't cut upside down! End position must be greater than start position!");
        return;
    }

    QString outputVideoPath;
    switch (state)
    {
    case IDLE:
        showAlert("Ugh!", "Internal error, IDLE state!");
        return;

    case VIDEO_PROCESSING:
        outputVideoPath = videoPath + "_output.mp4";
        break;

    case GIF_PROCESSING:
        outputVideoPath = videoPath + "_output.gif";
        break;

    case EnumCount:
        break;
    }

    auto stateString = RaidenVideoRipper::Utils::capitalized(stateToString[state]);
    QString text = "Cutting " + stateString + "...";
    showProgressbarWindow(text);

    videoProcessor = new VideoProcessor(startPosition, endPosition, videoPath, outputVideoPath);
    videoProcessor.value()->setAutoDelete(true);

    connect(
        videoProcessor.value(),
        &VideoProcessor::videoProcessingDidFinish,
        this,
        &EditorWindow::convertingDidFinish
        );
    threadPool.start(videoProcessor.value());
}

void EditorWindow::didPollProgress(int progress)
{
    progressBarWindow.value()->setProgress(progress);
    qDebug() << "didPollProgess: " << progress;
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
    videoProcessorProgressPoller.value()->stop();
    progressBarWindow.value()->close();

    qDebug("Process Finished");

    auto isSuccess = result == 0;

    if (isSuccess)
    {
        qDebug("SUCCESS!!!");
        auto stateString = RaidenVideoRipper::Utils::capitalized(stateToString[state]);
        showAlert("WOW!", stateString + " Cutted Successfully!");
        if (state == VIDEO_PROCESSING)
        {
            if (convertToGifCheckboxAction->isChecked())
            {
                state = GIF_PROCESSING;
                cut();
            }
            else {
                state = IDLE;
            }
        }
        else if (state == GIF_PROCESSING)
        {
            state = IDLE;
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
    this->updateDurationLabel();
}

void EditorWindow::playbackChanged(qint64 position)
{
    auto sliderUpdate = [this] (int position) {
        timelineIndicator->blockSignals(true);
        timelineIndicator->setPlaybackValue(position);
        timelineIndicator->blockSignals(false);
    };

    if (player->isPlaying() && this->previewCheckboxAction->isChecked()) {
        auto startPosition = timelineIndicator->getStartValue();
        auto endPosition = timelineIndicator->getEndValue();
        if (position > endPosition) {
            player->setPosition(startPosition);
        }
        else if (position < startPosition) {
            player->setPosition(startPosition);
        }
    }
    sliderUpdate(player->position());
    updateDurationLabel();
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
    if (player->isPlaying()) {
        playbackButton->setIcon(pauseIcon);
    }
    else {
        playbackButton->setIcon(playIcon);
    }
    stopButton->setEnabled(state != QMediaPlayer::StoppedState);
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
