#include "timelinewidget.h"
#include <QLabel>
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMouseEvent>

TimelineWidget::TimelineWidget(QWidget* parent, int maximumValue) : QWidget(parent) {
    auto startValue = 0;
    auto endValue = 10000;
    auto playbackValue = endValue / 2;
    maximumValue = endValue;
    this->maximumValue = maximumValue;
    startSlider = new TimelineSliderWidget(
        startValue,
        maximumValue,
        TimelineSliderWidget::Left,
        QImage("://resources/images/startSliderImage.png"),
        false
        );
    playbackSlider = new TimelineSliderWidget(
        playbackValue,
        maximumValue,
        TimelineSliderWidget::Center,
        QImage("://resources/images/playbackSliderImage.png"),
        false
        );
    endSlider = new TimelineSliderWidget(
        endValue,
        maximumValue,
        TimelineSliderWidget::Right,
        QImage("://resources/images/endSliderImage.png"),
        false
        );
    this->draggingSlider = nullptr;
    this->setMinimumHeight(minimumHeight);
}

TimelineWidget::~TimelineWidget()
{
    delete startSlider;
    delete playbackSlider;
    delete endSlider;
}

int TimelineWidget::getStartValue()
{
    return startSlider->getValue();
}

int TimelineWidget::getPlaybackValue()
{
    return playbackSlider->getValue();
}

int TimelineWidget::getEndValue()
{
    return endSlider->getValue();
}

void TimelineWidget::setFreeplayMode(bool freeplayMode)
{
    this->freeplayMode = freeplayMode;
    startSlider->setIsHidden(freeplayMode);
    endSlider->setIsHidden(freeplayMode);
}

void TimelineWidget::setStartValue(int startValue)
{
    startSlider->setValue(startValue);
    update();
}

void TimelineWidget::setPlaybackValue(int playbackValue)
{
    playbackSlider->setValue(playbackValue);
    update();
}

void TimelineWidget::setEndValue(int endValue)
{
    endSlider->setValue(endValue);
    update();
}

void TimelineWidget::setMaximumValue(int maximumValue)
{
    startSlider->setMaximumValue(maximumValue);
    playbackSlider->setMaximumValue(maximumValue);
    endSlider->setMaximumValue(maximumValue);
}

void TimelineWidget::resizeEvent(QResizeEvent *event)
{
    QSize parentSize = event->size();
    startSlider->setParentSize(parentSize);
    playbackSlider->setParentSize(parentSize);
    endSlider->setParentSize(parentSize);
}

void TimelineWidget::drawBackgroundIfNeeded()
{
    if (debug) {
    auto rectangle = QRect(0, 0, width(), height());
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
    painter.setBrush(Qt::red);
    painter.drawRect(rectangle);
    }
}

void TimelineWidget::drawRangeLine()
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
    painter.setBrush(QColor(0x007ad9));
    painter.drawRoundedRect(rectangle, 1, 1);
}

void TimelineWidget::drawLine()
{
    auto rectangle = QRect(
        TimelineSliderWidget::width,
        height() / 2 - lineHeight / 2,
        width() - TimelineSliderWidget::width * 2,
        lineHeight
    );
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
    painter.setPen(QPen(QColor(0xd6d6d6), 1));
    painter.setBrush(QColor(0xe7eaea));
    painter.drawRoundedRect(rectangle, 1, 1);
}

void TimelineWidget::drawSlider(TimelineSliderWidget *slider)
{
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
    //painter.setPen(QPen(Qt::yellow, 1));
    //painter.setBrush(Qt::lightGray);
    //painter.drawRect(slider->getRenderRectangle());
    painter.drawImage(slider->getRenderRectangle(), slider->getImage());
}

void TimelineWidget::drawSliders()
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

void TimelineWidget::paintEvent([[maybe_unused]] QPaintEvent *event)
{
    redraw();
}

void TimelineWidget::redraw()
{
    drawBackgroundIfNeeded();
    drawLine();
    drawRangeLine();
    drawSliders();
}

void TimelineWidget::dragSlider(TimelineSliderWidget *slider, int x)
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

void TimelineWidget::movePlaybackIndicatorByOffset(int offset)
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

void TimelineWidget::moveLeft()
{
    movePlaybackIndicatorByOffset(-4);
}

void TimelineWidget::moveRight()
{
    movePlaybackIndicatorByOffset(+4);
}

void TimelineWidget::mousePressEvent(QMouseEvent *event)
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

void TimelineWidget::mouseMoveEvent(QMouseEvent *event)
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

void TimelineWidget::mouseReleaseEvent([[maybe_unused]]QMouseEvent *event)
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
