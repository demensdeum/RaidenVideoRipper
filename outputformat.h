#pragma once

#include <QString>

struct OutputFormat
{
    const QString identifier;
    const QString title;
    const QString extension;
    const std::vector<QString> customArguments;
    bool isSelected;

    OutputFormat(
        const QString& identifier,
        const QString& title,
        const QString& extension,
        const std::vector<QString> customArguments,
        bool isSelected
        )
        : identifier(identifier),
        title(title),
        extension(extension),
        customArguments(customArguments),
        isSelected(isSelected)
    {
    }
};
