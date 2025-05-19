#include "dithering.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QRandomGenerator>
#include <QDebug>

DitheringPlugin::DitheringPlugin(QObject *parent) : BasePlugin(parent)
{
    // Initialiser les préréglages
    QVariantMap preset1;
    preset1["type"] = static_cast<int>(Threshold);
    preset1["threshold"] = 128;
    m_presets.append(qMakePair(QString("Noir et blanc (50%)"), preset1));

    QVariantMap preset2;
    preset2["type"] = static_cast<int>(Bayer);
    preset2["threshold"] = 128;
    preset2["matrixSize"] = 4;
    m_presets.append(qMakePair(QString("Ordonné (4x4)"), preset2));

    QVariantMap preset3;
    preset3["type"] = static_cast<int>(FloydSteinberg);
    preset3["threshold"] = 128;
    m_presets.append(qMakePair(QString("Floyd-Steinberg"), preset3));

    QVariantMap preset4;
    preset4["type"] = static_cast<int>(Random);
    preset4["threshold"] = 128;
    preset4["intensity"] = 50;
    m_presets.append(qMakePair(QString("Bruité"), preset4));
}

QString DitheringPlugin::name() const
{
    return "Dithering";
}

QString DitheringPlugin::description() const
{
    return "Applique différentes techniques de dithering pour créer des effets rétro et lo-fi.";
}

QString DitheringPlugin::category() const
{
    return "Dithering";
}

QImage DitheringPlugin::processImage(const QImage &image, const QVariantMap &parameters)
{
    // Convertir d'abord en niveaux de gris
    QImage grayImage = convertToGrayscale(image);

    // Extraire les paramètres
    DitheringType type = static_cast<DitheringType>(parameters.value("type", Threshold).toInt());
    int threshold = parameters.value("threshold", 128).toInt();

    // Appliquer le type de dithering correspondant
    switch (type) {
    case Threshold:
        return applyThresholdDithering(grayImage, threshold);

    case Random:
        return applyRandomDithering(grayImage, threshold,
                                    parameters.value("intensity", 50).toInt());

    case Bayer:
        return applyBayerDithering(grayImage, threshold,
                                   parameters.value("matrixSize", 4).toInt());

    case FloydSteinberg:
        return applyFloydSteinbergDithering(grayImage, threshold);

    default:
        return grayImage; // Fallback
    }
}

