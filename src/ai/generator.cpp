#include "generator.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QBuffer>
#include <QDebug>
#include <QPainter>
#include <QFont>
#include <QRandomGenerator>

Generator::Generator(QObject *parent)
    : QObject(parent),
    m_networkManager(new QNetworkAccessManager(this)),
    m_useLocalModel(true),
    m_apiUrl("http://localhost:8000/generate"),
    m_isGenerating(false),
    m_currentReply(nullptr)
{
    // Initialiser les paramètres par défaut
    m_parameters["temperature"] = 0.7;
    m_parameters["seed"] = -1; // -1 = aléatoire
    m_parameters["steps"] = 30;

    // Connecter les signaux du network manager
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &Generator::handleNetworkReply);
}

Generator::~Generator()
{
    // Annuler toute génération en cours
    cancelGeneration();
}

void Generator::generateImage(const QString &prompt, int width, int height)
{
    // Ne pas commencer une nouvelle génération si une est déjà en cours
    if (m_isGenerating) {
        qWarning() << "Generation already in progress";
        return;
    }

    m_isGenerating = true;
    emit generationStarted();

    if (prompt.isEmpty()) {
        m_isGenerating = false;
        emit generationFailed(tr("Prompt cannot be empty"));
        return;
    }

    if (m_useLocalModel) {
        // Utiliser le modèle local (implémentation fictive pour l'instant)
        QImage result = generateImageLocally(prompt, width, height);

        if (!result.isNull()) {
            m_isGenerating = false;
            emit generationFinished(result);
        } else {
            m_isGenerating = false;
            emit generationFailed(tr("Local generation failed"));
        }
    } else {
        // Utiliser l'API externe
        generateImageExternally(prompt, width, height);
    }
}

void Generator::setUseLocalModel(bool useLocal)
{
    m_useLocalModel = useLocal;
}

void Generator::setExternalApiUrl(const QString &url)
{
    m_apiUrl = url;
}

void Generator::setParameters(const QVariantMap &params)
{
    m_parameters = params;
}

void Generator::cancelGeneration()
{
    if (!m_isGenerating) {
        return;
    }

    // Annuler la requête réseau en cours
    if (m_currentReply && m_currentReply->isRunning()) {
        m_currentReply->abort();
        m_currentReply = nullptr;
    }

    m_isGenerating = false;
    emit generationCancelled();
}

bool Generator::isGenerating() const
{
    return m_isGenerating;
}

