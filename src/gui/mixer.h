#ifndef MIXER_H
#define MIXER_H

#include <QWidget>
#include <QList>
#include <QMap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QDrag>
#include <QMimeData>
#include "../core/engine.h"
#include "../plugins/plugininterface.h"

// Déclaration avancée
class EffectSlot;

/**
 * @brief Widget représentant la chaîne d'effets (Mixer)
 *
 * Ce widget affiche la chaîne d'effets dans un style FL Studio,
 * avec des slots d'effets qui peuvent être réorganisés par glisser-déposer.
 */
class Mixer : public QWidget
{
    Q_OBJECT

public:
    explicit Mixer(QWidget *parent = nullptr);

    // Gestion du moteur
    void setEngine(Engine *engine);

public slots:
    // Slots pour gérer les modifications de la chaîne d'effets
    void updateEffectChain();
    void addEffect(PluginInterface *plugin);
    void removeEffect(int id);
    void moveEffect(int id, int newPosition);
    void showEffectEditor(int id);
    void toggleEffect(int id, bool enabled);

    // Slots pour les signaux de l'engine
    void onEffectAdded(int id);
    void onEffectRemoved(int id);
    void onEffectMoved(int id, int newPosition);
    void onEffectParametersChanged(int id);
    void onEffectEnabledChanged(int id, bool enabled);

signals:
    void effectAdded(int id);
    void effectRemoved(int id);
    void effectMoved(int id, int newPosition);
    void effectEdited(int id);
    void effectToggled(int id, bool enabled);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    Engine *m_engine;
    QHBoxLayout *m_effectsLayout;
    QMap<int, EffectSlot*> m_effectSlots;
    EffectSlot *m_draggedSlot;
    QWidget *m_dropIndicator;

    // Helpers
    void clearEffectSlots();
    int findDropPosition(const QPoint &pos) const;
    void updateSlotPositions();
    void updateDropIndicator(const QPoint &pos);
    void animateSlot(int id, bool appearing);
};

/**
 * @brief Widget représentant un slot d'effet dans le Mixer
 *
 * Chaque slot affiche les informations basiques sur un effet
 * et permet d'interagir avec lui (activer/désactiver, éditer, etc.).
 */
class EffectSlot : public QWidget
{
    Q_OBJECT

    // Propriétés pour l'animation
    Q_PROPERTY(qreal windowOpacity READ windowOpacity WRITE setWindowOpacity)
    Q_PROPERTY(int maximumHeight READ maximumHeight WRITE setMaximumHeight)

public:
    explicit EffectSlot(int id, PluginInterface *plugin, QWidget *parent = nullptr);

    int id() const { return m_id; }
    QString name() const { return m_name; }

    void setEnabled(bool enabled);
    bool isEnabled() const;

    void updateStyle();

signals:
    void editClicked(int id);
    void removeClicked(int id);
    void enabledToggled(int id, bool enabled);
    void moveRequested(int id, int newPosition);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    int m_id;
    QString m_name;
    bool m_enabled;
    QPoint m_dragStartPosition;

    QVBoxLayout *m_layout;
    QLabel *m_nameLabel;
    QPushButton *m_editButton;
    QPushButton *m_removeButton;
    QPushButton *m_enableButton;

    void updateToggleButtonText();
};

#endif // MIXER_H
