#include "project.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QDebug>
#include <QBuffer>
#include "../plugins/pluginmanager.h"

Project::Project(QObject *parent) :
    QObject(parent),
    m_name("Untitled"),
    m_path(""),
    m_isModified(false),
    m_engine(new Engine(this))
{
    // Connexion des signaux de l'engine
    connect(m_engine.get(), &Engine::effectChainChanged, this, [this]() {
        setModified(true);
    });

    connect(m_engine.get(), &Engine::processingFinished, this, [this](const QImage &) {
        // On pourrait faire quelque chose lorsque le traitement est terminé
    });
}

Project::~Project()
{
    // Le destructeur par défaut est suffisant car nous utilisons std::unique_ptr
}

bool Project::save(const QString &filename)
{
    QString targetFile = filename;

    // Si aucun fichier n'est spécifié, utiliser le chemin actuel
    if (targetFile.isEmpty()) {
        if (m_path.isEmpty()) {
            qWarning() << "No file path specified for saving";
            return false;
        }
        targetFile = m_path;
    }

    // Enregistrer dans notre format natif .vsd (VISIO Studio Document)
    bool success = saveToVSD(targetFile);

    if (success) {
        // Mettre à jour le chemin et le nom du projet
        m_path = targetFile;
        QFileInfo fileInfo(targetFile);
        m_name = fileInfo.baseName();

        m_isModified = false;

        emit projectSaved();
        emit projectPathChanged(m_path);
        emit projectNameChanged(m_name);
    }

    return success;
}

bool Project::load(const QString &filename)
{
    // Vérifier que le fichier existe
    if (!QFile::exists(filename)) {
        qWarning() << "File does not exist:" << filename;
        return false;
    }

    // Charger depuis notre format natif .vsd (VISIO Studio Document)
    bool success = loadFromVSD(filename);

    if (success) {
        // Mettre à jour le chemin et le nom du projet
        m_path = filename;
        QFileInfo fileInfo(filename);
        m_name = fileInfo.baseName();

        m_isModified = false;

        emit projectLoaded();
        emit projectPathChanged(m_path);
        emit projectNameChanged(m_name);
    }

    return success;
}

bool Project::exportImage(const QString &filename, const QString &format)
{
    // Traiter l'image avec tous les effets
    QImage result = m_engine->processImage();

    if (result.isNull()) {
        qWarning() << "Unable to export: processed image is null";
        return false;
    }

    // Enregistrer l'image dans le format demandé
    return result.save(filename, format.toUtf8().constData());
}

bool Project::newProject(int width, int height, const QColor &background)
{
    // Créer une nouvelle image vide avec les dimensions spécifiées
    QImage newImage(width, height, QImage::Format_ARGB32);
    newImage.fill(background);

    return setSourceImage(newImage);
}

bool Project::setSourceImage(const QImage &image)
{
    if (image.isNull()) {
        qWarning() << "Cannot set null image as source";
        return false;
    }

    // Définir l'image source
    m_engine->setSourceImage(image);

    // Marquer comme modifié
    setModified(true);

    return true;
}

bool Project::setSourceImage(const QString &imagePath)
{
    // Charger l'image depuis le fichier
    QImage image(imagePath);

    if (image.isNull()) {
        qWarning() << "Failed to load image from" << imagePath;
        return false;
    }

    return setSourceImage(image);
}

Engine* Project::engine() const
{
    return m_engine.get();
}

QString Project::name() const
{
    return m_name;
}

void Project::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        setModified(true);
        emit projectNameChanged(m_name);
    }
}

QString Project::path() const
{
    return m_path;
}

QSize Project::dimensions() const
{
    return m_engine->sourceImage().size();
}

bool Project::isModified() const
{
    return m_isModified;
}

void Project::setModified(bool modified)
{
    if (m_isModified != modified) {
        m_isModified = modified;
        emit projectModified();
    }
}

bool Project::canUndo() const
{
    // Sera implémenté avec l'historique plus tard
    return false;
}

bool Project::canRedo() const
{
    // Sera implémenté avec l'historique plus tard
    return false;
}

void Project::undo()
{
    // Sera implémenté avec l'historique plus tard
}

void Project::redo()
{
    // Sera implémenté avec l'historique plus tard
}

