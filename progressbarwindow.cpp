#include "progressbarwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QApplication>
#include <QScreen>

ProgressBarWindow::ProgressBarWindow(QString text)
    : QWidget()
{
    setWindowTitle(text);
    auto availableGeometry = QApplication::primaryScreen()->availableGeometry();
    auto width = availableGeometry.width() * 0.2604166666666667;
    auto height = width * 0.25;
    setFixedSize(width, height);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowFlag(Qt::WindowMinimizeButtonHint, false);

    auto mainLayout = new QVBoxLayout(this);

    progressBar.setValue(0);
    mainLayout->addWidget(&progressBar);

    QPushButton* cancelButton = new QPushButton(tr("&Cancel"));
    mainLayout->addWidget(cancelButton);

    connect(
        cancelButton,
        &QPushButton::clicked,
        this,
        &ProgressBarWindow::cancelButtonPressed
        );

    setLayout(mainLayout);
}

void ProgressBarWindow::setMaxValue(int maxValue)
{
    this->maxValue = maxValue;
    progressBar.setMaximum(maxValue);
}

void ProgressBarWindow::setProgress(int value)
{
    progressBar.setValue(value);
}
