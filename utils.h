#pragma once

#include <QString>
#include <QStringList>
#include <QDebug>
#include <queue>
#include <QGuiApplication>
#include <QIcon>
#include <QStyleHints>
#include <QStyle>

namespace RaidenVideoRipper {
namespace Utils {

template <typename T>
void clearQueue(std::queue<T>& queue)
{
    while (!queue.empty()) {
        queue.pop();
    }
}

bool isApplicationInDarkMode();
QIcon tintIconColors(const QIcon& originalIcon, const QColor& tintColor);
QImage tintImageColors(const QImage& originalImage, const QColor& tintColor);
QIcon themeStyledIcon(const QString& themeName, const QIcon& fallback);
QImage themeStyledImage(const char *originalImageName);
QString capitalized(const QString& text);

}
}

