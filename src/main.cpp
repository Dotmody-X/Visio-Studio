#include "gui/mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDir>
#include "plugins/pluginmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Configuration de l'application
    app.setApplicationName("VISIO STUDIO");
    app.setOrganizationName("VISIO STUDIO Team");
    app.setOrganizationDomain("visiostudio.com");
    app.setApplicationVersion("0.1.0"); // Version de développement

    // Charger la feuille de style
    QFile styleFile(":/styles/main.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        app.setStyleSheet(style);
        qDebug() << "Stylesheet loaded successfully";
    } else {
        qWarning() << "Could not load stylesheet:" << styleFile.errorString();
    }

    // Configurer les répertoires de ressources
    // Ce sera utilisé plus tard pour les plugins, presets, etc.
    QDir appDir(QApplication::applicationDirPath());

    // S'assurer que les dossiers de plugins existent
    QString userDataPath = QDir::homePath() + "/Documents/VISIO_STUDIO";
    QDir().mkpath(userDataPath + "/plugins");
    QDir().mkpath(userDataPath + "/presets");

    // Charger les plugins
    PluginManager::instance()->loadPlugins();

    // Créer et afficher la fenêtre principale
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
