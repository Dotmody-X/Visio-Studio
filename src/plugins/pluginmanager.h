#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QDir>
#include "plugininterface.h"

/**
 * @brief Gestionnaire des plugins VISIO STUDIO
 *
 * Cette classe est responsable de découvrir, charger, et gérer
 * tous les plugins disponibles pour l'application.
 */
class PluginManager : public QObject
{
    Q_OBJECT

public:
    static PluginManager* instance();
    ~PluginManager();

    // Chargement des plugins
    void loadPlugins();

    // Accès aux plugins
    QList<PluginInterface*> allPlugins() const;
    QList<PluginInterface*> pluginsByCategory(const QString &category) const;
    PluginInterface* pluginByName(const QString &name) const;

    // Gestion des dossiers de plugins
    void addPluginDirectory(const QString &directory);
    QStringList pluginDirectories() const;

signals:
    void pluginsLoaded();
    void pluginLoaded(PluginInterface *plugin);

private:
    explicit PluginManager(QObject *parent = nullptr);

    static PluginManager* m_instance;

    QList<PluginInterface*> m_plugins;
    QMap<QString, QList<PluginInterface*>> m_pluginsByCategory;
    QMap<QString, PluginInterface*> m_pluginsByName;
    QStringList m_pluginDirectories;

    // Méthodes internes
    void scanDirectory(const QDir &dir);
    void loadBuiltinPlugins();
    void registerBuiltinPlugin(PluginInterface* plugin);
};

#endif // PLUGINMANAGER_H
