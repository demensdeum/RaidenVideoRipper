#include "utils.h"

namespace RaidenVideoRipper {
namespace Utils {

bool isApplicationInDarkMode() {
    return QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
}

QIcon tintIconColors(const QIcon& originalIcon, const QColor& tintColor) {
    if (originalIcon.isNull()) {
        return QIcon();
    }
    QIcon tintedIcon;
    const QList<QSize>& sizes = originalIcon.availableSizes();
    for (const QSize& size : sizes) {
        for (QIcon::Mode mode : {QIcon::Normal, QIcon::Disabled, QIcon::Active, QIcon::Selected}) {
            for (QIcon::State state : {QIcon::Off, QIcon::On}) {
                QPixmap originalPixmap = originalIcon.pixmap(size, mode, state);
                if (!originalPixmap.isNull()) {
                    QImage image = originalPixmap.toImage();
                    for (int y = 0; y < image.height(); ++y) {
                        for (int x = 0; x < image.width(); ++x) {
                            QRgb pixel = image.pixel(x, y);
                            int a_orig = qAlpha(pixel);

                            if (a_orig == 0) {
                                continue;
                            }

                            QColor originalPixelColor(qRed(pixel), qGreen(pixel), qBlue(pixel));

                            int originalLightness = originalPixelColor.lightness();
                            int newLightness = qBound(0, originalLightness + 125, 255);

                            QColor tintedPixel;
                            tintedPixel.setHsl(tintColor.hue(), tintColor.saturation(), newLightness, a_orig);

                            image.setPixel(x, y, tintedPixel.rgb());
                        }
                    }
                    tintedIcon.addPixmap(QPixmap::fromImage(image), mode, state);
                }
            }
        }
    }
    return tintedIcon;
}

QImage tintImageColors(const QImage& originalImage, const QColor& tintColor) {
    if (originalImage.isNull()) {
        return QImage();
    }
    QImage tintedImage = originalImage.copy();

    for (int y = 0; y < tintedImage.height(); ++y) {
        for (int x = 0; x < tintedImage.width(); ++x) {
            QRgb pixel = tintedImage.pixel(x, y);
            int a_orig = qAlpha(pixel);

            if (a_orig == 0) {
                continue;
            }

            QColor originalPixelColor(qRed(pixel), qGreen(pixel), qBlue(pixel));

            int originalLightness = originalPixelColor.lightness();
            int newLightness = qBound(0, originalLightness + 125, 255); // Increase brightness by 125

            QColor tintedPixel;
            tintedPixel.setHsl(tintColor.hue(), tintColor.saturation(), newLightness, a_orig);

            tintedImage.setPixel(x, y, tintedPixel.rgb());
        }
    }
    return tintedImage;
}

QIcon themeStyledIcon(const QString& themeName, const QIcon& fallback) {
    QIcon baseIcon;
    baseIcon = QIcon::fromTheme(themeName, fallback);
    if (isApplicationInDarkMode()) {
        baseIcon = tintIconColors(baseIcon, QColor::fromRgb(0x007ad9));
    }
    return baseIcon;
}

QImage themeStyledImage(const char *originalImageName) {
    QImage originalImage = QImage(originalImageName);
    if (isApplicationInDarkMode()) {
        return tintImageColors(originalImage, QColor::fromRgb(0x007ad9));
    }
    return originalImage;
}


QString capitalized(const QString& text) {
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
