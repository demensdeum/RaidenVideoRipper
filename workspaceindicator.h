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

signals:
    void startValueChanged(int value);
    void playbackValueChanged(int value);
    void endValueChanged(int value);

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
    void drawSliders();
    void drawSlider(WorkspaceIndicatorSlider *slider);
    void redraw();

    const int lineHeight = 20;
    const int minimumHeight = 100;
    const int minimumValue = 0;
    int maximumValue;
    bool freeplayMode;
    WorkspaceIndicatorSlider* draggingSlider;
};
