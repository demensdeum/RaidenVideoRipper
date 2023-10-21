#pragma once

#include <QWidget>
#include <timelinesliderwidget.h>

class TimelineWidget : public QWidget {
    Q_OBJECT

public:
    TimelineWidget(QWidget* parent, int maximumValue);
    ~TimelineWidget();

    int getStartValue();
    int getPlaybackValue();
    int getEndValue();
    void setFreeplayMode(bool freeplayMode);
    void setStartValue(int startValue);
    void setPlaybackValue(int playbackValue);
    void setEndValue(int endValue);
    void setMaximumValue(int maximumValue);
    void moveLeft();
    void moveRight();

signals:
    void startValueChanged(int value);
    void playbackValueChanged(int value);
    void endValueChanged(int value);

    void startSliderDraggingStarted();
    void startSliderDraggingFinished();

    void playbackSliderDraggingStarted();
    void playbackSliderDraggingFinished();

    void endSliderDraggingStarted();
    void endSliderDraggingFinished();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent([[maybe_unused]] QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent([[maybe_unused]] QMouseEvent *event);

private:
    TimelineSliderWidget *startSlider;
    TimelineSliderWidget *playbackSlider;
    TimelineSliderWidget *endSlider;

    void dragSlider(TimelineSliderWidget *slider, int x);

    void drawBackgroundIfNeeded();
    void drawLine();
    void drawRangeLine();
    void drawSliders();
    void drawSlider(TimelineSliderWidget *slider);
    void redraw();
    void movePlaybackIndicatorByOffset(int offset);

    const int lineHeight = 4;
    const int minimumHeight = 20;
    const int minimumValue = 0;
    int maximumValue;
    bool freeplayMode;
    TimelineSliderWidget* draggingSlider;
    const bool debug = false;
};
