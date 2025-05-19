#include "glitch.h"
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
#include <QApplication>
#include <QGroupBox>
#include <QFormLayout>
#include <algorithm>
#include <vector>

GlitchPlugin::GlitchPlugin(QObject *parent) : BasePlugin(parent)
{
    // Initialiser les préréglages
    QVariantMap preset1;
    preset1["type"] = static_cast<int>(RGBShift);
    preset1["xShift"] = 5;
    preset1["yShift"] = 0;
    m_presets.append(qMakePair(QString("RGB Drift"), preset1));

    QVariantMap preset2;
    preset2["type"] = static_cast<int>(BlockShift);
    preset2["blockSize"] = 15;
    preset2["maxOffset"] = 20;
    preset2["chance"] = 30;
    m_presets.append(qMakePair(QString("Digital Fault"), preset2));

    QVariantMap preset3;
    preset3["type"] = static_cast<int>(Corruption);
    preset3["intensity"] = 50;
    preset3["seed"] = -1;
    m_presets.append(qMakePair(QString("Data Corruption"), preset3));

    QVariantMap preset4;
    preset4["type"] = static_cast<int>(ScanLines);
    preset4["lineThickness"] = 2;
    preset4["lineSpacing"] = 4;
    preset4["opacity"] = 70;
    m_presets.append(qMakePair(QString("Retro VHS"), preset4));

    QVariantMap preset5;
    preset5["type"] = static_cast<int>(PixelSort);
    preset5["threshold"] = 128;
    preset5["sortLength"] = 50;
    m_presets.append(qMakePair(QString("Pixel Melt"), preset5));
}

QString GlitchPlugin::name() const
{
    return "Glitch Effects";
}

QString GlitchPlugin::description() const
{
    return "Applique divers effets de glitch inspirés des erreurs numériques pour un style électro/cyberpunk.";
}

QString GlitchPlugin::category() const
{
    return "Artistic";
}

QImage GlitchPlugin::processImage(const QImage &image, const QVariantMap &parameters)
{
    // Extraire les paramètres
    GlitchType type = static_cast<GlitchType>(parameters.value("type", RGBShift).toInt());

    // Appliquer l'effet correspondant
    switch (type) {
    case RGBShift:
        return applyRGBShift(image,
                             parameters.value("xShift", 3).toInt(),
                             parameters.value("yShift", 0).toInt());

    case BlockShift:
        return applyBlockShift(image,
                               parameters.value("blockSize", 10).toInt(),
                               parameters.value("maxOffset", 15).toInt(),
                               parameters.value("chance", 20).toInt());

    case PixelSort:
        return applyPixelSort(image,
                              parameters.value("threshold", 128).toInt(),
                              parameters.value("sortLength", 50).toInt());

    case Corruption:
        return applyCorruption(image,
                               parameters.value("intensity", 30).toInt(),
                               parameters.value("seed", -1).toInt());

    case ScanLines:
        return applyScanLines(image,
                              parameters.value("lineThickness", 1).toInt(),
                              parameters.value("lineSpacing", 3).toInt(),
                              parameters.value("opacity", 50).toInt());

    default:
        return image; // Fallback
    }
}

