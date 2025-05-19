#ifndef BASIC_H
#define BASIC_H

#include <QObject>
#include "../../plugins/baseplugin.h"

/**
 * @brief Plugin pour les effets de base
 *
 * Ce plugin implémente des effets fondamentaux comme
 * le contraste, la luminosité, la saturation, etc.
 */
class BasicPlugin : public BasePlugin
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

public:
    enum EffectType {
        Brightness,     // Ajustement de la luminosité
        Contrast,       // Ajustement du contraste
        Saturation,     // Ajustement de la saturation
        Invert          // Inversion des couleurs
    };

    explicit BasicPlugin(QObject *parent = nullptr);

    // Implémentation de l'interface PluginInterface
    QString name() const override;
    QString description() const override;
    QString category() const override;
    QImage processImage(const QImage &image, const QVariantMap &parameters) override;
    QWidget* createControlPanel() override;
    QVariantMap defaultParameters() const override;

private:
    // Méthodes pour les différents types d'effets
    QImage applyBrightness(const QImage &image, int brightness) const;
    QImage applyContrast(const QImage &image, int contrast) const;
    QImage applySaturation(const QImage &image, int saturation) const;
    QImage applyInvert(const QImage &image) const;
};

#endif // BASIC_H
