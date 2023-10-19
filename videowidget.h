#pragma once

#include <QVideoWidget>
#include <QObject>
#include <QEvent>
#include <QUrl>


class VideoWidget: public QVideoWidget
{
    Q_OBJECT

public:
    VideoWidget(QWidget *parent);

signals:
    void dragDidDropUrl(QUrl url);

private:
    bool eventFilter(QObject *object, QEvent *event);
    void handleDragDidDrop(QDropEvent *event);
};
