#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QVariantMap>
#include <memory>
#include "engine.h"

/**
 * @brief Classe gérant un projet VISIO STUDIO
 *
 * Cette classe encapsule toutes les données d'un projet, incluant
 * l'image source, la chaîne d'effets, et les métadonnées associées.
 * Elle gère également le chargement et la sauvegarde des projets.
 */
class Project : public QObject
{
    Q_OBJECT

public:
    explicit Project(QObject *parent = nullptr);
    ~Project();

    // Gestion de fichier
    bool save(const QString &filename = QString());
    bool load(const QString &filename);
    bool exportImage(const QString &filename, const QString &format = "PNG");

    // Création de projet
    bool newProject(int width, int height, const QColor &background = Qt::white);
    bool setSourceImage(const QImage &image);
    bool setSourceImage(const QString &imagePath);

    // Accès à l'engine
    Engine* engine() const;

    // Métadonnées du projet
    QString name() const;
    void setName(const QString &name);

    QString path() const;

    QSize dimensions() const;

    bool isModified() const;
    void setModified(bool modified = true);

    // Historique (undo/redo)
    bool canUndo() const;
    bool canRedo() const;
    void undo();
    void redo();

signals:
    void projectModified();
    void projectSaved();
    void projectLoaded();
    void projectNameChanged(const QString &name);
    void projectPathChanged(const QString &path);

private:
    QString m_name;
    QString m_path;
    bool m_isModified;
    std::unique_ptr<Engine> m_engine;

    // Format de fichier
    bool saveToVSD(const QString &filename);
    bool loadFromVSD(const QString &filename);

    // Historique (à implémenter plus tard)
    // QUndoStack *m_undoStack;
};

#endif // PROJECT_H
