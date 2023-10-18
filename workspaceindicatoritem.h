#pragma once

#include <QRect>
#include <QImage>

class WorkspaceIndicatorItem
{
public:
    enum Alignment {
        Left,
        Center,
        Right
    };

    WorkspaceIndicatorItem(
        int minimal,
        int value,
        int maximal,
        int rangeLineHorizontalIndent,
        QImage image,
        Alignment alignment
        );

    void update(QSize parentSize);

    void setRange(int minimal, int value, int maximal);
    void setX(int x);
    int getX();
    int getRightBorder();
    bool getIsVisible();

    QRect getRectangle();

    static const int borderWidth = 0;
    static const int width = 20;
    static const int height = 20;

    bool hitTest(int x, int y);
    bool isHighlighted();
    void setIsHighlighted(bool isHighlighted);
    void setIsVisible(bool isVisible);

    QImage getImage();

    QString DEBUG_NAME = "";

private:
   void setValue(int value);
    int getValue();

    bool highlighted;
    int minimal;
    int value;
    int maximal;
    int rangeLineHorizontalIndent;
    Alignment alignment;
    int indicatorPositionX;
    int rangeLineWidth;
    bool isVisible;
    QRect rectangle;
    QImage image;
};