QWidget* GlitchPlugin::createControlPanel()
{
    // Créer un widget pour contrôler les paramètres du glitch
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Type de glitch
    QHBoxLayout *typeLayout = new QHBoxLayout();
    QLabel *typeLabel = new QLabel("Type:", widget);
    QComboBox *typeCombo = new QComboBox(widget);
    typeCombo->addItem("RGB Shift", RGBShift);
    typeCombo->addItem("Block Shift", BlockShift);
    typeCombo->addItem("Pixel Sort", PixelSort);
    typeCombo->addItem("Corruption", Corruption);
    typeCombo->addItem("Scan Lines", ScanLines);

    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(typeCombo);
    layout->addLayout(typeLayout);

    // Paramètres pour RGB Shift
    QGroupBox *rgbGroup = new QGroupBox("RGB Shift Parameters", widget);
    QFormLayout *rgbLayout = new QFormLayout(rgbGroup);

    QSlider *xShiftSlider = new QSlider(Qt::Horizontal, rgbGroup);
    xShiftSlider->setRange(-20, 20);
    xShiftSlider->setValue(3);
    QSpinBox *xShiftSpin = new QSpinBox(rgbGroup);
    xShiftSpin->setRange(-20, 20);
    xShiftSpin->setValue(3);

    QSlider *yShiftSlider = new QSlider(Qt::Horizontal, rgbGroup);
    yShiftSlider->setRange(-20, 20);
    yShiftSlider->setValue(0);
    QSpinBox *yShiftSpin = new QSpinBox(rgbGroup);
    yShiftSpin->setRange(-20, 20);
    yShiftSpin->setValue(0);

    // Connecter les widgets
    connect(xShiftSlider, &QSlider::valueChanged, xShiftSpin, &QSpinBox::setValue);
    connect(xShiftSpin, QOverload<int>::of(&QSpinBox::valueChanged), xShiftSlider, &QSlider::setValue);
    connect(yShiftSlider, &QSlider::valueChanged, yShiftSpin, &QSpinBox::setValue);
    connect(yShiftSpin, QOverload<int>::of(&QSpinBox::valueChanged), yShiftSlider, &QSlider::setValue);

    rgbLayout->addRow("X Shift:", xShiftSlider);
    rgbLayout->addRow("", xShiftSpin);
    rgbLayout->addRow("Y Shift:", yShiftSlider);
    rgbLayout->addRow("", yShiftSpin);

    // Paramètres pour Block Shift
    QGroupBox *blockGroup = new QGroupBox("Block Shift Parameters", widget);
    QFormLayout *blockLayout = new QFormLayout(blockGroup);

    QSlider *blockSizeSlider = new QSlider(Qt::Horizontal, blockGroup);
    blockSizeSlider->setRange(1, 50);
    blockSizeSlider->setValue(10);
    QSpinBox *blockSizeSpin = new QSpinBox(blockGroup);
    blockSizeSpin->setRange(1, 50);
    blockSizeSpin->setValue(10);

    QSlider *maxOffsetSlider = new QSlider(Qt::Horizontal, blockGroup);
    maxOffsetSlider->setRange(1, 100);
    maxOffsetSlider->setValue(15);
    QSpinBox *maxOffsetSpin = new QSpinBox(blockGroup);
    maxOffsetSpin->setRange(1, 100);
    maxOffsetSpin->setValue(15);

    QSlider *chanceSlider = new QSlider(Qt::Horizontal, blockGroup);
    chanceSlider->setRange(1, 100);
    chanceSlider->setValue(20);
    QSpinBox *chanceSpin = new QSpinBox(blockGroup);
    chanceSpin->setRange(1, 100);
    chanceSpin->setValue(20);

    // Connecter les widgets
    connect(blockSizeSlider, &QSlider::valueChanged, blockSizeSpin, &QSpinBox::setValue);
    connect(blockSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged), blockSizeSlider, &QSlider::setValue);
    connect(maxOffsetSlider, &QSlider::valueChanged, maxOffsetSpin, &QSpinBox::setValue);
    connect(maxOffsetSpin, QOverload<int>::of(&QSpinBox::valueChanged), maxOffsetSlider, &QSlider::setValue);
    connect(chanceSlider, &QSlider::valueChanged, chanceSpin, &QSpinBox::setValue);
    connect(chanceSpin, QOverload<int>::of(&QSpinBox::valueChanged), chanceSlider, &QSlider::setValue);

    blockLayout->addRow("Block Size:", blockSizeSlider);
    blockLayout->addRow("", blockSizeSpin);
    blockLayout->addRow("Max Offset:", maxOffsetSlider);
    blockLayout->addRow("", maxOffsetSpin);
    blockLayout->addRow("Chance (%):", chanceSlider);
    blockLayout->addRow("", chanceSpin);

    // Paramètres pour Pixel Sort
    QGroupBox *pixelGroup = new QGroupBox("Pixel Sort Parameters", widget);
    QFormLayout *pixelLayout = new QFormLayout(pixelGroup);

    QSlider *thresholdSlider = new QSlider(Qt::Horizontal, pixelGroup);
    thresholdSlider->setRange(0, 255);
    thresholdSlider->setValue(128);
    QSpinBox *thresholdSpin = new QSpinBox(pixelGroup);
    thresholdSpin->setRange(0, 255);
    thresholdSpin->setValue(128);

    QSlider *sortLengthSlider = new QSlider(Qt::Horizontal, pixelGroup);
    sortLengthSlider->setRange(1, 200);
    sortLengthSlider->setValue(50);
    QSpinBox *sortLengthSpin = new QSpinBox(pixelGroup);
    sortLengthSpin->setRange(1, 200);
    sortLengthSpin->setValue(50);

    // Connecter les widgets
    connect(thresholdSlider, &QSlider::valueChanged, thresholdSpin, &QSpinBox::setValue);
    connect(thresholdSpin, QOverload<int>::of(&QSpinBox::valueChanged), thresholdSlider, &QSlider::setValue);
    connect(sortLengthSlider, &QSlider::valueChanged, sortLengthSpin, &QSpinBox::setValue);
    connect(sortLengthSpin, QOverload<int>::of(&QSpinBox::valueChanged), sortLengthSlider, &QSlider::setValue);

    pixelLayout->addRow("Threshold:", thresholdSlider);
    pixelLayout->addRow("", thresholdSpin);
    pixelLayout->addRow("Sort Length:", sortLengthSlider);
    pixelLayout->addRow("", sortLengthSpin);

    // Paramètres pour Corruption
    QGroupBox *corruptGroup = new QGroupBox("Corruption Parameters", widget);
    QFormLayout *corruptLayout = new QFormLayout(corruptGroup);

    QSlider *intensitySlider = new QSlider(Qt::Horizontal, corruptGroup);
    intensitySlider->setRange(1, 100);
    intensitySlider->setValue(30);
    QSpinBox *intensitySpin = new QSpinBox(corruptGroup);
    intensitySpin->setRange(1, 100);
    intensitySpin->setValue(30);

    QSpinBox *seedSpin = new QSpinBox(corruptGroup);
    seedSpin->setRange(-1, 9999);
    seedSpin->setValue(-1);
    seedSpin->setSpecialValueText("Random");

    // Connecter les widgets
    connect(intensitySlider, &QSlider::valueChanged, intensitySpin, &QSpinBox::setValue);
    connect(intensitySpin, QOverload<int>::of(&QSpinBox::valueChanged), intensitySlider, &QSlider::setValue);

    corruptLayout->addRow("Intensity:", intensitySlider);
    corruptLayout->addRow("", intensitySpin);
    corruptLayout->addRow("Seed:", seedSpin);

    // Paramètres pour Scan Lines
    QGroupBox *scanGroup = new QGroupBox("Scan Lines Parameters", widget);
    QFormLayout *scanLayout = new QFormLayout(scanGroup);

    QSlider *lineThicknessSlider = new QSlider(Qt::Horizontal, scanGroup);
    lineThicknessSlider->setRange(1, 10);
    lineThicknessSlider->setValue(1);
    QSpinBox *lineThicknessSpin = new QSpinBox(scanGroup);
    lineThicknessSpin->setRange(1, 10);
    lineThicknessSpin->setValue(1);

    QSlider *lineSpacingSlider = new QSlider(Qt::Horizontal, scanGroup);
    lineSpacingSlider->setRange(1, 20);
    lineSpacingSlider->setValue(3);
    QSpinBox *lineSpacingSpin = new QSpinBox(scanGroup);
    lineSpacingSpin->setRange(1, 20);
    lineSpacingSpin->setValue(3);

    QSlider *opacitySlider = new QSlider(Qt::Horizontal, scanGroup);
    opacitySlider->setRange(0, 100);
    opacitySlider->setValue(50);
    QSpinBox *opacitySpin = new QSpinBox(scanGroup);
    opacitySpin->setRange(0, 100);
    opacitySpin->setValue(50);

    // Connecter les widgets
    connect(lineThicknessSlider, &QSlider::valueChanged, lineThicknessSpin, &QSpinBox::setValue);
    connect(lineThicknessSpin, QOverload<int>::of(&QSpinBox::valueChanged), lineThicknessSlider, &QSlider::setValue);
    connect(lineSpacingSlider, &QSlider::valueChanged, lineSpacingSpin, &QSpinBox::setValue);
    connect(lineSpacingSpin, QOverload<int>::of(&QSpinBox::valueChanged), lineSpacingSlider, &QSlider::setValue);
    connect(opacitySlider, &QSlider::valueChanged, opacitySpin, &QSpinBox::setValue);
    connect(opacitySpin, QOverload<int>::of(&QSpinBox::valueChanged), opacitySlider, &QSlider::setValue);

    scanLayout->addRow("Line Thickness:", lineThicknessSlider);
    scanLayout->addRow("", lineThicknessSpin);
    scanLayout->addRow("Line Spacing:", lineSpacingSlider);
    scanLayout->addRow("", lineSpacingSpin);
    scanLayout->addRow("Opacity (%):", opacitySlider);
    scanLayout->addRow("", opacitySpin);

    // Ajouter tous les groupes au layout principal
    layout->addWidget(rgbGroup);
    layout->addWidget(blockGroup);
    layout->addWidget(pixelGroup);
    layout->addWidget(corruptGroup);
    layout->addWidget(scanGroup);

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
                    int type = params.value("type", 0).toInt();
                    typeCombo->setCurrentIndex(type);

                    // Mettre à jour les contrôles selon le type
                    switch (type) {
                    case RGBShift:
                        xShiftSlider->setValue(params.value("xShift", 3).toInt());
                        yShiftSlider->setValue(params.value("yShift", 0).toInt());
                        break;
                    case BlockShift:
                        blockSizeSlider->setValue(params.value("blockSize", 10).toInt());
                        maxOffsetSlider->setValue(params.value("maxOffset", 15).toInt());
                        chanceSlider->setValue(params.value("chance", 20).toInt());
                        break;
                    case PixelSort:
                        thresholdSlider->setValue(params.value("threshold", 128).toInt());
                        sortLengthSlider->setValue(params.value("sortLength", 50).toInt());
                        break;
                    case Corruption:
                        intensitySlider->setValue(params.value("intensity", 30).toInt());
                        seedSpin->setValue(params.value("seed", -1).toInt());
                        break;
                    case ScanLines:
                        lineThicknessSlider->setValue(params.value("lineThickness", 1).toInt());
                        lineSpacingSlider->setValue(params.value("lineSpacing", 3).toInt());
                        opacitySlider->setValue(params.value("opacity", 50).toInt());
                        break;
                    }
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

    // Réinitialiser les paramètres
    connect(resetBtn, &QPushButton::clicked, [=]() {
        // Remettre les valeurs par défaut selon le type actuel
        switch (typeCombo->currentIndex()) {
        case RGBShift:
            xShiftSlider->setValue(3);
            yShiftSlider->setValue(0);
            break;
        case BlockShift:
            blockSizeSlider->setValue(10);
            maxOffsetSlider->setValue(15);
            chanceSlider->setValue(20);
            break;
        case PixelSort:
            thresholdSlider->setValue(128);
            sortLengthSlider->setValue(50);
            break;
        case Corruption:
            intensitySlider->setValue(30);
            seedSpin->setValue(-1);
            break;
        case ScanLines:
            lineThicknessSlider->setValue(1);
            lineSpacingSlider->setValue(3);
            opacitySlider->setValue(50);
            break;
        }
    });

    // Afficher/masquer les groupes selon le type d'effet
    auto updateVisibility = [=](int typeIndex) {
        rgbGroup->setVisible(typeIndex == RGBShift);
        blockGroup->setVisible(typeIndex == BlockShift);
        pixelGroup->setVisible(typeIndex == PixelSort);
        corruptGroup->setVisible(typeIndex == Corruption);
        scanGroup->setVisible(typeIndex == ScanLines);
    };

    // Initialiser la visibilité
    updateVisibility(typeCombo->currentIndex());

    // Connecter le combobox pour mettre à jour la visibilité
    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), updateVisibility);

    // Ajouter un stretch pour bien placer les éléments
    layout->addStretch();

    // Finaliser le layout
    widget->setLayout(layout);

    return widget;
}