bool Project::saveToVSD(const QString &filename)
{
    // Structure du fichier VSD :
    // - Métadonnées du projet (nom, dimensions, etc.)
    // - Image source
    // - Chaîne d'effets (plugins et paramètres)

    QJsonObject projectData;

    // Métadonnées
    projectData["name"] = m_name;
    projectData["version"] = "1.0.0"; // Version du format de fichier

    QImage sourceImg = m_engine->sourceImage();
    QJsonObject imageData;
    imageData["width"] = sourceImg.width();
    imageData["height"] = sourceImg.height();
    imageData["format"] = sourceImg.format();

    // Enregistrer l'image source en base64
    QByteArray imageBytes;
    QBuffer buffer(&imageBytes);
    buffer.open(QIODevice::WriteOnly);
    sourceImg.save(&buffer, "PNG");
    imageData["data"] = QString(imageBytes.toBase64());

    projectData["sourceImage"] = imageData;

    // Chaîne d'effets
    QJsonArray effectsArray;
    for (const Engine::EffectNode &node : m_engine->effectChain()) {
        QJsonObject effectData;
        if (node.plugin) {
            effectData["id"] = node.id;
            effectData["pluginName"] = node.plugin->name();
            effectData["enabled"] = node.enabled;

            // Convertir les paramètres en JSON
            QJsonObject paramsObj;
            for (auto it = node.parameters.begin(); it != node.parameters.end(); ++it) {
                paramsObj[it.key()] = QJsonValue::fromVariant(it.value());
            }
            effectData["parameters"] = paramsObj;

            effectsArray.append(effectData);
        }
    }

    projectData["effectChain"] = effectsArray;

    // Enregistrer dans un fichier
    QJsonDocument doc(projectData);
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Could not open file for writing:" << filename;
        return false;
    }

    file.write(doc.toJson());
    file.close();

    return true;
}

bool Project::loadFromVSD(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file for reading:" << filename;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Invalid VSD file format";
        return false;
    }

    QJsonObject projectData = doc.object();

    // Charger les métadonnées
    if (projectData.contains("name")) {
        m_name = projectData["name"].toString();
    }

    // Charger l'image source
    if (projectData.contains("sourceImage") && projectData["sourceImage"].isObject()) {
        QJsonObject imageData = projectData["sourceImage"].toObject();
        QByteArray imageBytes = QByteArray::fromBase64(imageData["data"].toString().toLatin1());

        QImage sourceImg;
        if (!sourceImg.loadFromData(imageBytes, "PNG")) {
            qWarning() << "Failed to load source image data";
            return false;
        }

        // Définir l'image source sans marquer comme modifié
        m_engine->setSourceImage(sourceImg);
    } else {
        qWarning() << "No source image found in project file";
        return false;
    }

    // Charger la chaîne d'effets
    if (projectData.contains("effectChain") && projectData["effectChain"].isArray()) {
        QJsonArray effectsArray = projectData["effectChain"].toArray();

        // D'abord, effacer la chaîne d'effets existante
        QList<Engine::EffectNode> currentChain = m_engine->effectChain();
        for (const Engine::EffectNode &node : currentChain) {
            m_engine->removeEffect(node.id);
        }

        // Ensuite, charger les nouveaux effets
        PluginManager *pluginManager = PluginManager::instance();

        for (const QJsonValue &val : effectsArray) {
            if (!val.isObject()) continue;

            QJsonObject effectData = val.toObject();
            QString pluginName = effectData["pluginName"].toString();
            bool enabled = effectData["enabled"].toBool(true);

            // Trouver le plugin correspondant
            PluginInterface *plugin = pluginManager->pluginByName(pluginName);
            if (!plugin) {
                qWarning() << "Plugin not found:" << pluginName;
                continue;
            }

            // Convertir les paramètres depuis JSON
            QVariantMap parameters;
            if (effectData.contains("parameters") && effectData["parameters"].isObject()) {
                QJsonObject paramsObj = effectData["parameters"].toObject();
                for (auto it = paramsObj.begin(); it != paramsObj.end(); ++it) {
                    parameters[it.key()] = it.value().toVariant();
                }
            } else {
                parameters = plugin->defaultParameters();
            }

            // Ajouter l'effet à la chaîne
            int id = m_engine->addEffect(plugin, parameters);
            if (id >= 0) {
                m_engine->enableEffect(id, enabled);
            }
        }
    }

    return true;
}
