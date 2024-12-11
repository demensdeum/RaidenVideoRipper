#pragma once

#include <QSize>
#include <QRect>
#include <QImage>

class TimelineSliderWidget
{
public:
    enum Alignment {
        Left,
        Center,
        Right
    };
    
    TimelineSliderWidget(
        int value,
        int maximumValue,
        Alignment alignment,
        QImage image,
        bool isHidden
        );

    QImage getImage();
    int getValue();
    void setValue(int value);
    void setMaximumValue(int value);
    void setParentSize(QSize parentSize);
    bool hitTest(int x, int y);
    void dragToX(int x);
    QRect getRenderRectangle();
    int alignRenderX(int x);
    int lineWidth();
    float xToRatio(int x);
    float ratio();
    void setIsHidden(bool isHidden);
    bool getIsHidden();
    int getTargetX();

    static const int width = 20;
    static const int height = 20;

private:
    void updateRenderRectangle();

    int maximumValue;
    int value;

    QSize parentSize;
    QRect renderRectangle;

    Alignment alignment;
    QImage image;

    bool isHidden;
};
