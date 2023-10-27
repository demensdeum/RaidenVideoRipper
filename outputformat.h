#pragma once

#include <QObject>
#include <QString>

struct OutputFormat
{
    QString identifier;
    QString title;
    QString extension;
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

    OutputFormat(const OutputFormat& other)
        : identifier(other.identifier),
        title(other.title),
        extension(other.extension),
        isSelected(other.isSelected)
    {
    }

    OutputFormat& operator=(const OutputFormat& other)
    {
        if (this != &other)
        {
            identifier = other.identifier;
            title = other.title;
            extension = other.extension;
            isSelected = other.isSelected;
        }
        return *this;
    }
};
