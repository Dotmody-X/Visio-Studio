#include "./mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QLabel>
#include <QProgressBar>
#include <QListWidget>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QStatusBar>
#include <QInputDialog>
#include <QApplication>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QFormLayout>
#include <QGroupBox>
#include "canvas.h"
#include "mixer.h"
#include "../plugins/pluginmanager.h"
#include "../ai/generator.h"
#include <QTextEdit>
#include <QSpinBox>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentProject(new Project(this))
{
    ui->setupUi(this);

    setupUI();
    createActions();
    createMenus();
    createToolbars();
    createStatusBar();
    createConnections();

    // Créer un nouveau projet vide par défaut
    m_currentProject->newProject(800, 600, Qt::white);

    updateWindowTitle();
    updateUI();

    // Définir la taille initiale de la fenêtre
    resize(1280, 800);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::on_actionNew_triggered()
{
    if (maybeSave()) {
        // Dialogue pour demander les dimensions du nouveau projet
        bool ok;
        int width = QInputDialog::getInt(this, tr("New Project"),
                                         tr("Width:"), 800, 50, 4000, 50, &ok);
        if (!ok) return;

        int height = QInputDialog::getInt(this, tr("New Project"),
                                          tr("Height:"), 600, 50, 4000, 50, &ok);
        if (!ok) return;

        // Créer un nouveau projet
        m_currentProject->newProject(width, height, Qt::white);

        // Mettre à jour l'interface
        CanvasView *canvasView = findChild<CanvasView*>("canvasView");
        if (canvasView) {
            canvasView->canvas()->setImage(m_currentProject->engine()->sourceImage());
        }

        updateWindowTitle();
        updateUI();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open VISIO STUDIO Project"), "",
                                                        tr("VISIO STUDIO Projects (*.vsd);;All Files (*)"));

        if (!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (m_currentProject->path().isEmpty()) {
        on_actionSaveAs_triggered();
    } else {
        saveFile(m_currentProject->path());
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save VISIO STUDIO Project"), "",
                                                    tr("VISIO STUDIO Projects (*.vsd);;All Files (*)"));

    if (!fileName.isEmpty()) {
        saveFile(fileName);
    }
}

void MainWindow::on_actionExport_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Image"), "",
                                                    tr("PNG (*.png);;JPEG (*.jpg *.jpeg);;All Files (*)"));

    if (!fileName.isEmpty()) {
        // Déterminer le format à partir de l'extension
        QFileInfo fileInfo(fileName);
        QString ext = fileInfo.suffix().toLower();

        QString format = "PNG"; // Format par défaut
        if (ext == "jpg" || ext == "jpeg") {
            format = "JPEG";
        }

        if (!m_currentProject->exportImage(fileName, format)) {
            QMessageBox::warning(this, tr("Export Failed"),
                                 tr("Failed to export the image to %1").arg(fileName));
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionUndo_triggered()
{
    // Sera implémenté plus tard
    // m_currentProject->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    // Sera implémenté plus tard
    // m_currentProject->redo();
}

void MainWindow::on_actionAddEffect_triggered()
{
    showPluginSelector();
}

void MainWindow::on_actionRemoveEffect_triggered()
{
    // Implémenté plus tard - Pour l'instant, il faudrait avoir un effet sélectionné
}

void MainWindow::on_actionAIGenerator_triggered()
{
    // Créer une boîte de dialogue pour le générateur d'IA
    QDialog dialog(this);
    dialog.setWindowTitle(tr("AI Image Generator"));
    dialog.setMinimumSize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Zone de saisie de la description
    QLabel *promptLabel = new QLabel(tr("Description:"), &dialog);
    QTextEdit *promptEdit = new QTextEdit(&dialog);
    promptEdit->setPlaceholderText(tr("Enter a detailed description of the image you want to generate..."));
    promptEdit->setMaximumHeight(100);

    // Paramètres de taille
    QHBoxLayout *sizeLayout = new QHBoxLayout();
    QLabel *widthLabel = new QLabel(tr("Width:"), &dialog);
    QSpinBox *widthSpin = new QSpinBox(&dialog);
    widthSpin->setRange(64, 1024);
    widthSpin->setValue(512);
    widthSpin->setSingleStep(64);

    QLabel *heightLabel = new QLabel(tr("Height:"), &dialog);
    QSpinBox *heightSpin = new QSpinBox(&dialog);
    heightSpin->setRange(64, 1024);
    heightSpin->setValue(512);
    heightSpin->setSingleStep(64);

    sizeLayout->addWidget(widthLabel);
    sizeLayout->addWidget(widthSpin);
    sizeLayout->addWidget(heightLabel);
    sizeLayout->addWidget(heightSpin);

    // Style visuel
    QHBoxLayout *styleLayout = new QHBoxLayout();
    QLabel *styleLabel = new QLabel(tr("Style:"), &dialog);
    QComboBox *styleCombo = new QComboBox(&dialog);
    styleCombo->addItem(tr("Cyberpunk"), "cyberpunk");
    styleCombo->addItem(tr("Néon"), "neon");
    styleCombo->addItem(tr("Glitch"), "glitch");
    styleCombo->addItem(tr("Minimaliste"), "minimalist");
    styleCombo->addItem(tr("Électro"), "electronic");
    styleCombo->addItem(tr("Abstrait"), "abstract");

    styleLayout->addWidget(styleLabel);
    styleLayout->addWidget(styleCombo);

    // Paramètres avancés
    QGroupBox *advancedGroup = new QGroupBox(tr("Advanced Settings"), &dialog);
    QFormLayout *advancedLayout = new QFormLayout(advancedGroup);

    QSlider *temperatureSlider = new QSlider(Qt::Horizontal, advancedGroup);
    temperatureSlider->setRange(0, 100);
    temperatureSlider->setValue(70);

    QSpinBox *seedSpin = new QSpinBox(advancedGroup);
    seedSpin->setRange(-1, 9999999);
    seedSpin->setValue(-1);
    seedSpin->setSpecialValueText(tr("Random"));

    advancedLayout->addRow(tr("Creativity:"), temperatureSlider);
    advancedLayout->addRow(tr("Seed:"), seedSpin);

    // Aperçu
    QLabel *previewLabel = new QLabel(&dialog);
    previewLabel->setMinimumSize(256, 256);
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setText(tr("Preview will appear here"));
    previewLabel->setFrameShape(QFrame::StyledPanel);

    // Barre de progression
    QProgressBar *progressBar = new QProgressBar(&dialog);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setTextVisible(true);
    progressBar->setVisible(false);

    // Boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton(tr("Cancel"), &dialog);
    QPushButton *generateBtn = new QPushButton(tr("Generate"), &dialog);
    QPushButton *useBtn = new QPushButton(tr("Use Image"), &dialog);
    useBtn->setEnabled(false);

    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(generateBtn);
    buttonLayout->addWidget(useBtn);

    // Assembler le layout
    layout->addWidget(promptLabel);
    layout->addWidget(promptEdit);
    layout->addLayout(sizeLayout);
    layout->addLayout(styleLayout);
    layout->addWidget(advancedGroup);
    layout->addWidget(previewLabel, 1);
    layout->addWidget(progressBar);
    layout->addLayout(buttonLayout);

    // Créer une instance de Generator
    Generator *generator = new Generator(&dialog);

    // Connecter les signaux
    connect(generateBtn, &QPushButton::clicked, &dialog, [&]() {
        // Préparer les paramètres
        QString prompt = promptEdit->toPlainText();
        if (prompt.isEmpty()) {
            QMessageBox::warning(&dialog, tr("Empty Prompt"),
                                 tr("Please enter a description for the image to generate."));
            return;
        }

        // Ajouter le style sélectionné au prompt
        QString style = styleCombo->currentData().toString();
        prompt += tr(", style: %1").arg(style);

        // Paramètres avancés
        QVariantMap params;
        params["temperature"] = temperatureSlider->value() / 100.0;
        params["seed"] = seedSpin->value();

        // Configurer le generator
        generator->setParameters(params);

        // Désactiver les contrôles pendant la génération
        promptEdit->setEnabled(false);
        widthSpin->setEnabled(false);
        heightSpin->setEnabled(false);
        styleCombo->setEnabled(false);
        temperatureSlider->setEnabled(false);
        seedSpin->setEnabled(false);
        generateBtn->setEnabled(false);

        // Afficher la barre de progression
        progressBar->setValue(0);
        progressBar->setVisible(true);

        // Lancer la génération
        generator->generateImage(prompt, widthSpin->value(), heightSpin->value());
    });

    connect(generator, &Generator::generationProgress, progressBar, &QProgressBar::setValue);

    connect(generator, &Generator::generationFinished, &dialog, [&](const QImage &image) {
        // Afficher l'image générée
        previewLabel->setPixmap(QPixmap::fromImage(image).scaled(
            previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Réactiver les contrôles
        promptEdit->setEnabled(true);
        widthSpin->setEnabled(true);
        heightSpin->setEnabled(true);
        styleCombo->setEnabled(true);
        temperatureSlider->setEnabled(true);
        seedSpin->setEnabled(true);
        generateBtn->setEnabled(true);
        useBtn->setEnabled(true);

        // Cacher la barre de progression
        progressBar->setVisible(false);
    });

    connect(generator, &Generator::generationFailed, &dialog, [&](const QString &error) {
        // Afficher l'erreur
        QMessageBox::critical(&dialog, tr("Generation Failed"),
                              tr("Failed to generate image: %1").arg(error));

        // Réactiver les contrôles
        promptEdit->setEnabled(true);
        widthSpin->setEnabled(true);
        heightSpin->setEnabled(true);
        styleCombo->setEnabled(true);
        temperatureSlider->setEnabled(true);
        seedSpin->setEnabled(true);
        generateBtn->setEnabled(true);

        // Cacher la barre de progression
        progressBar->setVisible(false);
    });

    connect(useBtn, &QPushButton::clicked, &dialog, [&]() {
        // Récupérer l'image générée
        QPixmap pixmap = previewLabel->pixmap(Qt::ReturnByValue);
        if (!pixmap.isNull()) {
            QImage image = pixmap.toImage();

            // Définir l'image comme source dans le projet
            m_currentProject->setSourceImage(image);

            // Fermer la boîte de dialogue
            dialog.accept();
        }
    });

    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    // Exécuter la boîte de dialogue
    dialog.exec();

    // Nettoyer
    delete generator;
}

void MainWindow::updateWindowTitle()
{
    QString title = m_currentProject->name();
    if (m_currentProject->isModified()) {
        title += " *";
    }
    title += " - VISIO STUDIO";
    setWindowTitle(title);
}

void MainWindow::updateUI()
{
    // Mettre à jour l'affichage de l'image
    updateEffectChain();

    // Mettre à jour les actions en fonction de l'état du projet
    ui->actionSave->setEnabled(m_currentProject->isModified());
    ui->actionUndo->setEnabled(m_currentProject->canUndo());
    ui->actionRedo->setEnabled(m_currentProject->canRedo());
}

void MainWindow::updateEffectChain()
{
    if (!m_currentProject) return;

    // Déclencher le traitement asynchrone de l'image
    m_currentProject->engine()->processImageAsync();

    // Mettre à jour le mixer
    Mixer *mixer = findChild<Mixer*>("mixerPanel");
    if (mixer) {
        mixer->updateEffectChain();
    }
}

void MainWindow::projectModified()
{
    updateWindowTitle();
    updateUI();
}

void MainWindow::projectSaved()
{
    updateWindowTitle();
    updateUI();
}

void MainWindow::projectLoaded()
{
    updateWindowTitle();
    updateUI();
}

void MainWindow::processingStarted()
{
    // Afficher un indicateur de progression
    statusBar()->showMessage(tr("Processing..."));

    // Désactiver les contrôles pendant le traitement
    ui->actionAddEffect->setEnabled(false);
    ui->actionRemoveEffect->setEnabled(false);
    ui->actionZoomIn->setEnabled(false);
    ui->actionZoomOut->setEnabled(false);
    ui->actionZoomReset->setEnabled(false);
}

void MainWindow::processingStopped()
{
    // Masquer l'indicateur de progression
    statusBar()->clearMessage();

    // Réactiver les contrôles
    ui->actionAddEffect->setEnabled(true);
    ui->actionRemoveEffect->setEnabled(true);
    ui->actionZoomIn->setEnabled(true);
    ui->actionZoomOut->setEnabled(true);
    ui->actionZoomReset->setEnabled(true);

    // Mettre à jour l'affichage avec la nouvelle image
    QImage result = m_currentProject->engine()->processedImage();
    CanvasView *canvasView = findChild<CanvasView*>("canvasView");
    if (canvasView && !result.isNull()) {
        canvasView->canvas()->setImage(result);
    }
}

void MainWindow::processingCanceled()
{
    // Masquer l'indicateur de progression
    statusBar()->showMessage(tr("Processing canceled"), 2000);

    // Réactiver les contrôles
    ui->actionAddEffect->setEnabled(true);
    ui->actionRemoveEffect->setEnabled(true);
    ui->actionZoomIn->setEnabled(true);
    ui->actionZoomOut->setEnabled(true);
    ui->actionZoomReset->setEnabled(true);
}

void MainWindow::processingFailed(const QString &errorMessage)
{
    // Afficher l'erreur
    statusBar()->showMessage(tr("Processing failed: %1").arg(errorMessage), 3000);

    // Réactiver les contrôles
    ui->actionAddEffect->setEnabled(true);
    ui->actionRemoveEffect->setEnabled(true);
    ui->actionZoomIn->setEnabled(true);
    ui->actionZoomOut->setEnabled(true);
    ui->actionZoomReset->setEnabled(true);

    // Afficher une boîte de dialogue d'erreur
    QMessageBox::critical(this, tr("Processing Error"),
                          tr("An error occurred during image processing:\n%1").arg(errorMessage));
}

void MainWindow::processingProgress(int percent)
{
    // Mettre à jour la barre de progression
    statusBar()->showMessage(tr("Processing... %1%").arg(percent));
}

void MainWindow::setupUI()
{
    // Configuration du widget central - sera un canvas pour l'affichage d'image
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Créer une vue de canvas avec défilement
    CanvasView *canvasView = new CanvasView(centralWidget);
    canvasView->setObjectName("canvasView");

    // Connecter le signal de changement de zoom
    connect(canvasView, &CanvasView::zoomChanged, this, [this](double factor) {
        statusBar()->showMessage(tr("Zoom: %1%").arg(int(factor * 100)), 2000);
    });

    layout->addWidget(canvasView, 1);

    // Panneau pour la chaîne d'effets (style FL Studio) - en bas de l'écran
    Mixer *mixer = new Mixer(centralWidget);
    mixer->setObjectName("mixerPanel");

    // Connecter le mixer au moteur
    mixer->setEngine(m_currentProject->engine());

    // Connecter les signaux du mixer
    connect(mixer, &Mixer::effectAdded, this, [this](int id) {
        statusBar()->showMessage(tr("Effect added (ID: %1)").arg(id), 2000);
    });

    connect(mixer, &Mixer::effectRemoved, this, [this](int id) {
        statusBar()->showMessage(tr("Effect removed (ID: %1)").arg(id), 2000);
    });

    connect(mixer, &Mixer::effectEdited, this, [this, mixer](int id) {
        showPluginEditor(id);
    });

    layout->addWidget(mixer);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Créer le dock pour les plugins et effets sur le côté gauche
    QDockWidget *pluginDock = new QDockWidget(tr("Effects"), this);
    pluginDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget *pluginWidget = new QWidget(pluginDock);
    QVBoxLayout *pluginLayout = new QVBoxLayout(pluginWidget);

    QListWidget *pluginList = new QListWidget(pluginWidget);

    // Remplir la liste des plugins
    PluginManager *pluginManager = PluginManager::instance();
    for (PluginInterface *plugin : pluginManager->allPlugins()) {
        QListWidgetItem *item = new QListWidgetItem(plugin->name());
        item->setData(Qt::UserRole, QVariant::fromValue(plugin));
        pluginList->addItem(item);
    }

    // Ajouter un bouton pour ajouter l'effet sélectionné
    QPushButton *addEffectBtn = new QPushButton(tr("Add Effect"), pluginWidget);

    // Connecter le clic du bouton
    connect(addEffectBtn, &QPushButton::clicked, this, [this, pluginList, mixer]() {
        QListWidgetItem *item = pluginList->currentItem();
        if (item) {
            PluginInterface *plugin = item->data(Qt::UserRole).value<PluginInterface*>();
            if (plugin) {
                mixer->addEffect(plugin);
            }
        }
    });

    pluginLayout->addWidget(pluginList);
    pluginLayout->addWidget(addEffectBtn);

    pluginWidget->setLayout(pluginLayout);
    pluginDock->setWidget(pluginWidget);

    addDockWidget(Qt::LeftDockWidgetArea, pluginDock);

    // Créer le dock pour les propriétés sur le côté droit
    QDockWidget *propDock = new QDockWidget(tr("Properties"), this);
    propDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget *propWidget = new QWidget(propDock);
    QVBoxLayout *propLayout = new QVBoxLayout(propWidget);
    propWidget->setLayout(propLayout);
    propDock->setWidget(propWidget);

    addDockWidget(Qt::RightDockWidgetArea, propDock);

    // Stocker les références importantes pour un accès ultérieur
    canvasView->canvas()->setImage(m_currentProject->engine()->sourceImage());
}

void MainWindow::createActions()
{
    // Les actions de base sont créées par Qt Designer dans le UI
    // Nous pouvons ajouter des actions supplémentaires ici
}

void MainWindow::createMenus()
{
    // Les menus de base sont créés par Qt Designer dans le UI
    // Nous pouvons personnaliser davantage ici

    // Par exemple, ajouter dynamiquement des entrées de menu pour les effets récents
}

void MainWindow::createToolbars()
{
    // Les toolbars de base sont créées par Qt Designer dans le UI
    // Nous pouvons personnaliser davantage ici
}

void MainWindow::createStatusBar()
{
    m_statusLabel = new QLabel(statusBar());
    statusBar()->addPermanentWidget(m_statusLabel);

    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createConnections()
{
    // Connexion des signaux du projet
    connect(m_currentProject.get(), &Project::projectModified, this, &MainWindow::projectModified);
    connect(m_currentProject.get(), &Project::projectSaved, this, &MainWindow::projectSaved);
    connect(m_currentProject.get(), &Project::projectLoaded, this, &MainWindow::projectLoaded);

    // Connexion des signaux de l'engine
    Engine *engine = m_currentProject->engine();
    connect(engine, &Engine::processingStarted, this, &MainWindow::processingStarted);
    connect(engine, &Engine::processingFinished, this, &MainWindow::processingStopped);
    connect(engine, &Engine::processingCanceled, this, &MainWindow::processingCanceled);
    connect(engine, &Engine::processingFailed, this, &MainWindow::processingFailed);
    connect(engine, &Engine::processingProgress, this, &MainWindow::processingProgress);
    connect(engine, &Engine::effectChainChanged, this, &MainWindow::updateEffectChain);

    // Connexion des actions du menu
    connect(ui->actionShowGrid, &QAction::toggled, this, [this](bool checked) {
        CanvasView *canvasView = findChild<CanvasView*>("canvasView");
        if (canvasView) {
            canvasView->canvas()->setShowGrid(checked);
        }
    });

    connect(ui->actionShowRulers, &QAction::toggled, this, [this](bool checked) {
        CanvasView *canvasView = findChild<CanvasView*>("canvasView");
        if (canvasView) {
            canvasView->canvas()->setShowRulers(checked);
        }
    });

    connect(ui->actionZoomIn, &QAction::triggered, this, [this]() {
        CanvasView *canvasView = findChild<CanvasView*>("canvasView");
        if (canvasView) {
            canvasView->canvas()->zoomIn();
        }
    });

    connect(ui->actionZoomOut, &QAction::triggered, this, [this]() {
        CanvasView *canvasView = findChild<CanvasView*>("canvasView");
        if (canvasView) {
            canvasView->canvas()->zoomOut();
        }
    });

    connect(ui->actionZoomReset, &QAction::triggered, this, [this]() {
        CanvasView *canvasView = findChild<CanvasView*>("canvasView");
        if (canvasView) {
            canvasView->canvas()->resetZoom();
        }
    });
}

void MainWindow::showPluginSelector()
{
    // Créer une boîte de dialogue pour sélectionner un plugin
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Add Effect"));
    dialog.setMinimumSize(300, 400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Créer une liste pour les plugins disponibles
    QListWidget *pluginList = new QListWidget(&dialog);

    // Remplir la liste des plugins
    PluginManager *pluginManager = PluginManager::instance();
    QList<PluginInterface*> allPlugins = pluginManager->allPlugins();

    if (allPlugins.isEmpty()) {
        QMessageBox::information(this, tr("No Plugins Available"),
                                 tr("No plugins are currently available. Please install some plugins and restart the application."));
        return;
    }

    // Grouper par catégorie
    QMap<QString, QList<PluginInterface*>> pluginsByCategory;
    for (PluginInterface *plugin : allPlugins) {
        QString category = plugin->category();
        if (!pluginsByCategory.contains(category)) {
            pluginsByCategory[category] = QList<PluginInterface*>();
        }
        pluginsByCategory[category].append(plugin);
    }

    // Ajouter les plugins à la liste, organisés par catégorie
    for (auto category = pluginsByCategory.begin(); category != pluginsByCategory.end(); ++category) {
        // Ajouter un en-tête de catégorie
        QListWidgetItem *headerItem = new QListWidgetItem(category.key());
        headerItem->setFlags(Qt::ItemIsEnabled); // Non sélectionnable
        headerItem->setBackground(QColor(60, 60, 60));
        headerItem->setForeground(Qt::cyan);
        pluginList->addItem(headerItem);

        // Ajouter les plugins de cette catégorie
        for (PluginInterface *plugin : category.value()) {
            QListWidgetItem *item = new QListWidgetItem(plugin->name());
            item->setData(Qt::UserRole, QVariant::fromValue(plugin));
            item->setToolTip(plugin->description());
            pluginList->addItem(item);
        }
    }

    layout->addWidget(pluginList);

    // Boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton(tr("Cancel"), &dialog);
    QPushButton *addBtn = new QPushButton(tr("Add"), &dialog);
    addBtn->setDefault(true);

    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(addBtn);

    layout->addLayout(buttonLayout);

    // Connecter les boutons
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(addBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    // Exécuter la boîte de dialogue
    if (dialog.exec() == QDialog::Accepted) {
        QListWidgetItem *item = pluginList->currentItem();

        // Vérifier que l'élément sélectionné est un plugin (pas un en-tête)
        if (item && item->data(Qt::UserRole).isValid()) {
            PluginInterface *plugin = item->data(Qt::UserRole).value<PluginInterface*>();
            if (plugin) {
                // Ajouter l'effet à la chaîne
                int effectId = m_currentProject->engine()->addEffect(plugin);

                if (effectId >= 0) {
                    showPluginEditor(effectId);
                } else {
                    QMessageBox::warning(this, tr("Add Effect Failed"),
                                         tr("Failed to add the effect to the chain."));
                }
            }
        } else {
            QMessageBox::warning(this, tr("No Plugin Selected"),
                                 tr("Please select a plugin from the list."));
        }
    }
}

void MainWindow::showPluginEditor(int effectId)
{
    if (!m_currentProject) return;

    Engine *engine = m_currentProject->engine();
    Engine::EffectNode node = engine->effectById(effectId);

    if (!node.plugin) {
        QMessageBox::warning(this, tr("Edit Effect"),
                             tr("Cannot edit effect: effect not found or plugin is invalid."));
        return;
    }

    // Créer une boîte de dialogue pour l'éditeur
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Edit Effect: %1").arg(node.plugin->name()));
    dialog.setMinimumSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Créer le panneau de contrôle du plugin
    QWidget *controlPanel = node.plugin->createControlPanel();
    layout->addWidget(controlPanel);

    // Ajouter des boutons OK/Cancel
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton(tr("Cancel"), &dialog);
    QPushButton *okBtn = new QPushButton(tr("Apply"), &dialog);
    okBtn->setDefault(true);

    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(okBtn);

    layout->addLayout(buttonLayout);

    // Connecter les boutons
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    // Exécuter la boîte de dialogue
    if (dialog.exec() == QDialog::Accepted) {
        // À implémenter: récupérer les paramètres depuis le panneau de contrôle
        // et les appliquer à l'effet
    }
}

bool MainWindow::maybeSave()
{
    if (m_currentProject->isModified()) {
        QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("VISIO STUDIO"),
                                                               tr("The document has been modified.\n"
                                                                  "Do you want to save your changes?"),
                                                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return on_actionSave_triggered(), true;
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    if (m_currentProject->load(fileName)) {
        // Mettre à jour l'interface
        CanvasView *canvasView = findChild<CanvasView*>("canvasView");
        if (canvasView) {
            canvasView->canvas()->setImage(m_currentProject->engine()->sourceImage());
        }

        statusBar()->showMessage(tr("File loaded"), 2000);
    } else {
        QMessageBox::warning(this, tr("Open Failed"),
                             tr("Failed to open the file %1").arg(fileName));
    }
}

bool MainWindow::saveFile(const QString &fileName)
{
    if (m_currentProject->save(fileName)) {
        statusBar()->showMessage(tr("File saved"), 2000);
        return true;
    } else {
        QMessageBox::warning(this, tr("Save Failed"),
                             tr("Failed to save the file %1").arg(fileName));
        return false;
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    // Cette méthode sera utilisée pour mettre à jour le chemin du fichier et l'historique récent
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
