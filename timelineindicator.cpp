#include "timelineindicator.h"
#include <QLabel>
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMouseEvent>

TimelineIndicator::TimelineIndicator(QWidget* parent, int maximumValue) : QWidget(parent) {
    auto startValue = 0;
    auto endValue = 10000;
    auto playbackValue = endValue / 2;
    maximumValue = endValue;
    this->maximumValue = maximumValue;
    startSlider = new TimelineIndicatorSlider(
        startValue,
        maximumValue,
        TimelineIndicatorSlider::Left,
        QImage("://resources/images/startSliderImage.png"),
        false
        );
    playbackSlider = new TimelineIndicatorSlider(
        playbackValue,
        maximumValue,
        TimelineIndicatorSlider::Center,
        QImage("://resources/images/playbackSliderImage.png"),
        false
        );
    endSlider = new TimelineIndicatorSlider(
        endValue,
        maximumValue,
        TimelineIndicatorSlider::Right,
        QImage("://resources/images/endSliderImage.png"),
        false
        );
    this->draggingSlider = nullptr;
    this->setMinimumHeight(minimumHeight);
}

TimelineIndicator::~TimelineIndicator()
{
    delete startSlider;
    delete playbackSlider;
    delete endSlider;
}

int TimelineIndicator::getStartValue()
{
    return startSlider->getValue();
}

int TimelineIndicator::getPlaybackValue()
{
    return playbackSlider->getValue();
}

int TimelineIndicator::getEndValue()
{
    return endSlider->getValue();
}

void TimelineIndicator::setFreeplayMode(bool freeplayMode)
{
    this->freeplayMode = freeplayMode;
    startSlider->setIsHidden(freeplayMode);
    endSlider->setIsHidden(freeplayMode);
}

void TimelineIndicator::setStartValue(int startValue)
{
    startSlider->setValue(startValue);
    update();
}

void TimelineIndicator::setPlaybackValue(int playbackValue)
{
    playbackSlider->setValue(playbackValue);
    update();
}

void TimelineIndicator::setEndValue(int endValue)
{
    endSlider->setValue(endValue);
    update();
}

void TimelineIndicator::setMaximumValue(int maximumValue)
{
    startSlider->setMaximumValue(maximumValue);
    playbackSlider->setMaximumValue(maximumValue);
    endSlider->setMaximumValue(maximumValue);
}

void TimelineIndicator::resizeEvent(QResizeEvent *event)
{
    QSize parentSize = event->size();
    startSlider->setParentSize(parentSize);
    playbackSlider->setParentSize(parentSize);
    endSlider->setParentSize(parentSize);
}

void TimelineIndicator::drawBackgroundIfNeeded()
{
    if (debug) {
    auto rectangle = QRect(0, 0, width(), height());
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
    painter.setBrush(Qt::red);
    painter.drawRect(rectangle);
    }
}

void TimelineIndicator::drawRangeLine()
{
    auto rectangle = QRect(
        startSlider->getTargetX(),
        height() / 2 - lineHeight / 2,
        endSlider->getTargetX() - startSlider->getTargetX(),
        lineHeight
        );
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
    painter.setPen(QPen(Qt::lightGray, 1));
    painter.setBrush(Qt::blue);
    painter.drawRoundedRect(rectangle, 1, 1);
}

void TimelineIndicator::drawLine()
{
    auto rectangle = QRect(
        TimelineIndicatorSlider::width,
        height() / 2 - lineHeight / 2,
        width() - TimelineIndicatorSlider::width * 2,
        lineHeight
    );
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
    painter.setPen(QPen(QColor(0xd6d6d6), 1));
    painter.setBrush(QColor(0xe7eaea));
    painter.drawRoundedRect(rectangle, 1, 1);
}

void TimelineIndicator::drawSlider(TimelineIndicatorSlider *slider)
{
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
    //painter.setPen(QPen(Qt::yellow, 1));
    //painter.setBrush(Qt::lightGray);
    //painter.drawRect(slider->getRenderRectangle());
    painter.drawImage(slider->getRenderRectangle(), slider->getImage());
}

void TimelineIndicator::drawSliders()
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

void TimelineIndicator::paintEvent([[maybe_unused]] QPaintEvent *event)
{
    redraw();
}

void TimelineIndicator::redraw()
{
    drawBackgroundIfNeeded();
    drawLine();
    drawRangeLine();
    drawSliders();
}

void TimelineIndicator::dragSlider(TimelineIndicatorSlider *slider, int x)
{
    if (draggingSlider->getIsHidden()) {
        return;
    }

    auto ratio = slider->xToRatio(x);
    if (ratio < 0 || ratio > 1) {
        return;
    }

    if (draggingSlider == startSlider && ratio > endSlider->ratio()) {
        return;
    }

    if (draggingSlider == endSlider && ratio < startSlider->ratio()) {
        return;
    }

    draggingSlider->dragToX(x);
}

void TimelineIndicator::movePlaybackIndicatorByOffset(int offset)
{
    if (draggingSlider)
    {
        return;
    }
    auto x = playbackSlider->getTargetX() + offset;
    auto ratio = playbackSlider->xToRatio(x);
    if (ratio < 0 || ratio > 1) {
        return;
    }
    playbackSlider->dragToX(x);
    emit playbackValueChanged(playbackSlider->getValue());
    update();
}

void TimelineIndicator::moveLeft()
{
    movePlaybackIndicatorByOffset(-4);
}

void TimelineIndicator::moveRight()
{
    movePlaybackIndicatorByOffset(+4);
}

void TimelineIndicator::mousePressEvent(QMouseEvent *event)
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
            if (draggingSlider == startSlider) {
                emit startSliderDraggingStarted();
            }
            else if (draggingSlider == playbackSlider) {
                emit playbackSliderDraggingStarted();
            }
            else if (draggingSlider == endSlider) {
                emit endSliderDraggingStarted();
            }
            break;
        }
    }
}

void TimelineIndicator::mouseMoveEvent(QMouseEvent *event)
{
    if (!draggingSlider)
    {
        return;
    }
    auto x = event->position().x();
    dragSlider(draggingSlider, x);
    if (draggingSlider == startSlider) {
        emit startValueChanged(draggingSlider->getValue());
    }
    else if (draggingSlider == playbackSlider) {
        emit playbackValueChanged(draggingSlider->getValue());
    }
    else if (draggingSlider == endSlider) {
        emit endValueChanged(draggingSlider->getValue());
    }
    update();
}

void TimelineIndicator::mouseReleaseEvent([[maybe_unused]]QMouseEvent *event)
{
    auto x = event->position().x();
    if (draggingSlider) {
        dragSlider(draggingSlider, x);
        if (draggingSlider == startSlider) {
            emit startSliderDraggingFinished();
        }
        else if (draggingSlider == playbackSlider) {
            emit playbackSliderDraggingFinished();
        }
        else if (draggingSlider == endSlider) {
            emit endSliderDraggingFinished();
        }
        draggingSlider = nullptr;
    }
}
