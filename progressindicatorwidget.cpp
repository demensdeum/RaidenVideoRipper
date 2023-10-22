#include "progressindicatorwidget.h"
#include <QPainter>
#include <QPaintEvent>

ProgressIndicatorWidget::ProgressIndicatorWidget(QWidget* parent) : QWidget(parent)
{
    debug = true;
}

void ProgressIndicatorWidget::resizeEvent(QResizeEvent *event)
{
    update();
    qDebug() << event;
}

void ProgressIndicatorWidget::paintEvent([[maybe_unused]]QPaintEvent *event)
{
    if (debug) {
        auto rectangle = QRect(10, 0, 500, height());
        auto painter = QPainter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, QPainter::Antialiasing);
        painter.setBrush(Qt::red);
        painter.drawRect(rectangle);
    }
}
