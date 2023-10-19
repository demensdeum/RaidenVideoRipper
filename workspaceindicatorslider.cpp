#include "workspaceindicatorslider.h"

WorkspaceIndicatorSlider::WorkspaceIndicatorSlider(
    int value,
    int maximumValue,
    Alignment alignment
    )
{
    this->value = value;
    this->maximumValue = maximumValue;
    this->alignment = alignment;
}

int WorkspaceIndicatorSlider::getValue()
{
    return value;
}

void WorkspaceIndicatorSlider::setValue(int value)
{
    this->value = value;
}

void WorkspaceIndicatorSlider::setMaximumValue(int value)
{
    this->maximumValue = value;
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

void WorkspaceIndicatorSlider::drag(int x){
    // TODO: convert x to value
    this->x = x;
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

void WorkspaceIndicatorSlider::updateRenderRectangle()
{
    auto ratio = float(value) / float(maximumValue);
    auto lineWidth = parentSize.width() - width * 2;
    auto renderX = lineWidth * ratio;
    renderX = alignRenderX(renderX);
    auto renderY = parentSize.height() / 2 - height / 2;
    renderRectangle = QRect(
        renderX,
        renderY,
        width,
        height
    );
}
