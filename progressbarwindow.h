#pragma once

#include <QWidget>
#include <QProgressBar>

class ProgressBarWindow : public QWidget
{
    Q_OBJECT

public:
    ProgressBarWindow(QString text = "Processing...");

    void setMaxValue(int maxValue);
    void setProgress(int value);

signals:
    void cancelButtonPressed();

private:
    QProgressBar progressBar;
    int maxValue = 100;
};
