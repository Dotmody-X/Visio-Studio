#include "basic.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QDebug>
#include <cmath> // Utilisé pour pow(), round() et d'autres fonctions mathématiques

BasicPlugin::BasicPlugin(QObject *parent) : BasePlugin(parent)
{
    // Initialiser les préréglages
    QVariantMap preset1;
    preset1["type"] = static_cast<int>(Brightness);
    preset1["brightness"] = 20;
    m_presets.append(qMakePair(QString("Plus lumineux"), preset1));

    QVariantMap preset2;
    preset2["type"] = static_cast<int>(Contrast);
    preset2["contrast"] = 30;
    m_presets.append(qMakePair(QString("Plus contrasté"), preset2));

    QVariantMap preset3;
    preset3["type"] = static_cast<int>(Saturation);
    preset3["saturation"] = 50;
    m_presets.append(qMakePair(QString("Vibrant"), preset3));

    QVariantMap preset4;
    preset4["type"] = static_cast<int>(Invert);
    m_presets.append(qMakePair(QString("Négatif"), preset4));
}

QString BasicPlugin::name() const
{
    return "Basic Effects";
}

QString BasicPlugin::description() const
{
    return "Effets de base pour ajuster la luminosité, le contraste et la saturation.";
}

QString BasicPlugin::category() const
{
    return "Basic";
}

QImage BasicPlugin::processImage(const QImage &image, const QVariantMap &parameters)
{
    // Extraire les paramètres
    EffectType type = static_cast<EffectType>(parameters.value("type", Brightness).toInt());

    // Appliquer l'effet correspondant
    switch (type) {
    case Brightness:
        return applyBrightness(image, parameters.value("brightness", 0).toInt());

    case Contrast:
        return applyContrast(image, parameters.value("contrast", 0).toInt());

    case Saturation:
        return applySaturation(image, parameters.value("saturation", 0).toInt());

    case Invert:
        return applyInvert(image);

    default:
        return image; // Fallback
    }
}

QWidget* BasicPlugin::createControlPanel()
{
    // Créer un widget pour contrôler les paramètres des effets de base
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Type d'effet
    QHBoxLayout *typeLayout = new QHBoxLayout();
    QLabel *typeLabel = new QLabel("Effect:", widget);
    QComboBox *typeCombo = new QComboBox(widget);
    typeCombo->addItem("Brightness", Brightness);
    typeCombo->addItem("Contrast", Contrast);
    typeCombo->addItem("Saturation", Saturation);
    typeCombo->addItem("Invert", Invert);

    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(typeCombo);
    layout->addLayout(typeLayout);

    // Luminosité
    QHBoxLayout *brightnessLayout = new QHBoxLayout();
    QLabel *brightnessLabel = new QLabel("Brightness:", widget);
    QSlider *brightnessSlider = new QSlider(Qt::Horizontal, widget);
    brightnessSlider->setRange(-100, 100);
    brightnessSlider->setValue(0);
    QSpinBox *brightnessSpin = new QSpinBox(widget);
    brightnessSpin->setRange(-100, 100);
    brightnessSpin->setValue(0);

    // Connecter le slider et le spinbox
    connect(brightnessSlider, &QSlider::valueChanged, brightnessSpin, &QSpinBox::setValue);
    connect(brightnessSpin, QOverload<int>::of(&QSpinBox::valueChanged), brightnessSlider, &QSlider::setValue);

    brightnessLayout->addWidget(brightnessLabel);
    brightnessLayout->addWidget(brightnessSlider);
    brightnessLayout->addWidget(brightnessSpin);
    layout->addLayout(brightnessLayout);

    // Contraste
    QHBoxLayout *contrastLayout = new QHBoxLayout();
    QLabel *contrastLabel = new QLabel("Contrast:", widget);
    QSlider *contrastSlider = new QSlider(Qt::Horizontal, widget);
    contrastSlider->setRange(-100, 100);
    contrastSlider->setValue(0);
    QSpinBox *contrastSpin = new QSpinBox(widget);
    contrastSpin->setRange(-100, 100);
    contrastSpin->setValue(0);

    // Connecter le slider et le spinbox
    connect(contrastSlider, &QSlider::valueChanged, contrastSpin, &QSpinBox::setValue);
    connect(contrastSpin, QOverload<int>::of(&QSpinBox::valueChanged), contrastSlider, &QSlider::setValue);

    contrastLayout->addWidget(contrastLabel);
    contrastLayout->addWidget(contrastSlider);
    contrastLayout->addWidget(contrastSpin);
    layout->addLayout(contrastLayout);

    // Saturation
    QHBoxLayout *saturationLayout = new QHBoxLayout();
    QLabel *saturationLabel = new QLabel("Saturation:", widget);
    QSlider *saturationSlider = new QSlider(Qt::Horizontal, widget);
    saturationSlider->setRange(-100, 100);
    saturationSlider->setValue(0);
    QSpinBox *saturationSpin = new QSpinBox(widget);
    saturationSpin->setRange(-100, 100);
    saturationSpin->setValue(0);

    // Connecter le slider et le spinbox
    connect(saturationSlider, &QSlider::valueChanged, saturationSpin, &QSpinBox::setValue);
    connect(saturationSpin, QOverload<int>::of(&QSpinBox::valueChanged), saturationSlider, &QSlider::setValue);

    saturationLayout->addWidget(saturationLabel);
    saturationLayout->addWidget(saturationSlider);
    saturationLayout->addWidget(saturationSpin);
    layout->addLayout(saturationLayout);

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

                    if (params.contains("brightness")) {
                        brightnessSlider->setValue(params.value("brightness").toInt());
                    }

                    if (params.contains("contrast")) {
                        contrastSlider->setValue(params.value("contrast").toInt());
                    }

                    if (params.contains("saturation")) {
                        saturationSlider->setValue(params.value("saturation").toInt());
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
        brightnessSlider->setValue(0);
        contrastSlider->setValue(0);
        saturationSlider->setValue(0);
    });

    // Afficher/masquer les contrôles selon le type d'effet
    auto updateVisibility = [=](int typeIndex) {
        bool isInvert = (typeIndex == Invert);

        brightnessLabel->setVisible(typeIndex == Brightness || !isInvert);
        brightnessSlider->setVisible(typeIndex == Brightness || !isInvert);
        brightnessSpin->setVisible(typeIndex == Brightness || !isInvert);

        contrastLabel->setVisible(typeIndex == Contrast || !isInvert);
        contrastSlider->setVisible(typeIndex == Contrast || !isInvert);
        contrastSpin->setVisible(typeIndex == Contrast || !isInvert);

        saturationLabel->setVisible(typeIndex == Saturation || !isInvert);
        saturationSlider->setVisible(typeIndex == Saturation || !isInvert);
        saturationSpin->setVisible(typeIndex == Saturation || !isInvert);
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

QVariantMap BasicPlugin::defaultParameters() const
{
    QVariantMap params;
    params["type"] = static_cast<int>(Brightness);
    params["brightness"] = 0;
    params["contrast"] = 0;
    params["saturation"] = 0;
    return params;
}

QImage BasicPlugin::applyBrightness(const QImage &image, int brightness) const
{
    if (image.isNull()) {
        return QImage();
    }

    // Convertir la valeur de -100..100 à un facteur plus pratique
    double factor = 1.0 + (brightness / 100.0);

    QImage result = image.convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < result.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(result.scanLine(y));

        for (int x = 0; x < result.width(); ++x) {
            int r = qRed(line[x]);
            int g = qGreen(line[x]);
            int b = qBlue(line[x]);
            int a = qAlpha(line[x]);

            // Ajuster chaque composante de couleur
            r = qBound(0, static_cast<int>(r * factor), 255);
            g = qBound(0, static_cast<int>(g * factor), 255);
            b = qBound(0, static_cast<int>(b * factor), 255);

            line[x] = qRgba(r, g, b, a);
        }
    }

    return result;
}