QVariantMap GlitchPlugin::defaultParameters() const
{
    QVariantMap params;
    params["type"] = static_cast<int>(RGBShift);
    params["xShift"] = 3;
    params["yShift"] = 0;
    params["blockSize"] = 10;
    params["maxOffset"] = 15;
    params["chance"] = 20;
    params["threshold"] = 128;
    params["sortLength"] = 50;
    params["intensity"] = 30;
    params["seed"] = -1;
    params["lineThickness"] = 1;
    params["lineSpacing"] = 3;
    params["opacity"] = 50;
    return params;
}

QImage GlitchPlugin::applyRGBShift(const QImage &image, int xShift, int yShift) const
{
    if (image.isNull() || (xShift == 0 && yShift == 0)) {
        return image;
    }

    QImage result = image.convertToFormat(QImage::Format_ARGB32);
    QImage shiftedImage = result.copy();

    // Extraire les composantes RGB
    for (int y = 0; y < result.height(); ++y) {
        for (int x = 0; x < result.width(); ++x) {
            // Coordonnées pour le canal rouge (décalé)
            int redX = qBound(0, x + xShift, result.width() - 1);
            int redY = qBound(0, y + yShift, result.height() - 1);

            // Coordonnées pour le canal bleu (décalé dans l'autre sens)
            int blueX = qBound(0, x - xShift, result.width() - 1);
            int blueY = qBound(0, y - yShift, result.height() - 1);

            // Récupérer les composantes de couleur
            QRgb pixelRed = result.pixel(redX, redY);
            QRgb pixelGreen = result.pixel(x, y);
            QRgb pixelBlue = result.pixel(blueX, blueY);

            // Composer la nouvelle couleur avec les canaux décalés
            int r = qRed(pixelRed);
            int g = qGreen(pixelGreen);
            int b = qBlue(pixelBlue);
            int a = qAlpha(pixelGreen); // Utiliser l'alpha du pixel original

            // Appliquer la nouvelle couleur
            shiftedImage.setPixel(x, y, qRgba(r, g, b, a));
        }
    }

    return shiftedImage;
}

