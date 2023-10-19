#pragma once

#include <QSize>
#include <QRect>

class WorkspaceIndicatorSlider
{
public:
    enum Alignment {
        Left,
        Center,
        Right
    };

    WorkspaceIndicatorSlider(
        int value,
        int maximumValue,
        Alignment alignment
        );

    int getValue();
    void setValue(int value);
    void setMaximumValue(int value);
    void setParentSize(QSize parentSize);
    bool hitTest(int x, int y);
    void drag(int x);
    QRect getRenderRectangle();
    int alignRenderX(int x);

    static const int width = 20;
    static const int height = 20;

private:
    void updateRenderRectangle();

    const int minimumValue = 0;
    int maximumValue;
    int value;

    QSize parentSize;
    QRect renderRectangle;

    Alignment alignment;
    int x;
};
