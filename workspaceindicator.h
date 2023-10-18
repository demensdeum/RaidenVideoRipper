#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <workspaceindicatoritem.h>

class WorkspaceIndicator : public QWidget {
    Q_OBJECT

public:
    WorkspaceIndicator(QWidget *parent = nullptr);

    int getStartValue();
    int getPlaybackValue();
    int getEndValue();

    void setStartValue(int startValue);
    void setPlaybackValue(int playbackValue);
    void setEndValue(int endValue);
    void setMaximalValue(int maximalValue);
    void setFreeplayMode(bool freeplayMode);

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
    int safeBound(int min, int value, int max);
    void setupUi();
    void createIndicators();
    WorkspaceIndicatorItem* createIndicator(int value, WorkspaceIndicatorItem::Alignment alignment);
    void moveIndicator(WorkspaceIndicatorItem* item, int x);
    void highlightIndicatorIfNeeded(int x, int y);
    void drawItem(WorkspaceIndicatorItem* item);
    int rangeLineWidth();

    QVBoxLayout *layout;
    int heightConstant = 40;

    static const int rangeLineBorderHeight = 1;
    static const int rangeLineHeight = 5;
    static const int rangeLineHorizontalIndent = 20;

    int startValue;
    int playbackValue;
    int endValue;

    int minimalValue;
    int maximalValue;

    bool cursorIsPressed;
    bool freeplayMode;

    std::vector<WorkspaceIndicatorItem*> indicators;

    WorkspaceIndicatorItem* startIndicator;
    WorkspaceIndicatorItem* playbackIndicator;
    WorkspaceIndicatorItem* endIndicator;

    WorkspaceIndicatorItem* draggingIndicator;
};
