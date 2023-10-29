#include "ProgressBarWindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpacerItem>

ProgressBarWindow::ProgressBarWindow(QString text)
    : QWidget()
{
    setWindowTitle(text);
    setFixedSize(400, 100);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowFlag(Qt::WindowMinimizeButtonHint, false);

    auto mainLayout = new QVBoxLayout(this);

    progressBar.setValue(0);
    mainLayout->addWidget(&progressBar);

    QPushButton* cancelButton = new QPushButton("&Cancel");
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
