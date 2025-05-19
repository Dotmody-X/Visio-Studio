#include "engine.h"
#include <QDebug>
#include <QtConcurrent>

Engine::Engine(QObject *parent) : QObject(parent), m_nextEffectId(0)
{
    // Initialisation du moteur
}

Engine::~Engine()
{
    // Nettoyage si nécessaire
}

void Engine::setSourceImage(const QImage &image)
{
    m_sourceImage = image;
    // Réinitialiser l'image traitée
    m_processedImage = QImage();

    // Informer que la chaîne d'effets a changé (car l'image source a changé)
    emit effectChainChanged();
}

void Engine::processImageAsync()
{
    if (m_sourceImage.isNull()) {
        qWarning() << "Attempt to process a null source image";
        emit processingFailed("Source image is null");
        return;
    }

    // Traitement synchrone pour le moment (vous pourrez le rendre asynchrone plus tard)
    QImage result = processImage();

    // Signaler la fin du traitement
    emit processingFinished(result);
}

void Engine::handleProcessingFinished()
{
    // Rien à faire pour le moment, mais cette méthode doit exister
}

void Engine::handleProcessingCanceled()
{
    // Rien à faire pour le moment, mais cette méthode doit exister
}

QImage Engine::processedImage() const
{
    return m_processedImage;
}

QImage Engine::sourceImage() const
{
    return m_sourceImage;
}

int Engine::addEffect(PluginInterface *plugin, const QVariantMap &parameters)
{
    if (!plugin) {
        qWarning() << "Attempt to add null plugin to effect chain";
        return -1;
    }

    // Créer un nœud d'effet avec un ID unique
    EffectNode node(plugin, parameters);
    node.id = generateEffectId();

    // Si aucun paramètre n'est fourni, utiliser les paramètres par défaut du plugin
    if (parameters.isEmpty()) {
        node.parameters = plugin->defaultParameters();
    }

    // Ajouter à la fin de la chaîne
    m_effectChain.append(node);

    // Émettre des signaux
    emit effectAdded(node.id);
    emit effectChainChanged();

    return node.id;
}

bool Engine::removeEffect(int id)
{
    int index = indexFromId(id);
    if (index < 0) {
        qWarning() << "Attempt to remove non-existent effect with id" << id;
        return false;
    }

    m_effectChain.removeAt(index);

    // Émettre des signaux
    emit effectRemoved(id);
    emit effectChainChanged();

    return true;
}

bool Engine::moveEffect(int id, int newPosition)
{
    int currentIndex = indexFromId(id);
    if (currentIndex < 0) {
        qWarning() << "Attempt to move non-existent effect with id" << id;
        return false;
    }

    if (newPosition < 0 || newPosition >= m_effectChain.size()) {
        qWarning() << "Invalid new position for effect" << newPosition;
        return false;
    }

    // Ne rien faire si on demande de déplacer à la même position
    if (currentIndex == newPosition) {
        return true;
    }

    // Déplacer l'effet
    m_effectChain.move(currentIndex, newPosition);

    // Émettre des signaux
    emit effectMoved(id, newPosition);
    emit effectChainChanged();

    return true;
}

bool Engine::updateEffectParameters(int id, const QVariantMap &parameters)
{
    int index = indexFromId(id);
    if (index < 0) {
        qWarning() << "Attempt to update parameters of non-existent effect with id" << id;
        return false;
    }

    // Mettre à jour les paramètres
    m_effectChain[index].parameters = parameters;

    // Émettre des signaux
    emit effectParametersChanged(id);
    emit effectChainChanged();

    return true;
}

bool Engine::enableEffect(int id, bool enabled)
{
    int index = indexFromId(id);
    if (index < 0) {
        qWarning() << "Attempt to enable/disable non-existent effect with id" << id;
        return false;
    }

    // Ne rien faire si l'état est déjà celui demandé
    if (m_effectChain[index].enabled == enabled) {
        return true;
    }

    // Mettre à jour l'état d'activation
    m_effectChain[index].enabled = enabled;

    // Émettre des signaux
    emit effectEnabledChanged(id, enabled);
    emit effectChainChanged();

    return true;
}

QList<Engine::EffectNode> Engine::effectChain() const
{
    return m_effectChain;
}

Engine::EffectNode Engine::effectAt(int position) const
{
    if (position < 0 || position >= m_effectChain.size()) {
        qWarning() << "Attempt to access effect at invalid position" << position;
        return EffectNode();
    }

    return m_effectChain.at(position);
}

Engine::EffectNode Engine::effectById(int id) const
{
    int index = indexFromId(id);
    if (index < 0) {
        qWarning() << "Attempt to access non-existent effect with id" << id;
        return EffectNode();
    }

    return m_effectChain.at(index);
}

int Engine::effectCount() const
{
    return m_effectChain.size();
}

QImage Engine::processImage()
{
    if (m_sourceImage.isNull()) {
        qWarning() << "Attempt to process a null source image";
        return QImage();
    }

    // Signaler le début du traitement
    emit processingStarted();

    // Image de travail, initialement l'image source
    QImage result = m_sourceImage;

    // Nombre total d'effets pour le calcul de progression
    int totalEffects = m_effectChain.size();
    int processedEffects = 0;

    // Appliquer chaque effet dans l'ordre
    for (const EffectNode &node : m_effectChain) {
        // Ignorer les effets désactivés
        if (!node.enabled) {
            processedEffects++;
            emit processingProgress((processedEffects * 100) / totalEffects);
            continue;
        }

        // Appliquer l'effet si le plugin est valide
        if (node.plugin) {
            result = node.plugin->processImage(result, node.parameters);
        }

        // Mettre à jour la progression
        processedEffects++;
        emit processingProgress((processedEffects * 100) / totalEffects);
    }

    // Stocker le résultat final
    m_processedImage = result;

    // Signaler la fin du traitement
    emit processingFinished(m_processedImage);

    return m_processedImage;
}

QImage Engine::previewEffect(int id)
{
    if (m_sourceImage.isNull()) {
        qWarning() << "Attempt to preview with a null source image";
        return QImage();
    }

    // Trouver l'effet
    int index = indexFromId(id);
    if (index < 0) {
        qWarning() << "Attempt to preview non-existent effect with id" << id;
        return m_sourceImage;
    }

    // Image de travail, initialement l'image source
    QImage result = m_sourceImage;

    // Appliquer tous les effets jusqu'à celui demandé
    for (int i = 0; i <= index; ++i) {
        const EffectNode &node = m_effectChain.at(i);

        // Ignorer les effets désactivés
        if (!node.enabled) {
            continue;
        }

        // Appliquer l'effet si le plugin est valide
        if (node.plugin) {
            result = node.plugin->processImage(result, node.parameters);
        }
    }

    return result;
}

int Engine::generateEffectId()
{
    return m_nextEffectId++;
}

int Engine::indexFromId(int id) const
{
    for (int i = 0; i < m_effectChain.size(); ++i) {
        if (m_effectChain.at(i).id == id) {
            return i;
        }
    }

    return -1; // Non trouvé
}
