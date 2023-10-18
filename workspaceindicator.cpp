#include "WorkspaceIndicator.h"
#include <QLabel>
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMouseEvent>

WorkspaceIndicator::WorkspaceIndicator(QWidget* parent) : QWidget(parent) {
    setupUi();
}

void WorkspaceIndicator::setupUi() {
    layout = new QVBoxLayout(this);
    setMinimumHeight(heightConstant);
    setMaximumHeight(heightConstant);
    minimalValue = 0;
    maximalValue = 100;
    startValue = 0;
    playbackValue = 50;
    endValue = maximalValue;
    setMouseTracking(true);

    createIndicators();
}

void WorkspaceIndicator::createIndicators() {
    startIndicator = createIndicator(startValue, WorkspaceIndicatorItem::Left);
    playbackIndicator = createIndicator(playbackValue, WorkspaceIndicatorItem::Center);
    endIndicator = createIndicator(endValue, WorkspaceIndicatorItem::Right);

    indicators = {startIndicator, playbackIndicator, endIndicator};
}

WorkspaceIndicatorItem* WorkspaceIndicator::createIndicator(int value, WorkspaceIndicatorItem::Alignment alignment) {
    return new WorkspaceIndicatorItem(minimalValue, value, maximalValue, rangeLineHorizontalIndent, alignment);
}

int WorkspaceIndicator::getStartValue() {
    return startValue;
}

void WorkspaceIndicator::setFreeplayMode(bool freeplayMode) {
    this->freeplayMode = freeplayMode;
    startIndicator->setIsVisible(!freeplayMode);
    endIndicator->setIsVisible(!freeplayMode);

    auto inRange = playbackIndicator->getX() > startIndicator->getRightBorder()
                   ||
                   playbackIndicator->getRightBorder() < endIndicator->getX();

    if (!freeplayMode && !inRange) {
        playbackIndicator->setX(
            safeBound(
                startIndicator->getX(),
                playbackIndicator->getX(),
                endIndicator->getRightBorder()
            )
        );
    }

    update();
}

int WorkspaceIndicator::getPlaybackValue() {
    return playbackValue;
}

int WorkspaceIndicator::getEndValue() {
    return endValue;
}

void WorkspaceIndicator::setStartValue(int startValue) {
    this->startValue = qMax(0, startValue);
    startIndicator->setRange(minimalValue, this->startValue, maximalValue);
    update();
}

int WorkspaceIndicator::safeBound(int min, int value, int max) {
    if (max < min) {
        qDebug() << "Broken bound!!!";
        return value;
    }
    return value;
}

void WorkspaceIndicator::setPlaybackValue(int playbackValue) {
    if (!freeplayMode) {
        playbackValue = safeBound(
            startValue,
            playbackValue,
            endValue
        );
    }
    this->playbackValue = playbackValue;
    playbackIndicator->setRange(minimalValue, playbackValue, maximalValue);
    playbackIndicator->update(size());
    update();
}

void WorkspaceIndicator::setEndValue(int endValue) {
    this->endValue = qMax(0, endValue);
    endIndicator->setRange(minimalValue, this->endValue, maximalValue);
    update();
}

void WorkspaceIndicator::setMaximalValue(int maximalValue) {
    this->maximalValue = qMax(0, maximalValue);
    update();
}

void WorkspaceIndicator::mousePressEvent([[maybe_unused]] QMouseEvent* event) {
    cursorIsPressed = true;
}

void WorkspaceIndicator::mouseMoveEvent([[maybe_unused]] QMouseEvent* event) {
    if (!cursorIsPressed) return;

    auto x = event->position().x();
    auto y = event->position().y();

    if (draggingIndicator) {
        moveIndicator(draggingIndicator, x);
        return;
    }

    if (startIndicator->hitTest(x, y)) {
        moveIndicator(startIndicator, x);
    } else if (playbackIndicator->hitTest(x, y)) {
        moveIndicator(playbackIndicator, x);
    } else if (endIndicator->hitTest(x, y)) {
        moveIndicator(endIndicator, x);
    }
}

