#pragma once

#include <QWidget>
#include <timelineindicatorslider.h>

class TimelineIndicator : public QWidget {
    Q_OBJECT

public:
    TimelineIndicator(QWidget* parent, int maximumValue);
    ~TimelineIndicator();

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
    TimelineIndicatorSlider *startSlider;
    TimelineIndicatorSlider *playbackSlider;
    TimelineIndicatorSlider *endSlider;

    void dragSlider(TimelineIndicatorSlider *slider, int x);

    void drawBackgroundIfNeeded();
    void drawLine();
    void drawRangeLine();
    void drawSliders();
    void drawSlider(TimelineIndicatorSlider *slider);
    void redraw();
    void movePlaybackIndicatorByOffset(int offset);

    const int lineHeight = 4;
    const int minimumHeight = 20;
    const int minimumValue = 0;
    int maximumValue;
    bool freeplayMode;
    TimelineIndicatorSlider* draggingSlider;
    const bool debug = false;
};
