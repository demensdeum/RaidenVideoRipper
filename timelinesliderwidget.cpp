#include "timelinesliderwidget.h"
#include <QDebug>

TimelineSliderWidget::TimelineSliderWidget(
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

float TimelineSliderWidget::ratio() {
    auto ratio = float(value) / float(maximumValue);
    return ratio;
}

float TimelineSliderWidget::xToRatio(int x) {
    auto adaptedX = x;
    switch (alignment) {
    case TimelineSliderWidget::Left:
        adaptedX -= width / 2;
        break;
        
    case TimelineSliderWidget::Center:
        adaptedX -= width;
        break;
        
    case TimelineSliderWidget::Right:
        adaptedX -= width * 1.5;
        break;
    }
    auto ratio = float(adaptedX) / float(lineWidth());
    return ratio;
}

QImage TimelineSliderWidget::getImage()
{
    return image;
}

int TimelineSliderWidget::getValue()
{
    return value;
}

void TimelineSliderWidget::setValue(int value)
{
    this->value = value;
    this->updateRenderRectangle();
}

void TimelineSliderWidget::setMaximumValue(int value)
{
    this->maximumValue = value;
    this->updateRenderRectangle();
}

void TimelineSliderWidget::setParentSize(QSize parentSize)
{
    this->parentSize = parentSize;
    this->updateRenderRectangle();
}

QRect TimelineSliderWidget::getRenderRectangle() {
    return renderRectangle;
}

bool TimelineSliderWidget::hitTest(int x, int y)
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

void TimelineSliderWidget::dragToX(int x){
    auto ratio = xToRatio(x);
    setValue(maximumValue * ratio);
}

void TimelineSliderWidget::setIsHidden(bool isHidden)
{
    this->isHidden = isHidden;
}

int TimelineSliderWidget::getTargetX()
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

bool TimelineSliderWidget::getIsHidden()
{
    return isHidden;
}

int TimelineSliderWidget::alignRenderX(int renderX)
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

int TimelineSliderWidget::lineWidth()
{
    auto lineWidth = parentSize.width() - width * 2;
    return lineWidth;
}

void TimelineSliderWidget::updateRenderRectangle()
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
