#pragma once

#include <QString>
#include <QStringList>
#include <QDebug>
#include <queue>

namespace RaidenVideoRipper {
namespace Utils {

template <typename T>
void clearQueue(std::queue<T>& queue)
{
    while (!queue.empty()) {
        queue.pop();
    }
}

QString capitalized(const QString& text)
{
    auto parts = text.split("");
    auto size = parts.size();
    if (size < 1) {
        return text;
    }
    parts[1] = parts[1].toUpper();
    return parts.join("");
}
}
}

