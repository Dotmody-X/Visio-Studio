#include "pluginmanager.h"
#include <QPluginLoader>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

// Inclusion des plugins intégrés
#include "../effects/dithering/dithering.h"
#include "../effects/basic/basic.h"
#include "../effects/glitch/glitch.h"

// Initialisation du singleton
PluginManager* PluginManager::m_instance = nullptr;

PluginManager* PluginManager::instance()
{
    if (!m_instance) {
        m_instance = new PluginManager();
    }
    return m_instance;
}

PluginManager::PluginManager(QObject *parent) : QObject(parent)
{
    // Initialiser les dossiers de plugins par défaut
    QString appDir = QCoreApplication::applicationDirPath();
    m_pluginDirectories << appDir + "/plugins";

// Sur macOS, rechercher aussi dans le bundle
#ifdef Q_OS_MAC
    m_pluginDirectories << appDir + "/../PlugIns";
#endif

    // Dossier de plugins dans les documents utilisateur
    QString userPluginsDir = QDir::homePath() + "/Documents/VISIO_STUDIO/plugins";
    m_pluginDirectories << userPluginsDir;
}

PluginManager::~PluginManager()
{
    // Nettoyage si nécessaire
}

void PluginManager::loadPlugins()
{
    // Vider les collections précédentes
    m_plugins.clear();
    m_pluginsByCategory.clear();
    m_pluginsByName.clear();

    // Charger d'abord les plugins intégrés
    loadBuiltinPlugins();

    // Puis scanner les dossiers de plugins
    for (const QString &dir : m_pluginDirectories) {
        scanDirectory(QDir(dir));
    }

    emit pluginsLoaded();
}

QList<PluginInterface*> PluginManager::allPlugins() const
{
    return m_plugins;
}

QList<PluginInterface*> PluginManager::pluginsByCategory(const QString &category) const
{
    return m_pluginsByCategory.value(category, QList<PluginInterface*>());
}

PluginInterface* PluginManager::pluginByName(const QString &name) const
{
    return m_pluginsByName.value(name, nullptr);
}

void PluginManager::addPluginDirectory(const QString &directory)
{
    if (!m_pluginDirectories.contains(directory)) {
        m_pluginDirectories << directory;
    }
}

QStringList PluginManager::pluginDirectories() const
{
    return m_pluginDirectories;
}

void PluginManager::scanDirectory(const QDir &dir)
{
    if (!dir.exists()) {
        return;
    }

    qDebug() << "Scanning for plugins in" << dir.absolutePath();

    // Trouver tous les fichiers de plugin
    QStringList filters;

#ifdef Q_OS_WIN
    filters << "*.dll";
#elif defined(Q_OS_MAC)
    filters << "*.dylib";
#else
    filters << "*.so";
#endif

    // Créer une copie non-const du QDir
    QDir mutableDir(dir);
    mutableDir.setNameFilters(filters);

    // Essayer de charger chaque fichier comme un plugin
    for (const QString &fileName : mutableDir.entryList(QDir::Files)) {
        QString filePath = dir.absoluteFilePath(fileName);
        QPluginLoader loader(filePath);

        // Vérifier si c'est un plugin valide
        if (QObject *pluginInstance = loader.instance()) {
            // Essayer de convertir en PluginInterface
            PluginInterface *plugin = qobject_cast<PluginInterface*>(pluginInstance);
            if (plugin) {
                qDebug() << "Loaded plugin:" << plugin->name() << "(" << plugin->version() << ")";

                // Stocker le plugin
                m_plugins.append(plugin);
                m_pluginsByName[plugin->name()] = plugin;

                // Ajouter à la catégorie
                QString category = plugin->category();
                if (!m_pluginsByCategory.contains(category)) {
                    m_pluginsByCategory[category] = QList<PluginInterface*>();
                }
                m_pluginsByCategory[category].append(plugin);

                emit pluginLoaded(plugin);
            } else {
                // Ce n'est pas un plugin VISIO STUDIO valide
                loader.unload();
                qDebug() << "Invalid plugin format:" << filePath;
            }
        } else {
            qDebug() << "Failed to load plugin:" << filePath << "-" << loader.errorString();
        }
    }

    // Rechercher récursivement dans les sous-dossiers
    for (const QString &subDir : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        scanDirectory(QDir(dir.absoluteFilePath(subDir)));
    }
}

void PluginManager::loadBuiltinPlugins()
{
    qDebug() << "Loading built-in plugins...";

    // Créer les plugins intégrés
    // Note: Nous incluons les fichiers d'en-tête en dehors de cette fonction
    // pour éviter des problèmes de compilation
    DitheringPlugin *ditheringPlugin = new DitheringPlugin(this);
    registerBuiltinPlugin(ditheringPlugin);

    BasicPlugin *basicPlugin = new BasicPlugin(this);
    registerBuiltinPlugin(basicPlugin);

    // Ajouter le nouveau plugin Glitch
    GlitchPlugin *glitchPlugin = new GlitchPlugin(this);
    registerBuiltinPlugin(glitchPlugin);
}

void PluginManager::registerBuiltinPlugin(PluginInterface* plugin)
{
    if (!plugin) return;

    qDebug() << "Registering built-in plugin:" << plugin->name() << "(" << plugin->version() << ")";

    // Stocker le plugin
    m_plugins.append(plugin);
    m_pluginsByName[plugin->name()] = plugin;

    // Ajouter à la catégorie
    QString category = plugin->category();
    if (!m_pluginsByCategory.contains(category)) {
        m_pluginsByCategory[category] = QList<PluginInterface*>();
    }
    m_pluginsByCategory[category].append(plugin);

    emit pluginLoaded(plugin);
}
