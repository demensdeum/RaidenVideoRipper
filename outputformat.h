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

    OutputFormat& operator=(OutputFormat&& other) noexcept
    {
        if (this != &other)
        {
            identifier = std::move(other.identifier);
            title = std::move(other.title);
            extension = std::move(other.extension);
            isSelected = other.isSelected;
        }
        return *this;
    }
};
