#pragma once

#include <QWidget>

class ProgressIndicatorWidget : public QWidget
{
    Q_OBJECT
public:
    ProgressIndicatorWidget(QWidget* parent);

    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    bool debug;
};
