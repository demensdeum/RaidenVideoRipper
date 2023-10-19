#pragma once

#include <QWidget>
#include <workspaceindicatorslider.h>

class WorkspaceIndicator : public QWidget {
    Q_OBJECT

public:
    WorkspaceIndicator(QWidget* parent, int maximumValue);
    ~WorkspaceIndicator();

    int getStartValue();
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
    void endSliderDraggingStarted();
    void endSliderDraggingFinished();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent([[maybe_unused]] QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent([[maybe_unused]] QMouseEvent *event);

private:
    WorkspaceIndicatorSlider *startSlider;
    WorkspaceIndicatorSlider *playbackSlider;
    WorkspaceIndicatorSlider *endSlider;

    void dragSlider(WorkspaceIndicatorSlider *slider, int x);

    void drawBackgroundIfNeeded();
    void drawLine();
    void drawRangeLine();
    void drawSliders();
    void drawSlider(WorkspaceIndicatorSlider *slider);
    void redraw();
    void movePlaybackIndicatorByOffset(int offset);

    const int lineHeight = 4;
    const int minimumHeight = 20;
    const int minimumValue = 0;
    int maximumValue;
    bool freeplayMode;
    WorkspaceIndicatorSlider* draggingSlider;
};