QImage GlitchPlugin::applyBlockShift(const QImage &image, int blockSize, int maxOffset, int chance) const
{
    if (image.isNull() || blockSize <= 0 || maxOffset <= 0 || chance <= 0) {
        return image;
    }

    QImage result = image.convertToFormat(QImage::Format_ARGB32);

    int width = result.width();
    int height = result.height();

    // Garantir une taille de bloc minimale
    blockSize = qMax(1, blockSize);

    // Nombre de blocs horizontaux et verticaux
    int hBlocks = (width + blockSize - 1) / blockSize;
    int vBlocks = (height + blockSize - 1) / blockSize;

    // Créer une image temporaire
    QImage tempImage = result.copy();

    // Traiter chaque bloc avec une certaine probabilité
    for (int v = 0; v < vBlocks; ++v) {
        // Vérifier si ce bloc doit être décalé
        if (QRandomGenerator::global()->bounded(100) >= chance) {
            continue;
        }

        // Calculer le décalage aléatoire
        int offset = QRandomGenerator::global()->bounded(-maxOffset, maxOffset + 1);
        if (offset == 0) {
            continue;
        }

        // Hauteur de ce bloc (peut être inférieure à blockSize à la dernière ligne)
        int blockHeight = qMin(blockSize, height - v * blockSize);

        // Coordonnées y de début du bloc
        int yStart = v * blockSize;

        // Copier le bloc avec décalage
        for (int y = 0; y < blockHeight; ++y) {
            int srcY = yStart + y;

            for (int x = 0; x < width; ++x) {
                // Calculer la coordonnée x source avec décalage
                int srcX = (x + offset) % width;
                if (srcX < 0) {
                    srcX += width;
                }

                // Copier le pixel
                tempImage.setPixel(x, srcY, result.pixel(srcX, srcY));
            }
        }
    }

    return tempImage;
}

