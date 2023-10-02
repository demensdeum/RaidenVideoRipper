import sys
from logging import debug as debugPrint
from os import getenv
from PySide6.QtCore import QStandardPaths, Qt, Slot, QProcess, QDir
from PySide6.QtGui import QAction, QIcon, QKeySequence
from PySide6.QtWidgets import (QApplication, QDialog, QFileDialog,
    QMainWindow, QSlider, QStyle, QToolBar, QWidget, QPushButton, QMessageBox, QVBoxLayout, QCheckBox, QSpacerItem)
from PySide6.QtMultimedia import (QAudioOutput, QMediaFormat,
                                  QMediaPlayer)
from PySide6.QtMultimediaWidgets import QVideoWidget

class MainWindow(QMainWindow):

    def __init__(self):
        super().__init__()

        self.VIDEO_STATE = "VIDEO"
        self.GIF_STATE = "GIF"

        self.state = self.VIDEO_STATE

        self.setWindowTitle("Raiden Video Ripper")

        self.audioOutput = QAudioOutput()
        self.player = QMediaPlayer()
        self.player.setAudioOutput(self.audioOutput)

        self.player.positionChanged.connect(self.playbackChanged)
        self.player.errorOccurred.connect(self.handlePlayerError)

        toolBar = QToolBar()
        self.addToolBar(toolBar)

        file_menu = self.menuBar().addMenu("&File")
        icon = QIcon.fromTheme("document-open")
        openAction = QAction(icon, "&Open...", self,
                              shortcut=QKeySequence.Open, triggered=self.open)
        file_menu.addAction(openAction)
        toolBar.addAction(openAction)
        icon = QIcon.fromTheme("application-exit")
        exit_action = QAction(icon, "E&xit", self,
                              shortcut="Ctrl+Q", triggered=self.close)
        file_menu.addAction(exit_action)

        playMenu = self.menuBar().addMenu("&Play")
        style = self.style()
        icon = QIcon.fromTheme("media-playback-start.png",
                               style.standardIcon(QStyle.SP_MediaPlay))
        self.playAction = toolBar.addAction(icon, "Play")
        self.playAction.triggered.connect(self.player.play)
        playMenu.addAction(self.playAction)

        icon = QIcon.fromTheme("media-playback-pause.png",
                               style.standardIcon(QStyle.SP_MediaPause))
        self.pauseAction = toolBar.addAction(icon, "Pause")
        self.pauseAction.triggered.connect(self.player.pause)
        playMenu.addAction(self.pauseAction)

        icon = QIcon.fromTheme("media-playback-stop.png",
                               style.standardIcon(QStyle.SP_MediaStop))
        self.stopAction = toolBar.addAction(icon, "Stop")
        self.stopAction.triggered.connect(self.ensure_stopped)
        playMenu.addAction(self.stopAction)

        self.volumeSlider = QSlider()
        self.volumeSlider.setOrientation(Qt.Horizontal)
        self.volumeSlider.setMinimum(0)
        self.volumeSlider.setMaximum(100)

        availableWidth = self.screen().availableGeometry().width()
        self.volumeSlider.setFixedWidth(availableWidth / 10)
        self.volumeSlider.setValue(self.volumeSlider.maximum())
        self.volumeSlider.setTickInterval(10)
        self.volumeSlider.setTickPosition(QSlider.TicksBelow)
        self.volumeSlider.setToolTip("Volume")
        self.volumeSlider.valueChanged.connect(self.volumeSliderMoved)
        toolBar.addWidget(self.volumeSlider)

        toolBar.addSeparator()

        self.convertToVideoCheckbox = QCheckBox('mp4', self)
        self.convertToVideoCheckbox.setChecked(True)
        toolBar.addWidget(self.convertToVideoCheckbox)

        self.convertToGifCheckbox = QCheckBox('gif', self)
        self.convertToGifCheckbox.setChecked(True)
        toolBar.addWidget(self.convertToGifCheckbox)

        about_menu = self.menuBar().addMenu("&About")
        about_qt_act = QAction("About &Qt", self, triggered=qApp.aboutQt)
        about_menu.addAction(about_qt_act)

        self.verticalLayout = QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")

        self.videoWidget = QVideoWidget()
        self.verticalLayout.addWidget(self.videoWidget)

        self.playbackSlider = QSlider()
        self.playbackSlider.setOrientation(Qt.Horizontal)
        self.playbackSlider.setMinimum(0)
        self.playbackSlider.setMaximum(100)

        self.startPositionSlider = QSlider()
        self.startPositionSlider.setOrientation(Qt.Horizontal)
        self.startPositionSlider.setMinimum(0)
        self.startPositionSlider.setMaximum(100)

        self.endPositionSlider = QSlider()
        self.endPositionSlider.setOrientation(Qt.Horizontal)
        self.endPositionSlider.setMinimum(0)
        self.endPositionSlider.setMaximum(100)
        self.endPositionSlider.setSliderPosition(100)

        self.ripButton = QPushButton("RIP EM!")
        self.ripButton.clicked.connect(self.ripButtonClicked)

        self.verticalLayout.addWidget(self.playbackSlider)
        self.verticalLayout.addWidget(self.startPositionSlider)
        self.verticalLayout.addWidget(self.endPositionSlider)
        self.verticalLayout.addWidget(self.ripButton)

        widget = QWidget()
        widget.setLayout(self.verticalLayout)
        self.setCentralWidget(widget)

        self.player.playbackStateChanged.connect(self.update_buttons)
        self.player.setVideoOutput(self.videoWidget)

        self.update_buttons(self.player.playbackState())

        self.ffmpegPath = getenv("FFMPEG_BINARY")
        debugPrint(self.ffmpegPath)
        if self.ffmpegPath == None or len(self.ffmpegPath) < 1:
            self.showAlert("WHAT!", "INSTALL FFMPEG and add FFMPEG_BINARY Environment variable to ffmpeg.exe!!!")
            debugPrint("WUT NO FFMPEG_BINARY ENVIRONMENT VARIABLE??! EXIT!")
            exit(1)
    def ripButtonClicked(self):
        if self.state == self.VIDEO_STATE and self.convertToVideoCheckbox.isChecked() == False:
            self.state = self.GIF_STATE
        self.rip()

    def volumeSliderMoved(self, position):
        volume = float(position) / float(self.volumeSlider.maximum())
        debugPrint(position)
        debugPrint(volume)
        self.audioOutput.setVolume(volume)
    def rip(self):
        startPosition = self.startPositionSlider.sliderPosition()
        endPosition = self.endPositionSlider.sliderPosition()

        if endPosition < startPosition:
            self.showAlert("ERROR!", "You can't rip upside down! End position must be greater that start position!")
            return

        if self.state == self.VIDEO_STATE:
            outputVideoPath = f"{self.videoPath}_output.mp4"
        elif self.state == self.GIF_STATE:
            outputVideoPath = f"{self.videoPath}_output.gif"
        else:
            self.showAlert("WUT!", "UNKNOWN STATE!")
            debugPrint("WUT! UNKNOWN STATE!!")
        commandLine=f"{self.ffmpegPath} -ss {startPosition}ms -to {endPosition}ms -y -i \"{self.videoPath}\" \"{outputVideoPath}\""

        debugPrint(commandLine)

        self.process = QProcess()
        self.process.setProcessChannelMode(QProcess.MergedChannels)
        self.process.started.connect(self.processStarted)
        self.process.readyReadStandardOutput.connect(self.processReadyReadStandardOutput)
        self.process.stateChanged.connect(self.processStateChanged)
        self.process.finished.connect(self.processFinished)
        self.process.start(commandLine)

    def processStarted(self):
        debugPrint("Process Started")
        self.showAlert("Started!", f"Rippin {self.state}!!")

    def processReadyReadStandardOutput(self):
        debugPrint(self.process.readAllStandardOutput())

    def processStateChanged(self):
        debugPrint("State changed")
        debugPrint(self.process.readAllStandardOutput())
        debugPrint(self.process.readAllStandardError())

    def showAlert(self, title, message):
        messageBox = QMessageBox()
        messageBox.setWindowTitle(title)
        messageBox.setText(message)
        messageBox.setIcon(QMessageBox.Icon.Information)
        messageBox.exec()

    def showPrompt(self, title, message):
        messageBox = QMessageBox()
        messageBox.setWindowTitle(title)
        messageBox.setText(message)
        messageBox.setIcon(QMessageBox.Icon.Information)
        messageBox.setStandardButtons(QMessageBox.StandardButton.Ok | QMessageBox.StandardButton.Cancel)
        if messageBox.exec() == QMessageBox.StandardButton.Ok:
            return True
        else:
            return False
    def processFinished(self):
        debugPrint("Process Finished")
        exitStatus = self.process.exitStatus()

        if exitStatus == QProcess.NormalExit:
            debugPrint("SUCCESS!!!")
            self.showAlert("WOW!", f"{self.state} Ripped Successfully!")
            if self.state == self.VIDEO_STATE:
                if self.convertToGifCheckbox.isChecked():
                    self.state = self.GIF_STATE
                    self.rip()
            elif self.state == self.GIF_STATE:
                self.state = self.VIDEO_STATE
        else:
            self.showAlert("Ugh!!", "Rip Failed! :-(")
            debugPrint(f"Not normal exit: {exitStatus}")


    def closeEvent(self, event):
        self.ensure_stopped()
        event.accept()

    @Slot()
    def open(self):
        self.ensure_stopped()

        file_dialog = QFileDialog(self)

        movies_location = QStandardPaths.writableLocation(QStandardPaths.MoviesLocation)

        file_dialog.setDirectory(movies_location)
        if file_dialog.exec() == QDialog.Accepted:
            self.state = self.VIDEO_STATE

            url = file_dialog.selectedUrls()[0]
            self.videoPath = QDir.toNativeSeparators(url.toLocalFile())

            self.player.setSource(url)
            self.player.play()

            self.playbackSlider.setMinimum(0)
            self.playbackSlider.setMaximum(self.player.duration())
            self.playbackSlider.valueChanged.connect(self.playbackSliderMoved)
            self.playbackSlider.setSliderPosition(0)

            self.startPositionSlider.setMinimum(0)
            self.startPositionSlider.setMaximum(self.player.duration())
            self.startPositionSlider.setSliderPosition(0)

            self.endPositionSlider.setMinimum(self.startPositionSlider.minimum())
            self.endPositionSlider.setMaximum(self.startPositionSlider.maximum())
            self.endPositionSlider.setSliderPosition(self.endPositionSlider.maximum())
            self.endPositionSlider.setSliderPosition(self.endPositionSlider.maximum())

            debugPrint(self.player.duration())

    def playbackSliderMoved(self, position):
        self.player.setPosition(position)

    def playbackChanged(self, position):
        self.playbackSlider.blockSignals(True)
        self.playbackSlider.setSliderPosition(position)
        self.playbackSlider.blockSignals(False)

    @Slot()
    def ensure_stopped(self):
        if self.player.playbackState() != QMediaPlayer.StoppedState:
            self.player.stop()

    @Slot("QMediaPlayer::PlaybackState")
    def update_buttons(self, state):
        self.playAction.setEnabled(state != QMediaPlayer.PlayingState)
        self.pauseAction.setEnabled(state == QMediaPlayer.PlayingState)
        self.stopAction.setEnabled(state != QMediaPlayer.StoppedState)

    def showStatusMessage(self, message):
        self.statusBar().showMessage(message, 5000)

    @Slot("QMediaPlayer::Error", str)
    def handlePlayerError(self, _, errorString):
        debugPrint(errorString, file=sys.stderr)
        self.showStatusMessage(errorString)


if __name__ == '__main__':
    application = QApplication(sys.argv)
    window = MainWindow()
    availableGeometry = window.screen().availableGeometry()
    window.resize(availableGeometry.width() * 0.8,
                  availableGeometry.height() * 0.8)
    window.show()
    sys.exit(application.exec())