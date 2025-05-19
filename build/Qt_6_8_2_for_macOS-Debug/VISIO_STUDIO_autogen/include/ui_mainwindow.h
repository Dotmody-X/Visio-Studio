/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionExit;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionPreferences;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionZoomReset;
    QAction *actionShowGrid;
    QAction *actionShowRulers;
    QAction *actionAddEffect;
    QAction *actionRemoveEffect;
    QAction *actionManagePresets;
    QAction *actionAIGenerator;
    QAction *actionTextTool;
    QAction *actionWebsite;
    QAction *actionDocumentation;
    QAction *actionAbout;
    QAction *actionExport;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuEffects;
    QMenu *menuTools;
    QMenu *menuHelp;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1280, 800);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName("actionNew");
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName("actionSaveAs");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName("actionUndo");
        actionUndo->setEnabled(false);
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName("actionRedo");
        actionRedo->setEnabled(false);
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName("actionCut");
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName("actionCopy");
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName("actionPaste");
        actionPreferences = new QAction(MainWindow);
        actionPreferences->setObjectName("actionPreferences");
        actionZoomIn = new QAction(MainWindow);
        actionZoomIn->setObjectName("actionZoomIn");
        actionZoomOut = new QAction(MainWindow);
        actionZoomOut->setObjectName("actionZoomOut");
        actionZoomReset = new QAction(MainWindow);
        actionZoomReset->setObjectName("actionZoomReset");
        actionShowGrid = new QAction(MainWindow);
        actionShowGrid->setObjectName("actionShowGrid");
        actionShowGrid->setCheckable(true);
        actionShowRulers = new QAction(MainWindow);
        actionShowRulers->setObjectName("actionShowRulers");
        actionShowRulers->setCheckable(true);
        actionAddEffect = new QAction(MainWindow);
        actionAddEffect->setObjectName("actionAddEffect");
        actionRemoveEffect = new QAction(MainWindow);
        actionRemoveEffect->setObjectName("actionRemoveEffect");
        actionManagePresets = new QAction(MainWindow);
        actionManagePresets->setObjectName("actionManagePresets");
        actionAIGenerator = new QAction(MainWindow);
        actionAIGenerator->setObjectName("actionAIGenerator");
        actionTextTool = new QAction(MainWindow);
        actionTextTool->setObjectName("actionTextTool");
        actionWebsite = new QAction(MainWindow);
        actionWebsite->setObjectName("actionWebsite");
        actionDocumentation = new QAction(MainWindow);
        actionDocumentation->setObjectName("actionDocumentation");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        actionExport = new QAction(MainWindow);
        actionExport->setObjectName("actionExport");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1280, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName("menuEdit");
        menuView = new QMenu(menubar);
        menuView->setObjectName("menuView");
        menuEffects = new QMenu(menubar);
        menuEffects->setObjectName("menuEffects");
        menuTools = new QMenu(menubar);
        menuTools->setObjectName("menuTools");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        toolBar->setMovable(false);
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuEffects->menuAction());
        menubar->addAction(menuTools->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSaveAs);
        menuFile->addSeparator();
        menuFile->addAction(actionExport);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionPreferences);
        menuView->addAction(actionZoomIn);
        menuView->addAction(actionZoomOut);
        menuView->addAction(actionZoomReset);
        menuView->addSeparator();
        menuView->addAction(actionShowGrid);
        menuView->addAction(actionShowRulers);
        menuEffects->addAction(actionAddEffect);
        menuEffects->addAction(actionRemoveEffect);
        menuEffects->addSeparator();
        menuEffects->addAction(actionManagePresets);
        menuTools->addAction(actionAIGenerator);
        menuTools->addAction(actionTextTool);
        menuHelp->addAction(actionWebsite);
        menuHelp->addAction(actionDocumentation);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout);
        toolBar->addAction(actionNew);
        toolBar->addAction(actionOpen);
        toolBar->addAction(actionSave);
        toolBar->addSeparator();
        toolBar->addAction(actionUndo);
        toolBar->addAction(actionRedo);
        toolBar->addSeparator();
        toolBar->addAction(actionAddEffect);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "VISIO STUDIO", nullptr));
        actionNew->setText(QCoreApplication::translate("MainWindow", "New", nullptr));
#if QT_CONFIG(shortcut)
        actionNew->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Open...", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSaveAs->setText(QCoreApplication::translate("MainWindow", "Save As...", nullptr));
#if QT_CONFIG(shortcut)
        actionSaveAs->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionUndo->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
#if QT_CONFIG(shortcut)
        actionUndo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        actionRedo->setText(QCoreApplication::translate("MainWindow", "Redo", nullptr));
#if QT_CONFIG(shortcut)
        actionRedo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCut->setText(QCoreApplication::translate("MainWindow", "Cut", nullptr));
#if QT_CONFIG(shortcut)
        actionCut->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+X", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCopy->setText(QCoreApplication::translate("MainWindow", "Copy", nullptr));
#if QT_CONFIG(shortcut)
        actionCopy->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPaste->setText(QCoreApplication::translate("MainWindow", "Paste", nullptr));
#if QT_CONFIG(shortcut)
        actionPaste->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+V", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPreferences->setText(QCoreApplication::translate("MainWindow", "Preferences...", nullptr));
        actionZoomIn->setText(QCoreApplication::translate("MainWindow", "Zoom In", nullptr));
#if QT_CONFIG(shortcut)
        actionZoomIn->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl++", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoomOut->setText(QCoreApplication::translate("MainWindow", "Zoom Out", nullptr));
#if QT_CONFIG(shortcut)
        actionZoomOut->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+-", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoomReset->setText(QCoreApplication::translate("MainWindow", "Reset Zoom", nullptr));
#if QT_CONFIG(shortcut)
        actionZoomReset->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+0", nullptr));
#endif // QT_CONFIG(shortcut)
        actionShowGrid->setText(QCoreApplication::translate("MainWindow", "Show Grid", nullptr));
        actionShowRulers->setText(QCoreApplication::translate("MainWindow", "Show Rulers", nullptr));
        actionAddEffect->setText(QCoreApplication::translate("MainWindow", "Add Effect...", nullptr));
        actionRemoveEffect->setText(QCoreApplication::translate("MainWindow", "Remove Effect", nullptr));
        actionManagePresets->setText(QCoreApplication::translate("MainWindow", "Manage Presets...", nullptr));
        actionAIGenerator->setText(QCoreApplication::translate("MainWindow", "AI Generator...", nullptr));
        actionTextTool->setText(QCoreApplication::translate("MainWindow", "Text Tool", nullptr));
        actionWebsite->setText(QCoreApplication::translate("MainWindow", "Website", nullptr));
        actionDocumentation->setText(QCoreApplication::translate("MainWindow", "Documentation", nullptr));
#if QT_CONFIG(shortcut)
        actionDocumentation->setShortcut(QCoreApplication::translate("MainWindow", "F1", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About VISIO STUDIO", nullptr));
        actionExport->setText(QCoreApplication::translate("MainWindow", "Export...", nullptr));
#if QT_CONFIG(shortcut)
        actionExport->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "View", nullptr));
        menuEffects->setTitle(QCoreApplication::translate("MainWindow", "Effects", nullptr));
        menuTools->setTitle(QCoreApplication::translate("MainWindow", "Tools", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
