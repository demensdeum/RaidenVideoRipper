#include "workspaceindicatoritem.h"
#include <QDebug>

// TODO: Move to QWidget

WorkspaceIndicatorItem::WorkspaceIndicatorItem(
    int minimal,
    int value,
    int maximal,
    int rangeLineHorizontalIndent,
    QImage image,
    Alignment alignment
    )
{
    this->isVisible = true;
    this->highlighted = false;
    this->minimal = minimal;
    this->setValue(value);
    this->maximal = maximal;
    this->rangeLineHorizontalIndent = rangeLineHorizontalIndent;
    this->rangeLineWidth = 1;
    this->image = image;
    this->alignment = alignment;
    this->indicatorPositionX = 0;
}

QImage WorkspaceIndicatorItem::getImage() {
    return image;
}

bool WorkspaceIndicatorItem::getIsVisible() {
    if (DEBUG_NAME == "startIndicator") {
        qDebug() << "get startIndicatorX: " << value;
    }
    return this->isVisible;
}

void WorkspaceIndicatorItem::setValue(int value) {
    this->value = value;
    if (DEBUG_NAME == "playbackIndicator") {
        qDebug() << "set playbackIndicatorX: " << value;
    }
}

void WorkspaceIndicatorItem::setIsVisible(bool isVisible) {
    this->isVisible = isVisible;
}

bool WorkspaceIndicatorItem::isHighlighted() {
    return highlighted;
}

void WorkspaceIndicatorItem::setIsHighlighted(bool isHighlighted) {
    this->highlighted = isHighlighted;
}

bool WorkspaceIndicatorItem::hitTest(int x, int y) {
    if (!isVisible) {
        return false;
    }
    if (x < rectangle.x()) {
        return false;
    }
    if (x > rectangle.x() + rectangle.width()) {
        return false;
    }
    if (y < rectangle.y()) {
        return false;
    }
    if (y > rectangle.y() + rectangle.height()) {
        return false;
    }

    return true;
}

int WorkspaceIndicatorItem::getRightBorder() {
    auto x = rectangle.x();
    auto width = rectangle.width();
    auto rightBorder = x + width;
    return rightBorder;
}

int WorkspaceIndicatorItem::getX() {
    return rectangle.x();
}

void WorkspaceIndicatorItem::setX(int x) {
    qDebug() <<"setX: " << x;

    if (DEBUG_NAME == "endIndicator") {
        qDebug() << "sthop!!";
    }

    if (x < 0) {
        qDebug() << "sthap";
    }

    rectangle = QRect(
        x - width / 2,
        rectangle.y(),
        rectangle.width(),
        rectangle.height()
        );

    auto ratio = float(x - rangeLineHorizontalIndent) / float(rangeLineWidth);
    auto value = ratio * maximal;
    qDebug() << "value: " << value;
    setValue(value);
};

void WorkspaceIndicatorItem::setRange(int minimal, int value, int maximal) {
    this->minimal = minimal;
    setValue(value);
    this->maximal = maximal;

    if (DEBUG_NAME == "startIndicator") {
        qDebug() << "sthop!!";
    }

    auto ratio = float(value) / float(maximal);
    auto x = ratio * rangeLineWidth;
    x += rangeLineHorizontalIndent;

    rectangle = QRect(
        x,
        rectangle.y(),
        rectangle.width(),
        rectangle.height()
        );
}

QRect WorkspaceIndicatorItem::getRectangle() {
    return rectangle;
}

int WorkspaceIndicatorItem::getValue() {
    return value;
}

void WorkspaceIndicatorItem::update(QSize parentSize) {
    rangeLineWidth = parentSize.width() - rangeLineHorizontalIndent * 2;
    auto ratio = float(getValue()) / float(maximal);
    indicatorPositionX = rangeLineHorizontalIndent + (rangeLineWidth * ratio) - width / 2;

//    if (DEBUG_NAME == "startIndicator") {
//        qDebug() << "sthop!!";
//    }

    switch (alignment) {
    case WorkspaceIndicatorItem::Left:
        indicatorPositionX -= width / 2;
        break;
    case WorkspaceIndicatorItem::Center:
        break;
    case WorkspaceIndicatorItem::Right:
        indicatorPositionX += width / 2;
        break;
    }

    rectangle = QRect(
        indicatorPositionX,
        parentSize.height() / 2 - height / 2,
        width,
        height
        );
}
