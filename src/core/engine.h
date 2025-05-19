#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QImage>
#include <QList>
#include <QVariantMap>
#include <QFutureWatcher>
#include "../plugins/plugininterface.h"

/**
 * @brief Moteur central de traitement d'image
 *
 * Cette classe gère la chaîne d'effets et le traitement de l'image.
 * Elle s'inspire du flux de travail de FL Studio pour appliquer les
 * effets en série ou en parallèle.
 */
class Engine : public QObject
{
    Q_OBJECT

public:
    // Structure pour représenter un effet dans la chaîne
    struct EffectNode {
        PluginInterface *plugin;
        QVariantMap parameters;
        bool enabled;
        int id; // Identifiant unique pour cet effet dans la chaîne

        EffectNode() : plugin(nullptr), parameters(), enabled(true), id(-1) {}

        EffectNode(PluginInterface *p, const QVariantMap &params, bool en = true)
            : plugin(p), parameters(params), enabled(en), id(-1) {}
    };

    explicit Engine(QObject *parent = nullptr);
    ~Engine();

    // Gestion de l'image source
    void setSourceImage(const QImage &image);
    QImage sourceImage() const;

    // Gestion de la chaîne d'effets
    int addEffect(PluginInterface *plugin, const QVariantMap &parameters = QVariantMap());
    bool removeEffect(int id);
    bool moveEffect(int id, int newPosition);
    bool updateEffectParameters(int id, const QVariantMap &parameters);
    bool enableEffect(int id, bool enabled);

    // Accès aux effets
    QList<EffectNode> effectChain() const;
    EffectNode effectAt(int position) const;
    EffectNode effectById(int id) const;
    int effectCount() const;

    // État du traitement
    bool isProcessing() const;
    QImage processedImage() const;

    // Traitement et rendu
    QImage processImage();           // Méthode synchrone
    void processImageAsync();        // Méthode asynchrone
    void cancelProcessing();         // Annuler le traitement en cours
    QImage previewEffect(int id);    // Prévisualiser un effet spécifique

signals:
    void processingStarted();
    void processingFinished(const QImage &result);
    void processingCanceled();
    void processingFailed(const QString &errorMessage);
    void processingProgress(int percent);

    void effectChainChanged();
    void effectAdded(int id);
    void effectRemoved(int id);
    void effectMoved(int id, int newPosition);
    void effectParametersChanged(int id);
    void effectEnabledChanged(int id, bool enabled);

private:
    QImage m_sourceImage;
    QImage m_processedImage;
    QList<EffectNode> m_effectChain;
    int m_nextEffectId;

    // Traitement asynchrone
    QFutureWatcher<QImage> m_processingWatcher;
    bool m_isProcessing;

    // Méthodes internes
    int generateEffectId();
    int indexFromId(int id) const;
    QImage processImageInternal(const QImage &source, const QList<EffectNode> &chain);

private slots:
    void handleProcessingFinished();
    void handleProcessingCanceled();
};

#endif // ENGINE_H
