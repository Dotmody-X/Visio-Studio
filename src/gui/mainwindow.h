#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QLabel>
#include "../core/project.h"
#include "../plugins/plugininterface.h"
#include "../ai/generator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Fenêtre principale de VISIO STUDIO
 *
 * Cette classe gère l'interface utilisateur principale de l'application,
 * s'inspirant de FL Studio avec une zone centrale et des panneaux modulaires.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // Actions du menu Fichier
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionExport_triggered();
    void on_actionExit_triggered();

    // Actions du menu Édition
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();

    // Actions pour la gestion des effets
    void on_actionAddEffect_triggered();
    void on_actionRemoveEffect_triggered();

    // Slots pour la mise à jour de l'UI
    void updateWindowTitle();
    void updateUI();
    void updateEffectChain();

    // Gestion du projet
    void projectModified();
    void projectSaved();
    void projectLoaded();

    // Traitement d'image
    void processingStarted();
    void processingStopped();
    void processingProgress(int percent);
    void on_actionAIGenerator_triggered();
    void processingCanceled();
    void processingFailed(const QString &errorMessage);


private:
    Ui::MainWindow *ui;
    std::unique_ptr<Project> m_currentProject;
    QLabel *m_statusLabel;

    // Méthodes d'initialisation
    void setupUI();
    void createActions();
    void createMenus();
    void createToolbars();
    void createStatusBar();
    void createConnections();

    // Gestion des plugins
    void showPluginSelector();
    void showPluginEditor(int effectId);

    // Helpers
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
};

#endif // MAINWINDOW_H
