#pragma once

#include <QString>
#include <QStringList>
#include <QDebug>

namespace RaidenVideoRipper {
namespace Utils {
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

