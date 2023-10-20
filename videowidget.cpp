#include "videowidget.h"
#include <QEvent>
#include <QDropEvent>
#include <QMimeData>

VideoWidget::VideoWidget(QWidget *parent) : QVideoWidget(parent)
{
    QObject *c = findChild<QWidget*>();
    c->installEventFilter(this);
}

// Qt 6 bug: https://bugreports.qt.io/browse/QTBUG-107668

bool VideoWidget::eventFilter(QObject *object, QEvent *event)
{
    QObject *child = findChild<QWidget*>();

    if(object == child){
        if(event->type() == QEvent::DragEnter)
        {
            event->accept();
            return true;
        }
        else if(event->type() == QEvent::DragMove)
        {
            event->accept();
            return true;
        }
        else if(event->type() == QEvent::Drop)
        {
            handleDragDidDrop(static_cast<QDropEvent *>(event));
            event->accept();
            return true;
        }
    }
    return QVideoWidget::eventFilter(object, event);
}

void VideoWidget::handleDragDidDrop(QDropEvent *event) {
    auto mimeData = event->mimeData();
    if (!mimeData) {
        return;
    }
    if (!mimeData->hasUrls()) {
        return;
    }
    if (mimeData->urls().count() < 1) {
        return;
    }
    auto url = mimeData->urls().at(0);
    emit dragDidDropUrl(url);
}
