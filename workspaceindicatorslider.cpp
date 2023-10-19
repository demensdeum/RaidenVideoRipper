#include "workspaceindicatorslider.h"
#include <QDebug>

WorkspaceIndicatorSlider::WorkspaceIndicatorSlider(
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

float WorkspaceIndicatorSlider::ratio() {
    auto ratio = float(value) / float(maximumValue);
    return ratio;
}

float WorkspaceIndicatorSlider::xToRatio(int x) {
    auto adaptedX = x;
    switch (alignment) {
    case WorkspaceIndicatorSlider::Left:
        adaptedX -= width / 2;
        break;

    case WorkspaceIndicatorSlider::Center:
        adaptedX -= width;
        break;

    case WorkspaceIndicatorSlider::Right:
        adaptedX -= width * 1.5;
        break;
    }
    auto ratio = float(adaptedX) / float(lineWidth());
    return ratio;
}

QImage WorkspaceIndicatorSlider::getImage()
{
    return image;
}

int WorkspaceIndicatorSlider::getValue()
{
    return value;
}

void WorkspaceIndicatorSlider::setValue(int value)
{
    this->value = value;
    this->updateRenderRectangle();
}

void WorkspaceIndicatorSlider::setMaximumValue(int value)
{
    this->maximumValue = value;
    this->updateRenderRectangle();
}

void WorkspaceIndicatorSlider::setParentSize(QSize parentSize)
{
    this->parentSize = parentSize;
    this->updateRenderRectangle();
}

QRect WorkspaceIndicatorSlider::getRenderRectangle() {
    return renderRectangle;
}

bool WorkspaceIndicatorSlider::hitTest(int x, int y)
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

void WorkspaceIndicatorSlider::dragToX(int x){
    auto ratio = xToRatio(x);
    setValue(maximumValue * ratio);
}

void WorkspaceIndicatorSlider::setIsHidden(bool isHidden)
{
    this->isHidden = isHidden;
}

int WorkspaceIndicatorSlider::getTargetX()
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

bool WorkspaceIndicatorSlider::getIsHidden()
{
    return isHidden;
}

int WorkspaceIndicatorSlider::alignRenderX(int renderX)
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

int WorkspaceIndicatorSlider::lineWidth()
{
    auto lineWidth = parentSize.width() - width * 2;
    return lineWidth;
}

void WorkspaceIndicatorSlider::updateRenderRectangle()
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
