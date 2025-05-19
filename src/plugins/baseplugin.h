#ifndef BASEPLUGIN_H
#define BASEPLUGIN_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QWidget>
#include <QVariantMap>
#include <QIcon>
#include "plugininterface.h"

/**
 * @brief Classe de base pour simplifier la création de plugins
 *
 * Cette classe implémente certaines fonctionnalités communes
 * pour réduire la duplication de code dans les plugins.
 */
class BasePlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

public:
    explicit BasePlugin(QObject *parent = nullptr);
    virtual ~BasePlugin();

    // Implémentations par défaut pour les méthodes de PluginInterface
    QString version() const override;
    QString author() const override;
    QList<QPair<QString, QVariantMap>> presets() const override;

    // Les méthodes suivantes doivent toujours être implémentées par les plugins dérivés:
    // - QString name() const
    // - QString description() const
    // - QString category() const
    // - QImage processImage(const QImage &image, const QVariantMap &parameters)
    // - QWidget* createControlPanel()
    // - QVariantMap defaultParameters() const

protected:
    // Méthodes utilitaires pour les plugins dérivés
    QImage convertToGrayscale(const QImage &image) const;
    QImage applyThreshold(const QImage &image, int threshold) const;

    // Préréglages (presets) stockés
    QList<QPair<QString, QVariantMap>> m_presets;
};

#endif // BASEPLUGIN_H
