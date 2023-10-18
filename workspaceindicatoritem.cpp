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
    auto adaptedX = x;

    switch (alignment) {
    case WorkspaceIndicatorItem::Left:
        adaptedX = x - width;
        break;
    case WorkspaceIndicatorItem::Center:
        adaptedX = x - width / 2;
        break;
    case WorkspaceIndicatorItem::Right:
        break;
    }

    qDebug() << "1";

    rectangle = QRect(
        adaptedX,
        rectangle.y(),
        width,
        height
        );

    auto ratio = float(adaptedX - rangeLineHorizontalIndent * 2) / float(rangeLineWidth);
    auto value = ratio * maximal;
    setValue(value);

    if (DEBUG_NAME == "playbackIndicatorX") {
        qDebug() << "set playbackIndicator setX";
    }
};

void WorkspaceIndicatorItem::setRange(int minimal, int value, int maximal) {
    this->minimal = minimal;
    setValue(value);
    this->maximal = maximal;
    auto ratio = float(value) / float(maximal);
    auto x = ratio * rangeLineWidth;
    auto adaptedX = x;
    if (alignment == WorkspaceIndicatorItem::Center) {
        adaptedX -= width / 2;
    }

    qDebug() << "2";

    rectangle = QRect(
        adaptedX,
        rectangle.y(),
        rectangle.width(),
        rectangle.height()
        );

    if (DEBUG_NAME == "startIndicator") {
        qDebug() << "setRange";
    }
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

    switch (alignment) {
    case WorkspaceIndicatorItem::Left:
        indicatorPositionX -= width;
        break;
    case WorkspaceIndicatorItem::Center:
        break;
    case WorkspaceIndicatorItem::Right:
        indicatorPositionX += width;
        break;
    }

    if (indicatorPositionX + width / 2 < rangeLineHorizontalIndent) {
        indicatorPositionX = 0;
    }
    else if (indicatorPositionX > parentSize.width() - rangeLineHorizontalIndent) {
        indicatorPositionX = parentSize.width() - rangeLineHorizontalIndent * 2;
    }

    rectangle = QRect(
        indicatorPositionX,
        parentSize.height() / 2 - height / 2,
        width,
        height
        );
}
