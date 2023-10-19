#include "WorkspaceIndicator.h"
#include <QLabel>
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMouseEvent>

WorkspaceIndicator::WorkspaceIndicator(QWidget* parent, int maximumValue) : QWidget(parent) {
    auto startValue = 0;
    auto endValue = 10000;
    auto playbackValue = endValue / 2;
    maximumValue = endValue;
    this->maximumValue = maximumValue;
    startSlider = new WorkspaceIndicatorSlider(
        startValue,
        maximumValue,
        WorkspaceIndicatorSlider::Left,
        QImage("startSliderImage.png"),
        false
        );
    playbackSlider = new WorkspaceIndicatorSlider(
        playbackValue,
        maximumValue,
        WorkspaceIndicatorSlider::Center,
        QImage("playbackSliderImage.png"),
        false
        );
    endSlider = new WorkspaceIndicatorSlider(
        endValue,
        maximumValue,
        WorkspaceIndicatorSlider::Right,
        QImage("endSliderImage.png"),
        false
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
    startSlider->setIsHidden(freeplayMode);
    endSlider->setIsHidden(freeplayMode);
}

void WorkspaceIndicator::setStartValue(int startValue)
{
    startSlider->setValue(startValue);
    update();
}

void WorkspaceIndicator::setPlaybackValue(int playbackValue)
{
    playbackSlider->setValue(playbackValue);
    update();
}

void WorkspaceIndicator::setEndValue(int endValue)
{
    endSlider->setValue(endValue);
    update();
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
#if DEBUG
    auto rectangle = QRect(0, 0, width(), height());
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
    painter.setBrush(Qt::red);
    painter.drawRect(rectangle);
#endif
}

void WorkspaceIndicator::drawLine()
{
    auto rectangle = QRect(
        WorkspaceIndicatorSlider::width,
        height() / 2 - lineHeight / 2,
        width() - WorkspaceIndicatorSlider::width * 2,
        lineHeight
    );
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
    painter.setPen(QPen(Qt::lightGray, 1));
    painter.setBrush(QColor(0xe7eaea));
    painter.drawRoundedRect(rectangle, 1, 1);
}

void WorkspaceIndicator::drawSlider(WorkspaceIndicatorSlider *slider)
{
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
    //painter.setPen(QPen(Qt::yellow, 1));
    //painter.setBrush(Qt::lightGray);
    //painter.drawRect(slider->getRenderRectangle());
    painter.drawImage(slider->getRenderRectangle(), slider->getImage());
}

void WorkspaceIndicator::drawSliders()
{
    startSlider->setParentSize(size());
    playbackSlider->setParentSize(size());
    endSlider->setParentSize(size());

    auto sliders = {startSlider, endSlider, playbackSlider};

    for (auto slider : sliders) {
        if (slider->getIsHidden()) {
            continue;
        }
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
    auto ratio = slider->xToRatio(x);
    if (ratio < 0 && ratio > 1) {
        return;
    }

    if (draggingSlider == startSlider && ratio > endSlider->ratio()) {
        return;
    }

    if (draggingSlider == endSlider && ratio < startSlider->ratio()) {
        return;
    }

    draggingSlider->drag(x);
}

void WorkspaceIndicator::mousePressEvent(QMouseEvent *event)
{
    auto x = event->position().x();
    auto y = event->position().y();

    if (draggingSlider) {
        dragSlider(draggingSlider, x);
        return;
    }

    auto sliders = {startSlider, endSlider, playbackSlider};
    for (auto slider : sliders) {
        auto hitSlider = slider->hitTest(x, y);
        if (hitSlider) {
            draggingSlider = slider;
            break;
        }
    }
}

void WorkspaceIndicator::mouseMoveEvent(QMouseEvent *event)
{
    if (!draggingSlider)
    {
        return;
    }
    auto x = event->position().x();
    dragSlider(draggingSlider, x);
    emit startValueChanged(draggingSlider->getValue());
    update();
}

void WorkspaceIndicator::mouseReleaseEvent([[maybe_unused]]QMouseEvent *event)
{
    auto x = event->position().x();
    if (draggingSlider) {
        dragSlider(draggingSlider, x);
        draggingSlider = nullptr;
    }
}
