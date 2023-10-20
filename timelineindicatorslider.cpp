#include "timelineindicatorslider.h"
#include <QDebug>

TimelineIndicatorSlider::TimelineIndicatorSlider(
    int value,
    int maximumValue,
    Alignment alignment,
    QImage image,
    bool isHidden
    )
{
    this->isHidden = isHidden;
    this->value = value;
    this->maximumValue = maximumValue;
    this->alignment = alignment;
    this->image = image;
}

float TimelineIndicatorSlider::ratio() {
    auto ratio = float(value) / float(maximumValue);
    return ratio;
}

float TimelineIndicatorSlider::xToRatio(int x) {
    auto adaptedX = x;
    switch (alignment) {
    case TimelineIndicatorSlider::Left:
        adaptedX -= width / 2;
        break;

    case TimelineIndicatorSlider::Center:
        adaptedX -= width;
        break;

    case TimelineIndicatorSlider::Right:
        adaptedX -= width * 1.5;
        break;
    }
    auto ratio = float(adaptedX) / float(lineWidth());
    return ratio;
}

QImage TimelineIndicatorSlider::getImage()
{
    return image;
}

int TimelineIndicatorSlider::getValue()
{
    return value;
}

void TimelineIndicatorSlider::setValue(int value)
{
    this->value = value;
    this->updateRenderRectangle();
}

void TimelineIndicatorSlider::setMaximumValue(int value)
{
    this->maximumValue = value;
    this->updateRenderRectangle();
}

void TimelineIndicatorSlider::setParentSize(QSize parentSize)
{
    this->parentSize = parentSize;
    this->updateRenderRectangle();
}

QRect TimelineIndicatorSlider::getRenderRectangle() {
    return renderRectangle;
}

bool TimelineIndicatorSlider::hitTest(int x, int y)
{
    if (x < renderRectangle.x())
    {
        return false;
    }
    if (x > renderRectangle.x() + renderRectangle.width())
    {
        return false;
    }
    if (y < renderRectangle.y())
    {
        return false;
    }
    if (y > renderRectangle.y() + renderRectangle.height())
    {
        return false;
    }

    return true;
}

void TimelineIndicatorSlider::dragToX(int x){
    auto ratio = xToRatio(x);
    setValue(maximumValue * ratio);
}

void TimelineIndicatorSlider::setIsHidden(bool isHidden)
{
    this->isHidden = isHidden;
}

int TimelineIndicatorSlider::getTargetX()
{
    auto adaptedX = renderRectangle.x();
    switch (alignment) {
    case Left:
        return adaptedX + width;
    case Center:
        return adaptedX + width / 2;
    case Right:
        return adaptedX;
    }

    return adaptedX;
}

bool TimelineIndicatorSlider::getIsHidden()
{
    return isHidden;
}

int TimelineIndicatorSlider::alignRenderX(int renderX)
{
    switch (alignment) {
    case Left:
        return renderX;
    case Center:
        return renderX + width / 2;
    case Right:
        return renderX + width;
    }

    return renderX;
}

int TimelineIndicatorSlider::lineWidth()
{
    auto lineWidth = parentSize.width() - width * 2;
    return lineWidth;
}

void TimelineIndicatorSlider::updateRenderRectangle()
{
    auto ratio = float(value) / float(maximumValue);
    auto renderX = lineWidth() * ratio;
    renderX = alignRenderX(renderX);
    auto renderY = parentSize.height() / 2 - height / 2;
    renderRectangle = QRect(
        renderX,
        renderY,
        width,
        height
    );
}