void WorkspaceIndicator::moveIndicator(WorkspaceIndicatorItem* item, int x) {
    int lineX = x - rangeLineHorizontalIndent;
    int lineWidth = rangeLineWidth();
    float aspect = static_cast<float>(lineX) / static_cast<float>(lineWidth);
    int value = static_cast<int>(aspect * static_cast<float>(maximalValue));

    int leftBorder = rangeLineHorizontalIndent - startIndicator->width / 2;
    int rightBorder = rangeLineHorizontalIndent + rangeLineWidth() + startIndicator->width / 2;

    bool isInStartRange = (x >= leftBorder) && (x <= (rangeLineHorizontalIndent + rangeLineWidth()));
    bool isInEndRange = (x >= (rangeLineHorizontalIndent + endIndicator->width / 2)) && (x <= rightBorder);

    if (item == startIndicator) {
        if (isInStartRange && x < endIndicator->getX()) {
            item->setX(x);
        }
    } else if (item == playbackIndicator) {
        bool canMovePlayback = (freeplayMode && (isInStartRange || isInEndRange)) ||
                               (!freeplayMode && (x > startIndicator->getRightBorder() && x < endIndicator->getX()));
        if (canMovePlayback) {
            item->setX(x);
        }
    } else if (item == endIndicator) {
        if (isInEndRange && x > startIndicator->getRightBorder()) {
            item->setX(x);
        }
    }

    draggingIndicator = item;

    if (startIndicator == item) {
        startValue = value;
        emit startValueChanged(value);
    } else if (playbackIndicator == item) {
        playbackValue = value;
        emit playbackValueChanged(value);
    } else if (endIndicator == item) {
        endValue = value;
        emit endValueChanged(value);
    }

    update();
}

void WorkspaceIndicator::resizeEvent([[maybe_unused]] QResizeEvent* event) {
    for (auto indicator : indicators) {
        indicator->update(event->size());
    }
}

void WorkspaceIndicator::highlightIndicatorIfNeeded([[maybe_unused]] int x, [[maybe_unused]] int y) {
    if (draggingIndicator) return;

    for (auto indicator : indicators) {
        indicator->setIsHighlighted(false);
    }

    for (auto indicator : indicators) {
        if (indicator->hitTest(x, y)) {
            indicator->setIsHighlighted(true);
            qDebug() << "highlight!!";
            break;
        }
    }
    update();
}

void WorkspaceIndicator::mouseReleaseEvent([[maybe_unused]] QMouseEvent* e) {
    cursorIsPressed = false;
    draggingIndicator = nullptr;
}

void WorkspaceIndicator::drawItem(WorkspaceIndicatorItem* item) {
    if (!item->getIsVisible()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::yellow, WorkspaceIndicatorItem::borderWidth));

    if (item->isHighlighted()) {
        painter.setBrush(Qt::red);
    } else {
        painter.setBrush(Qt::yellow);
    }

    if (item == startIndicator) {
        painter.setBrush(Qt::red);
    } else if (item == playbackIndicator) {
        painter.setBrush(Qt::green);
    } else if (item == endIndicator) {
        painter.setBrush(Qt::blue);
    }

    painter.drawRect(item->getRectangle());
}

int WorkspaceIndicator::rangeLineWidth() {
    return width() - rangeLineHorizontalIndent * 2;
}

void WorkspaceIndicator::paintEvent([[maybe_unused]] QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // DEBUG BACKGROUND
    painter.setBrush(Qt::green);
    painter.drawRect(QRect(0, 0, width(), height()));

    // Range line
    painter.setPen(QPen(Qt::red, rangeLineBorderHeight));
    painter.setBrush(Qt::blue);

    QRect rangeLineRectangle(rangeLineHorizontalIndent, height() / 2 - rangeLineHeight / 2, rangeLineWidth(), rangeLineHeight);
    painter.drawRect(rangeLineRectangle);

    for (auto indicator : indicators) {
        drawItem(indicator);
    }
}