QImage GlitchPlugin::applyPixelSort(const QImage &image, int threshold, int sortLength) const
{
    if (image.isNull() || sortLength <= 0) {
        return image;
    }

    QImage result = image.convertToFormat(QImage::Format_ARGB32);

    int width = result.width();
    int height = result.height();

    // Trier les pixels par ligne
    for (int y = 0; y < height; ++y) {
        int sortStart = -1;
        QVector<QRgb> pixelsToSort;

        for (int x = 0; x < width; ++x) {
            QRgb pixel = result.pixel(x, y);
            int brightness = qGray(pixel);

            // Commencer un nouveau segment à trier
            if (brightness > threshold && sortStart < 0) {
                sortStart = x;
                pixelsToSort.clear();
            }

            // Ajouter le pixel au segment ou terminer le tri
            if (sortStart >= 0) {
                pixelsToSort.append(pixel);

                // Vérifier si on doit terminer le tri
                if (brightness <= threshold || x == width - 1 || pixelsToSort.size() >= sortLength) {
                    // Trier les pixels par luminosité
                    std::sort(pixelsToSort.begin(), pixelsToSort.end(),
                              [](QRgb a, QRgb b) {
                                  return qGray(a) < qGray(b);
                              });

                    // Appliquer les pixels triés
                    for (int i = 0; i < pixelsToSort.size(); ++i) {
                        int posX = sortStart + i;
                        if (posX < width) {
                            result.setPixel(posX, y, pixelsToSort[i]);
                        }
                    }

                    // Réinitialiser pour le prochain segment
                    sortStart = -1;
                    pixelsToSort.clear();
                }
            }
        }
    }

    return result;
}

