#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QtPlugin>
#include <QString>
#include <QImage>
#include <QWidget>
#include <QVariantMap>
#include <QIcon>

/**
 * @brief Interface que tous les plugins VISIO STUDIO doivent implémenter
 *
 * Cette interface définit le contrat entre le moteur central et les plugins.
 * Chaque plugin doit fournir des métadonnées, des méthodes de traitement d'image,
 * et créer son propre panneau de contrôle UI.
 */
class PluginInterface
{
public:
    virtual ~PluginInterface() {}

    // Métadonnées du plugin
    virtual QString name() const = 0;          // Nom du plugin
    virtual QString version() const = 0;       // Version (format: x.y.z)
    virtual QString description() const = 0;   // Description brève
    virtual QString author() const = 0;        // Auteur/créateur

    // Catégorie (dithering, artistic, basic, etc.)
    virtual QString category() const = 0;

    // Méthodes principales pour traiter une image
    virtual QImage processImage(const QImage &image, const QVariantMap &parameters) = 0;

    // UI Component - Panneau de contrôle spécifique au plugin
    virtual QWidget* createControlPanel() = 0;

    // Préréglages (presets)
    virtual QVariantMap defaultParameters() const = 0;
    virtual QList<QPair<QString, QVariantMap>> presets() const = 0;

    // Icône du plugin (optionnel - utilisera une icône par défaut si non implémenté)
    virtual QIcon icon() const { return QIcon(); }

    // Méthode pour identifier si le plugin est compatible avec le type d'image
    virtual bool isCompatibleWith(const QImage &image) const { return true; }
};

// Définir l'ID unique d'interface pour le système de plugins Qt
#define PluginInterface_iid "com.visiostudio.PluginInterface"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

#endif // PLUGININTERFACE_H