QWidget* DitheringPlugin::createControlPanel()
{
    // Créer un widget pour contrôler les paramètres du dithering
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Type de dithering
    QHBoxLayout *typeLayout = new QHBoxLayout();
    QLabel *typeLabel = new QLabel("Type:", widget);
    QComboBox *typeCombo = new QComboBox(widget);
    typeCombo->addItem("Threshold", Threshold);
    typeCombo->addItem("Random", Random);
    typeCombo->addItem("Bayer", Bayer);
    typeCombo->addItem("Floyd-Steinberg", FloydSteinberg);

    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(typeCombo);
    layout->addLayout(typeLayout);

    // Seuil
    QHBoxLayout *thresholdLayout = new QHBoxLayout();
    QLabel *thresholdLabel = new QLabel("Threshold:", widget);
    QSlider *thresholdSlider = new QSlider(Qt::Horizontal, widget);
    thresholdSlider->setRange(0, 255);
    thresholdSlider->setValue(128);
    QSpinBox *thresholdSpin = new QSpinBox(widget);
    thresholdSpin->setRange(0, 255);
    thresholdSpin->setValue(128);

    // Connecter le slider et le spinbox
    connect(thresholdSlider, &QSlider::valueChanged, thresholdSpin, &QSpinBox::setValue);
    connect(thresholdSpin, QOverload<int>::of(&QSpinBox::valueChanged), thresholdSlider, &QSlider::setValue);

    thresholdLayout->addWidget(thresholdLabel);
    thresholdLayout->addWidget(thresholdSlider);
    thresholdLayout->addWidget(thresholdSpin);
    layout->addLayout(thresholdLayout);

    // Intensité (pour Random)
    QHBoxLayout *intensityLayout = new QHBoxLayout();
    QLabel *intensityLabel = new QLabel("Intensity:", widget);
    QSlider *intensitySlider = new QSlider(Qt::Horizontal, widget);
    intensitySlider->setRange(0, 100);
    intensitySlider->setValue(50);
    QSpinBox *intensitySpin = new QSpinBox(widget);
    intensitySpin->setRange(0, 100);
    intensitySpin->setValue(50);

    // Connecter le slider et le spinbox
    connect(intensitySlider, &QSlider::valueChanged, intensitySpin, &QSpinBox::setValue);
    connect(intensitySpin, QOverload<int>::of(&QSpinBox::valueChanged), intensitySlider, &QSlider::setValue);

    intensityLayout->addWidget(intensityLabel);
    intensityLayout->addWidget(intensitySlider);
    intensityLayout->addWidget(intensitySpin);
    layout->addLayout(intensityLayout);

    // Taille de matrice (pour Bayer)
    QHBoxLayout *matrixLayout = new QHBoxLayout();
    QLabel *matrixLabel = new QLabel("Matrix Size:", widget);
    QComboBox *matrixCombo = new QComboBox(widget);
    matrixCombo->addItem("2x2", 2);
    matrixCombo->addItem("4x4", 4);
    matrixCombo->addItem("8x8", 8);
    matrixCombo->setCurrentIndex(1); // 4x4 par défaut

    matrixLayout->addWidget(matrixLabel);
    matrixLayout->addWidget(matrixCombo);
    layout->addLayout(matrixLayout);

    // Préréglages
    QHBoxLayout *presetLayout = new QHBoxLayout();
    QLabel *presetLabel = new QLabel("Presets:", widget);
    QComboBox *presetCombo = new QComboBox(widget);

    // Ajouter les préréglages
    for (const auto &preset : m_presets) {
        presetCombo->addItem(preset.first);
    }

    presetLayout->addWidget(presetLabel);
    presetLayout->addWidget(presetCombo);
    layout->addLayout(presetLayout);

    // Appliquer le préréglage lorsqu'il est sélectionné
    connect(presetCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int index) {
                if (index >= 0 && index < m_presets.size()) {
                    const QVariantMap &params = m_presets[index].second;

                    // Mettre à jour les contrôles UI
                    typeCombo->setCurrentIndex(params.value("type", 0).toInt());
                    thresholdSlider->setValue(params.value("threshold", 128).toInt());
                    intensitySlider->setValue(params.value("intensity", 50).toInt());

                    int matrixSizeIndex = 1; // 4x4 par défaut
                    int matrixSize = params.value("matrixSize", 4).toInt();
                    if (matrixSize == 2) matrixSizeIndex = 0;
                    else if (matrixSize == 8) matrixSizeIndex = 2;
                    matrixCombo->setCurrentIndex(matrixSizeIndex);
                }
            });

    // Boutons Apply/Reset
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *resetBtn = new QPushButton("Reset", widget);
    QPushButton *applyBtn = new QPushButton("Apply", widget);

    buttonLayout->addWidget(resetBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(applyBtn);
    layout->addLayout(buttonLayout);

    // Afficher/masquer les contrôles selon le type de dithering
    auto updateVisibility = [=](int typeIndex) {
        bool isRandom = (typeIndex == Random);
        bool isBayer = (typeIndex == Bayer);

        intensityLabel->setVisible(isRandom);
        intensitySlider->setVisible(isRandom);
        intensitySpin->setVisible(isRandom);

        matrixLabel->setVisible(isBayer);
        matrixCombo->setVisible(isBayer);
    };

    // Initialiser la visibilité
    updateVisibility(typeCombo->currentIndex());

    // Connecter le combobox pour mettre à jour la visibilité
    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), updateVisibility);

    // Finaliser le layout
    layout->addStretch();
    widget->setLayout(layout);

    return widget;
}

QVariantMap DitheringPlugin::defaultParameters() const
{
    QVariantMap params;
    params["type"] = static_cast<int>(Threshold);
    params["threshold"] = 128;
    params["intensity"] = 50;
    params["matrixSize"] = 4;
    return params;
}

QImage DitheringPlugin::applyThresholdDithering(const QImage &image, int threshold) const
{
    // Simple seuillage
    return applyThreshold(image, threshold);
}

QImage DitheringPlugin::applyRandomDithering(const QImage &image, int threshold, int intensity) const
{
    if (image.isNull()) {
        return QImage();
    }

    QImage result = image.convertToFormat(QImage::Format_ARGB32);

    // Facteur d'intensité (0-100)
    double intensityFactor = intensity / 100.0;

    for (int y = 0; y < result.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(result.scanLine(y));

        for (int x = 0; x < result.width(); ++x) {
            int gray = qGray(line[x]);

            // Ajouter du bruit aléatoire
            int noise = QRandomGenerator::global()->bounded(-intensity, intensity + 1);
            gray = qBound(0, gray + noise, 255);

            // Appliquer le seuil
            int value = (gray < threshold) ? 0 : 255;
            line[x] = qRgba(value, value, value, qAlpha(line[x]));
        }
    }

    return result;
}

