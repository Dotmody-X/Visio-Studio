#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <QImage>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVariantMap>
#include <QThread>

/**
 * @brief Générateur d'images par IA
 *
 * Cette classe implémente la génération d'images guidée par texte,
 * en utilisant soit un modèle local léger, soit une API externe.
 */
class Generator : public QObject
{
    Q_OBJECT

public:
    explicit Generator(QObject *parent = nullptr);
    ~Generator();

    // Générer une image basée sur une description
    void generateImage(const QString &prompt, int width = 512, int height = 512);

    // Définir si on utilise l'API locale ou externe
    void setUseLocalModel(bool useLocal);

    // Configurer l'URL API externe
    void setExternalApiUrl(const QString &url);

    // Définir les paramètres de génération
    void setParameters(const QVariantMap &params);

    // Annuler la génération en cours
    void cancelGeneration();

    // État actuel
    bool isGenerating() const;

signals:
    void generationStarted();
    void generationProgress(int percent);
    void generationFinished(const QImage &result);
    void generationFailed(const QString &error);
    void generationCancelled();

private:
    QNetworkAccessManager *m_networkManager;
    bool m_useLocalModel;
    QString m_apiUrl;
    QVariantMap m_parameters;
    bool m_isGenerating;
    QNetworkReply *m_currentReply;

    // Méthodes internes
    QImage generateImageLocally(const QString &prompt, int width, int height);
    void generateImageExternally(const QString &prompt, int width, int height);
    void handleNetworkReply(QNetworkReply *reply);
};

#endif // GENERATOR_H