QImage GlitchPlugin::applyCorruption(const QImage &image, int intensity, int seed) const
{
    if (image.isNull() || intensity <= 0) {
        return image;
    }

    QImage result = image.convertToFormat(QImage::Format_ARGB32);

    // Initialiser le générateur aléatoire
    QRandomGenerator random;
    if (seed < 0) {
        random = *QRandomGenerator::global();
    } else {
        random = QRandomGenerator(seed);
    }

    int width = result.width();
    int height = result.height();

    // Calculer le nombre de corruptions
    int numCorruptions = (width * height * intensity) / 1000;

    for (int i = 0; i < numCorruptions; ++i) {
        // Choisir un type de corruption aléatoire
        int corruptionType = random.bounded(4);

        switch (corruptionType) {
        case 0: {
            // Remplacer un bloc par du bruit
            int blockSize = random.bounded(5, 20);
            int x = random.bounded(width - blockSize);
            int y = random.bounded(height - blockSize);

            for (int by = 0; by < blockSize; ++by) {
                for (int bx = 0; bx < blockSize; ++bx) {
                    int nx = x + bx;
                    int ny = y + by;

                    if (nx < width && ny < height) {
                        QRgb randomColor = qRgb(
                            random.bounded(256),
                            random.bounded(256),
                            random.bounded(256)
                            );
                        result.setPixel(nx, ny, randomColor);
                    }
                }
            }
            break;
        }
        case 1: {
            // Échanger deux lignes
            int y1 = random.bounded(height);
            int y2 = random.bounded(height);

            for (int x = 0; x < width; ++x) {
                QRgb temp = result.pixel(x, y1);
                result.setPixel(x, y1, result.pixel(x, y2));
                result.setPixel(x, y2, temp);
            }
            break;
        }
        case 2: {
            // Échanger deux colonnes
            int x1 = random.bounded(width);
            int x2 = random.bounded(width);

            for (int y = 0; y < height; ++y) {
                QRgb temp = result.pixel(x1, y);
                result.setPixel(x1, y, result.pixel(x2, y));
                result.setPixel(x2, y, temp);
            }
            break;
        }
        case 3: {
            // Décaler une section de ligne
            int y = random.bounded(height);
            int start = random.bounded(width / 2);
            int length = random.bounded(width / 4, width / 2);
            int offset = random.bounded(-20, 21);

            if (offset != 0) {
                QVector<QRgb> lineBuffer;

                // Sauvegarder les pixels
                for (int x = start; x < start + length && x < width; ++x) {
                    lineBuffer.append(result.pixel(x, y));
                }

                // Appliquer le décalage
                for (int i = 0; i < lineBuffer.size(); ++i) {
                    int x = start + i;
                    int destX = x + offset;

                    if (destX >= 0 && destX < width) {
                        result.setPixel(destX, y, lineBuffer[i]);
                    }
                }
            }
            break;
        }
        }
    }

    return result;
}

QImage GlitchPlugin::applyScanLines(const QImage &image, int lineThickness, int lineSpacing, int opacity) const
{
    if (image.isNull() || lineThickness <= 0 || lineSpacing <= 0) {
        return image;
    }

    QImage result = image.convertToFormat(QImage::Format_ARGB32);

    int width = result.width();
    int height = result.height();

    // Calculer l'opacité des lignes (0-255)
    int alpha = (opacity * 255) / 100;

    // Couleur des lignes de balayage (noir semi-transparent)
    QRgb scanLineColor = qRgba(0, 0, 0, alpha);

    // Appliquer les lignes de balayage
    for (int y = 0; y < height; ++y) {
        if ((y % (lineThickness + lineSpacing)) < lineThickness) {
            // Cette ligne fait partie d'une ligne de balayage
            for (int x = 0; x < width; ++x) {
                QRgb originalPixel = result.pixel(x, y);

                // Fusionner avec la couleur de la ligne de balayage
                int r = qRed(originalPixel);
                int g = qGreen(originalPixel);
                int b = qBlue(originalPixel);
                int a = qAlpha(originalPixel);

                // Assombrir légèrement le pixel
                r = r * (255 - alpha) / 255;
                g = g * (255 - alpha) / 255;
                b = b * (255 - alpha) / 255;

                result.setPixel(x, y, qRgba(r, g, b, a));
            }
        }
    }

    return result;
}