QImage BasicPlugin::applyContrast(const QImage &image, int contrast) const
{
    if (image.isNull()) {
        return QImage();
    }

    // Convertir la valeur de -100..100 à un facteur plus pratique
    // Utilisation explicite de std::abs de cmath pour éviter l'avertissement
    double factor = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - std::abs(contrast)));

    QImage result = image.convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < result.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(result.scanLine(y));

        for (int x = 0; x < result.width(); ++x) {
            int r = qRed(line[x]);
            int g = qGreen(line[x]);
            int b = qBlue(line[x]);
            int a = qAlpha(line[x]);

            // Appliquer la formule de contraste à chaque composante
            r = qBound(0, static_cast<int>(factor * (r - 128) + 128), 255);
            g = qBound(0, static_cast<int>(factor * (g - 128) + 128), 255);
            b = qBound(0, static_cast<int>(factor * (b - 128) + 128), 255);

            line[x] = qRgba(r, g, b, a);
        }
    }

    return result;
}

QImage BasicPlugin::applySaturation(const QImage &image, int saturation) const
{
    if (image.isNull()) {
        return QImage();
    }

    // Convertir la valeur de -100..100 à un facteur plus pratique
    double factor = 1.0 + (saturation / 100.0);

    QImage result = image.convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < result.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(result.scanLine(y));

        for (int x = 0; x < result.width(); ++x) {
            int r = qRed(line[x]);
            int g = qGreen(line[x]);
            int b = qBlue(line[x]);
            int a = qAlpha(line[x]);

            // Calculer la luminance (en échelle de gris)
            int gray = qGray(line[x]);

            // Ajuster la saturation en mélangeant la couleur originale avec le gris
            r = qBound(0, static_cast<int>(gray + factor * (r - gray)), 255);
            g = qBound(0, static_cast<int>(gray + factor * (g - gray)), 255);
            b = qBound(0, static_cast<int>(gray + factor * (b - gray)), 255);

            line[x] = qRgba(r, g, b, a);
        }
    }

    return result;
}

QImage BasicPlugin::applyInvert(const QImage &image) const
{
    if (image.isNull()) {
        return QImage();
    }

    QImage result = image.convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < result.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(result.scanLine(y));

        for (int x = 0; x < result.width(); ++x) {
            int r = qRed(line[x]);
            int g = qGreen(line[x]);
            int b = qBlue(line[x]);
            int a = qAlpha(line[x]);

            // Inverser chaque composante de couleur
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;

            line[x] = qRgba(r, g, b, a);
        }
    }

    return result;
}
