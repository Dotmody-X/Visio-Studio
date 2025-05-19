#include "baseplugin.h"
#include <QColor>

BasePlugin::BasePlugin(QObject *parent) : QObject(parent)
{
    // Initialisation par défaut
}

BasePlugin::~BasePlugin()
{
    // Nettoyage si nécessaire
}

QString BasePlugin::version() const
{
    // Par défaut, version 1.0.0
    return "1.0.0";
}

QString BasePlugin::author() const
{
    // Auteur par défaut
    return "VISIO STUDIO Team";
}

QList<QPair<QString, QVariantMap>> BasePlugin::presets() const
{
    // Retourne les préréglages définis
    return m_presets;
}

QImage BasePlugin::convertToGrayscale(const QImage &image) const
{
    // Fonction utilitaire pour convertir une image en niveaux de gris
    if (image.isNull()) {
        return QImage();
    }

    QImage result = image.convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < result.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(result.scanLine(y));

        for (int x = 0; x < result.width(); ++x) {
            int gray = qGray(line[x]);
            line[x] = qRgba(gray, gray, gray, qAlpha(line[x]));
        }
    }

    return result;
}

QImage BasePlugin::applyThreshold(const QImage &image, int threshold) const
{
    // Fonction utilitaire pour appliquer un seuil à une image
    if (image.isNull()) {
        return QImage();
    }

    QImage result = image.convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < result.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(result.scanLine(y));

        for (int x = 0; x < result.width(); ++x) {
            int gray = qGray(line[x]);
            int value = (gray < threshold) ? 0 : 255;
            line[x] = qRgba(value, value, value, qAlpha(line[x]));
        }
    }

    return result;
}
