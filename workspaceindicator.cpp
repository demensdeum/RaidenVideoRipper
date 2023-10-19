#include "WorkspaceIndicator.h"
#include <QLabel>
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMouseEvent>

WorkspaceIndicator::WorkspaceIndicator(QWidget* parent, int maximumValue) : QWidget(parent) {
    auto startValue = 0;
    auto playbackValue = 50;
    auto endValue = 100;
    maximumValue = endValue;
    this->maximumValue = maximumValue;
    startSlider = new WorkspaceIndicatorSlider(
        startValue,
        maximumValue,
        WorkspaceIndicatorSlider::Left
        );
    playbackSlider = new WorkspaceIndicatorSlider(
        playbackValue,
        maximumValue,
        WorkspaceIndicatorSlider::Center
        );
    endSlider = new WorkspaceIndicatorSlider(
        endValue,
        maximumValue,
        WorkspaceIndicatorSlider::Right
        );
    this->draggingSlider = nullptr;
    this->setMinimumHeight(minimumHeight);
}

WorkspaceIndicator::~WorkspaceIndicator()
{
    delete startSlider;
    delete playbackSlider;
    delete endSlider;
}

int WorkspaceIndicator::getStartValue()
{
    return startSlider->getValue();
}

int WorkspaceIndicator::getEndValue()
{
    return endSlider->getValue();
}

void WorkspaceIndicator::setFreeplayMode(bool freeplayMode)
{
    this->freeplayMode = freeplayMode;
}

void WorkspaceIndicator::setStartValue(int startValue)
{
    startSlider->setValue(startValue);
}

void WorkspaceIndicator::setPlaybackValue(int playbackValue)
{
    playbackSlider->setValue(playbackValue);
}

void WorkspaceIndicator::setEndValue(int endValue)
{
    endSlider->setValue(endValue);
}

void WorkspaceIndicator::setMaximumValue(int maximumValue)
{
    startSlider->setMaximumValue(maximumValue);
    playbackSlider->setMaximumValue(maximumValue);
    endSlider->setMaximumValue(maximumValue);
}

void WorkspaceIndicator::resizeEvent(QResizeEvent *event)
{
    QSize parentSize = event->size();
    startSlider->setParentSize(parentSize);
    playbackSlider->setParentSize(parentSize);
    endSlider->setParentSize(parentSize);
}

void WorkspaceIndicator::drawBackgroundIfNeeded()
{
    auto rectangle = QRect(0, 0, width(), height());
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
    painter.setBrush(Qt::red);
    painter.drawRect(rectangle);
}

void WorkspaceIndicator::drawLine()
{
    auto rectangle = QRect(
        WorkspaceIndicatorSlider::width,
        height() / 2 - WorkspaceIndicatorSlider::height / 2,
        width() - WorkspaceIndicatorSlider::width * 2,
        lineHeight
    );
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
    painter.setPen(QPen(Qt::yellow, 1));
    painter.setBrush(Qt::red);
    painter.drawRect(rectangle);
}

void WorkspaceIndicator::drawSlider(WorkspaceIndicatorSlider *slider)
{
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
    painter.setPen(QPen(Qt::yellow, 1));
    painter.setBrush(Qt::red);
    painter.drawRect(slider->getRenderRectangle());
}

void WorkspaceIndicator::drawSliders()
{
    startSlider->setParentSize(size());
    playbackSlider->setParentSize(size());
    endSlider->setParentSize(size());

    auto sliders = {startSlider, endSlider, playbackSlider};

    for (auto slider : sliders) {
        drawSlider(slider);
    }
}

void WorkspaceIndicator::paintEvent([[maybe_unused]] QPaintEvent *event)
{
    redraw();
}

void WorkspaceIndicator::redraw()
{
    drawBackgroundIfNeeded();
    drawLine();
    drawSliders();
}

void WorkspaceIndicator::dragSlider(WorkspaceIndicatorSlider *slider, int x)
{
    slider->drag(x);
}

void WorkspaceIndicator::mousePressEvent(QMouseEvent *event)
{
    auto x = event->position().x();
    if (draggingSlider) {
        dragSlider(draggingSlider, x);
        return;
    }
}

void WorkspaceIndicator::mouseMoveEvent(QMouseEvent *event)
{
    auto x = event->position().x();
    dragSlider(draggingSlider, x);
}

void WorkspaceIndicator::mouseReleaseEvent([[maybe_unused]]QMouseEvent *event)
{
    auto x = event->position().x();
    if (draggingSlider) {
        dragSlider(draggingSlider, x);
        draggingSlider = nullptr;
    }
}
