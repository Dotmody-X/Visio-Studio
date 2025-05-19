#ifndef DITHERING_H
#define DITHERING_H

#include <QObject>
#include "../../plugins/baseplugin.h"

/**
 * @brief Plugin pour les effets de dithering
 *
 * Ce plugin implemente différentes techniques de dithering
 * pour créer des effets esthétiques rétro et lo-fi.
 */
class DitheringPlugin : public BasePlugin
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

public:
    enum DitheringType {
        Threshold,      // Conversion binaire simple avec seuil
        Random,         // Dithering avec bruit aléatoire
        Bayer,          // Dithering ordonné avec matrice de Bayer
        FloydSteinberg  // Dithering avec diffusion d'erreur
    };

    explicit DitheringPlugin(QObject *parent = nullptr);

    // Implémentation de l'interface PluginInterface
    QString name() const override;
    QString description() const override;
    QString category() const override;
    QImage processImage(const QImage &image, const QVariantMap &parameters) override;
    QWidget* createControlPanel() override;
    QVariantMap defaultParameters() const override;

private:
    // Méthodes pour les différents types de dithering
    QImage applyThresholdDithering(const QImage &image, int threshold) const;
    QImage applyRandomDithering(const QImage &image, int threshold, int intensity) const;
    QImage applyBayerDithering(const QImage &image, int threshold, int matrixSize) const;
    QImage applyFloydSteinbergDithering(const QImage &image, int threshold) const;

    // Génération de la matrice de Bayer
    QVector<QVector<int>> generateBayerMatrix(int size) const;
};

#endif // DITHERING_H