QImage DitheringPlugin::applyBayerDithering(const QImage &image, int threshold, int matrixSize) const
{
    if (image.isNull()) {
        return QImage();
    }

    QImage result = image.convertToFormat(QImage::Format_ARGB32);

    // Générer la matrice de Bayer
    QVector<QVector<int>> bayerMatrix = generateBayerMatrix(matrixSize);

    // Facteur de normalisation pour la matrice
    double matrixNorm = matrixSize * matrixSize;

    for (int y = 0; y < result.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(result.scanLine(y));

        for (int x = 0; x < result.width(); ++x) {
            int gray = qGray(line[x]);

            // Appliquer la matrice de Bayer
            int matrixX = x % matrixSize;
            int matrixY = y % matrixSize;
            double thresholdModifier = (bayerMatrix[matrixY][matrixX] / matrixNorm) * 255;

            // Comparer avec le seuil modifié
            int value = (gray < threshold + thresholdModifier - 128) ? 0 : 255;
            line[x] = qRgba(value, value, value, qAlpha(line[x]));
        }
    }

    return result;
}

QImage DitheringPlugin::applyFloydSteinbergDithering(const QImage &image, int threshold) const
{
    if (image.isNull()) {
        return QImage();
    }

    // Créer une copie de l'image pour le traitement
    QImage result = image.convertToFormat(QImage::Format_ARGB32);

    // Créer une matrice temporaire pour stocker les valeurs de gris
    QVector<QVector<int>> grayMatrix(result.height(), QVector<int>(result.width()));

    // Remplir la matrice avec les valeurs de gris
    for (int y = 0; y < result.height(); ++y) {
        const QRgb *line = reinterpret_cast<const QRgb*>(result.constScanLine(y));
        for (int x = 0; x < result.width(); ++x) {
            grayMatrix[y][x] = qGray(line[x]);
        }
    }

    // Appliquer l'algorithme de Floyd-Steinberg
    for (int y = 0; y < result.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(result.scanLine(y));

        for (int x = 0; x < result.width(); ++x) {
            int oldPixel = grayMatrix[y][x];
            int newPixel = (oldPixel < threshold) ? 0 : 255;
            line[x] = qRgba(newPixel, newPixel, newPixel, qAlpha(line[x]));

            // Calculer l'erreur
            int error = oldPixel - newPixel;

            // Distribuer l'erreur aux pixels voisins selon l'algorithme de Floyd-Steinberg
            if (x + 1 < result.width())
                grayMatrix[y][x + 1] = qBound(0, grayMatrix[y][x + 1] + error * 7 / 16, 255);

            if (y + 1 < result.height()) {
                if (x - 1 >= 0)
                    grayMatrix[y + 1][x - 1] = qBound(0, grayMatrix[y + 1][x - 1] + error * 3 / 16, 255);

                grayMatrix[y + 1][x] = qBound(0, grayMatrix[y + 1][x] + error * 5 / 16, 255);

                if (x + 1 < result.width())
                    grayMatrix[y + 1][x + 1] = qBound(0, grayMatrix[y + 1][x + 1] + error * 1 / 16, 255);
            }
        }
    }

    return result;
}

QVector<QVector<int>> DitheringPlugin::generateBayerMatrix(int size) const
{
    // Vérifier que la taille est une puissance de 2
    if (size != 2 && size != 4 && size != 8) {
        size = 4; // Fallback à 4x4
    }

    // Initialiser la matrice de base 2x2
    QVector<QVector<int>> matrix = {
        {0, 2},
        {3, 1}
    };

    // Si nous voulons une plus grande matrice, construire récursivement
    int currentSize = 2;
    while (currentSize < size) {
        int newSize = currentSize * 2;
        QVector<QVector<int>> newMatrix(newSize, QVector<int>(newSize));

        // Appliquer la règle récursive pour construire une matrice plus grande
        for (int y = 0; y < currentSize; ++y) {
            for (int x = 0; x < currentSize; ++x) {
                newMatrix[y][x] = matrix[y][x] * 4;
                newMatrix[y][x + currentSize] = matrix[y][x] * 4 + 2;
                newMatrix[y + currentSize][x] = matrix[y][x] * 4 + 3;
                newMatrix[y + currentSize][x + currentSize] = matrix[y][x] * 4 + 1;
            }
        }

        matrix = newMatrix;
        currentSize = newSize;
    }

    return matrix;
}