QImage Generator::generateImageLocally(const QString &prompt, int width, int height)
{
    // Implémentation fictive - à remplacer par un vrai modèle de diffusion
    // Pour l'instant, générer une image de test avec le texte du prompt
    QImage image(width, height, QImage::Format_ARGB32);

    // Choisir une couleur de fond aléatoire dans les tons sombres
    QColor backgroundColor = QColor(
        QRandomGenerator::global()->bounded(20, 60),
        QRandomGenerator::global()->bounded(20, 60),
        QRandomGenerator::global()->bounded(20, 60)
        );
    image.fill(backgroundColor);

    // Ajouter quelques éléments visuels aléatoires pour simuler une génération
    QPainter painter(&image);

    // Définir une palette de couleurs vives pour l'ambiance électro
    QVector<QColor> palette = {
        QColor(0, 255, 255),    // Cyan
        QColor(255, 0, 255),    // Magenta
        QColor(255, 255, 0),    // Jaune
        QColor(0, 255, 0),      // Vert
        QColor(255, 0, 0),      // Rouge
        QColor(0, 0, 255)       // Bleu
    };

    // Dessiner quelques formes aléatoires
    painter.setRenderHint(QPainter::Antialiasing);
    for (int i = 0; i < 20; i++) {
        QColor color = palette[QRandomGenerator::global()->bounded(palette.size())];
        color.setAlpha(QRandomGenerator::global()->bounded(100, 200));
        painter.setPen(Qt::NoPen);
        painter.setBrush(color);

        int size = QRandomGenerator::global()->bounded(50, 150);
        int x = QRandomGenerator::global()->bounded(width - size);
        int y = QRandomGenerator::global()->bounded(height - size);

        // Choisir entre rectangle, cercle ou ligne
        int shapeType = QRandomGenerator::global()->bounded(3);
        switch (shapeType) {
        case 0:
            painter.drawRect(x, y, size, size);
            break;
        case 1:
            painter.drawEllipse(x, y, size, size);
            break;
        case 2:
            painter.setPen(QPen(color, QRandomGenerator::global()->bounded(1, 8)));
            painter.drawLine(
                QRandomGenerator::global()->bounded(width),
                QRandomGenerator::global()->bounded(height),
                QRandomGenerator::global()->bounded(width),
                QRandomGenerator::global()->bounded(height)
                );
            break;
        }
    }

    // Ajouter un effet "glitch" simple
    for (int i = 0; i < 5; i++) {
        int blockHeight = QRandomGenerator::global()->bounded(5, 20);
        int y = QRandomGenerator::global()->bounded(height - blockHeight);
        int offset = QRandomGenerator::global()->bounded(-20, 20);

        for (int bY = y; bY < y + blockHeight && bY < height; bY++) {
            for (int x = 0; x < width; x++) {
                int sourceX = (x + offset) % width;
                if (sourceX < 0) sourceX += width;

                if (x < width && bY < height && sourceX < width) {
                    image.setPixel(x, bY, image.pixel(sourceX, bY));
                }
            }
        }
    }

    // Dessiner le prompt comme un watermark
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10));
    painter.setOpacity(0.7);

    // Dessiner le prompt dans l'image
    QRect textRect = image.rect().adjusted(20, 20, -20, -20);
    painter.drawText(textRect, Qt::TextWordWrap, tr("Generated preview\nPrompt: %1").arg(prompt));

    // Simuler un progrès
    for (int i = 0; i < 100; i += 10) {
        emit generationProgress(i);
        QThread::msleep(100); // Simulation de délai
    }

    emit generationProgress(100);

    return image;
}

void Generator::generateImageExternally(const QString &prompt, int width, int height)
{
    // Préparer la requête
    QNetworkRequest request;
    request.setUrl(QUrl(m_apiUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Créer le corps de la requête
    QJsonObject requestData;
    requestData["prompt"] = prompt;
    requestData["width"] = width;
    requestData["height"] = height;

    // Ajouter les paramètres
    for (auto it = m_parameters.begin(); it != m_parameters.end(); ++it) {
        requestData[it.key()] = QJsonValue::fromVariant(it.value());
    }

    QJsonDocument doc(requestData);

    // Envoyer la requête
    m_currentReply = m_networkManager->post(request, doc.toJson());

    // Simuler un progrès
    // Dans une implémentation réelle, le progrès viendrait de l'API
    for (int i = 0; i < 50; i += 10) {
        if (!m_isGenerating) return; // Vérifier si la génération a été annulée
        emit generationProgress(i);
        QThread::msleep(100); // Simulation de délai
    }
}

void Generator::handleNetworkReply(QNetworkReply *reply)
{
    m_currentReply = nullptr;

    // Vérifier si la génération a été annulée
    if (!m_isGenerating) {
        reply->deleteLater();
        return;
    }

    // Vérifier les erreurs
    if (reply->error() != QNetworkReply::NoError) {
        m_isGenerating = false;
        emit generationFailed(reply->errorString());
        reply->deleteLater();
        return;
    }

    // Lire la réponse
    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    // Parser le JSON
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    if (doc.isNull() || !doc.isObject()) {
        m_isGenerating = false;
        emit generationFailed(tr("Invalid response from API"));
        return;
    }

    QJsonObject responseObj = doc.object();

    // Vérifier si la réponse contient une image
    if (responseObj.contains("image")) {
        QString base64Image = responseObj["image"].toString();
        QByteArray imageData = QByteArray::fromBase64(base64Image.toLatin1());

        QImage result;
        if (result.loadFromData(imageData)) {
            emit generationProgress(100);
            m_isGenerating = false;
            emit generationFinished(result);
        } else {
            m_isGenerating = false;
            emit generationFailed(tr("Failed to decode image data"));
        }
    } else {
        m_isGenerating = false;
        emit generationFailed(tr("Response does not contain image data"));
    }
}
