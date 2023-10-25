#pragma once

#include <QWidget>
#include <QProgressBar>

class ProgressBarWindow : public QWidget
{
    Q_OBJECT

public:
    ProgressBarWindow(QWidget *parent = nullptr);

    void setMaxValue(int maxValue);
    void setProgress(int value);

signals:
    void cancelButtonPressed();

private:
    QProgressBar progressBar;
    int maxValue = 100;
};
