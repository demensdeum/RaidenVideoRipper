#pragma once

#include <QString>

struct OutputFormat
{
    const QString identifier;
    const QString title;
    const QString extension;
    bool isSelected;

    OutputFormat(
        const QString& identifier,
        const QString& title,
        const QString& extension,
        bool isSelected
        )
        : identifier(identifier),
        title(title),
        extension(extension),
        isSelected(isSelected)
    {
    }
};
