#ifndef GLITCH_H
#define GLITCH_H

#include <QObject>
#include "../../plugins/baseplugin.h"

/**
 * @brief Plugin pour les effets de glitch
 *
 * Ce plugin implémente différentes techniques de glitch
 * pour créer des effets esthétiques inspirés des erreurs numériques.
 */
class GlitchPlugin : public BasePlugin
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

public:
    enum GlitchType {
        RGBShift,       // Décalage des canaux RGB
        BlockShift,     // Décalage de blocs d'image
        PixelSort,      // Tri de pixels
        Corruption,     // Corruption de données aléatoire
        ScanLines       // Lignes de balayage type VHS
    };

    explicit GlitchPlugin(QObject *parent = nullptr);

    // Implémentation de l'interface PluginInterface
    QString name() const override;
    QString description() const override;
    QString category() const override;
    QImage processImage(const QImage &image, const QVariantMap &parameters) override;
    QWidget* createControlPanel() override;
    QVariantMap defaultParameters() const override;

private:
    // Méthodes pour les différents types de glitch
    QImage applyRGBShift(const QImage &image, int xShift, int yShift) const;
    QImage applyBlockShift(const QImage &image, int blockSize, int maxOffset, int chance) const;
    QImage applyPixelSort(const QImage &image, int threshold, int sortLength) const;
    QImage applyCorruption(const QImage &image, int intensity, int seed) const;
    QImage applyScanLines(const QImage &image, int lineThickness, int lineSpacing, int opacity) const;
};

#endif // GLITCH_H
