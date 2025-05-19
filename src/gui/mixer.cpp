#include "mixer.h"
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QApplication>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

// ============ Mixer Implementation ============

Mixer::Mixer(QWidget *parent)
    : QWidget(parent),
    m_engine(nullptr),
    m_draggedSlot(nullptr),
    m_dropIndicator(new QWidget(this))
{
    // Créer le layout horizontal pour les effets
    m_effectsLayout = new QHBoxLayout(this);
    m_effectsLayout->setContentsMargins(10, 5, 10, 5);
    m_effectsLayout->setSpacing(5);

    // Ajouter un espace élastique à la fin
    m_effectsLayout->addStretch();

    // Configurer l'indicateur de drop
    m_dropIndicator->setFixedSize(5, 60);
    m_dropIndicator->setStyleSheet("background-color: #00FFFF;");
    m_dropIndicator->hide();

    // Activer le glisser-déposer
    setAcceptDrops(true);

    // Définir une hauteur minimale
    setMinimumHeight(80);

    // Définir la politique de taille
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Appliquer un style de fond
    setObjectName("mixerPanel");
}

void Mixer::setEngine(Engine *engine)
{
    if (m_engine == engine) return;

    if (m_engine) {
        // Déconnecter les signaux de l'ancien moteur
        disconnect(m_engine, &Engine::effectChainChanged, this, &Mixer::updateEffectChain);
        disconnect(m_engine, &Engine::effectAdded, this, &Mixer::updateEffectChain);
        disconnect(m_engine, &Engine::effectRemoved, this, &Mixer::updateEffectChain);
        disconnect(m_engine, &Engine::effectMoved, this, &Mixer::updateEffectChain);
        disconnect(m_engine, &Engine::effectParametersChanged, this, &Mixer::updateEffectChain);
        disconnect(m_engine, &Engine::effectEnabledChanged, this, &Mixer::updateEffectChain);
    }

    m_engine = engine;

    if (m_engine) {
        // Connecter les signaux du nouveau moteur
        connect(m_engine, &Engine::effectChainChanged, this, &Mixer::updateEffectChain);
        connect(m_engine, &Engine::effectAdded, this, &Mixer::onEffectAdded);
        connect(m_engine, &Engine::effectRemoved, this, &Mixer::onEffectRemoved);
        connect(m_engine, &Engine::effectMoved, this, &Mixer::onEffectMoved);
        connect(m_engine, &Engine::effectParametersChanged, this, &Mixer::onEffectParametersChanged);
        connect(m_engine, &Engine::effectEnabledChanged, this, &Mixer::onEffectEnabledChanged);

        // Mettre à jour l'affichage
        updateEffectChain();
    } else {
        // Vider l'affichage
        clearEffectSlots();
    }
}

void Mixer::updateEffectChain()
{
    if (!m_engine) return;

    // Récupérer la chaîne d'effets actuelle
    QList<Engine::EffectNode> effectChain = m_engine->effectChain();

    // Liste des IDs d'effets actuels
    QList<int> currentIds;
    for (const Engine::EffectNode &node : effectChain) {
        currentIds.append(node.id);
    }

    // Supprimer les slots qui ne sont plus dans la chaîne
    QList<int> idsToRemove;
    for (int id : m_effectSlots.keys()) {
        if (!currentIds.contains(id)) {
            idsToRemove.append(id);
        }
    }

    for (int id : idsToRemove) {
        EffectSlot *slot = m_effectSlots.take(id);
        m_effectsLayout->removeWidget(slot);
        slot->deleteLater();
    }

    // Ajouter/mettre à jour les slots pour chaque effet
    for (int i = 0; i < effectChain.size(); ++i) {
        const Engine::EffectNode &node = effectChain.at(i);

        if (!m_effectSlots.contains(node.id)) {
            // Créer un nouveau slot pour cet effet
            EffectSlot *slot = new EffectSlot(node.id, node.plugin, this);

            // Connecter les signaux
            connect(slot, &EffectSlot::editClicked, this, &Mixer::showEffectEditor);
            connect(slot, &EffectSlot::removeClicked, this, &Mixer::removeEffect);
            connect(slot, &EffectSlot::enabledToggled, this, &Mixer::toggleEffect);
            connect(slot, &EffectSlot::moveRequested, this, &Mixer::moveEffect);

            // Ajouter au layout (avant le stretch à la fin)
            m_effectsLayout->insertWidget(i, slot);

            // Stocker dans la map
            m_effectSlots[node.id] = slot;
        }

        // Mettre à jour l'état d'activation
        m_effectSlots[node.id]->setEnabled(node.enabled);
    }

    // Réorganiser les widgets selon l'ordre actuel de la chaîne
    updateSlotPositions();
}

void Mixer::addEffect(PluginInterface *plugin)
{
    if (!m_engine || !plugin) return;

    // Ajouter l'effet à la chaîne
    int id = m_engine->addEffect(plugin);

    // Émettre un signal
    if (id >= 0) {
        emit effectAdded(id);
    }
}

void Mixer::removeEffect(int id)
{
    if (!m_engine) return;

    // Supprimer l'effet de la chaîne
    if (m_engine->removeEffect(id)) {
        emit effectRemoved(id);
    }
}

void Mixer::moveEffect(int id, int newPosition)
{
    if (!m_engine) return;

    // Déplacer l'effet dans la chaîne
    if (m_engine->moveEffect(id, newPosition)) {
        emit effectMoved(id, newPosition);
    }
}

void Mixer::showEffectEditor(int id)
{
    // Émettre un signal pour ouvrir l'éditeur
    emit effectEdited(id);
}

void Mixer::toggleEffect(int id, bool enabled)
{
    if (!m_engine) return;

    // Activer/désactiver l'effet
    if (m_engine->enableEffect(id, enabled)) {
        emit effectToggled(id, enabled);
    }
}

void Mixer::onEffectAdded(int id)
{
    updateEffectChain();
    animateSlot(id, true);
}

void Mixer::onEffectRemoved(int id)
{
    if (m_effectSlots.contains(id)) {
        EffectSlot *slot = m_effectSlots[id];
        animateSlot(id, false);
    } else {
        updateEffectChain();
    }
}

void Mixer::onEffectMoved(int id, int newPosition)
{
    updateEffectChain();
    animateSlot(id, true);
}

void Mixer::onEffectParametersChanged(int id)
{
    // Mettre à jour simplement le slot concerné
    if (m_effectSlots.contains(id)) {
        EffectSlot *slot = m_effectSlots[id];
        // Si nécessaire, mettre à jour le slot ici
    }
}

void Mixer::onEffectEnabledChanged(int id, bool enabled)
{
    // Mettre à jour l'état d'activation du slot
    if (m_effectSlots.contains(id)) {
        EffectSlot *slot = m_effectSlots[id];
        slot->setEnabled(enabled);
    }
}

void Mixer::animateSlot(int id, bool appearing)
{
    if (!m_effectSlots.contains(id)) return;

    EffectSlot *slot = m_effectSlots[id];

    // Créer une animation pour l'opacité
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(slot, "windowOpacity");
    opacityAnimation->setDuration(300);

    // Créer une animation pour la hauteur
    QPropertyAnimation *heightAnimation = new QPropertyAnimation(slot, "maximumHeight");
    heightAnimation->setDuration(300);

    if (appearing) {
        // Animation d'apparition
        opacityAnimation->setStartValue(0.0);
        opacityAnimation->setEndValue(1.0);

        heightAnimation->setStartValue(0);
        heightAnimation->setEndValue(70);

        slot->setMaximumHeight(0);
        slot->setWindowOpacity(0.0);
    } else {
        // Animation de disparition
        opacityAnimation->setStartValue(1.0);
        opacityAnimation->setEndValue(0.0);

        heightAnimation->setStartValue(70);
        heightAnimation->setEndValue(0);

        // Après l'animation, supprimer définitivement le slot
        connect(opacityAnimation, &QPropertyAnimation::finished, this, [this, id]() {
            if (m_effectSlots.contains(id)) {
                EffectSlot *slot = m_effectSlots.take(id);
                m_effectsLayout->removeWidget(slot);
                slot->deleteLater();
            }
        });
    }

    // Groupe d'animations parallèles
    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    group->addAnimation(opacityAnimation);
    group->addAnimation(heightAnimation);

    // Nettoyer le groupe après son exécution
    connect(group, &QParallelAnimationGroup::finished, group, &QParallelAnimationGroup::deleteLater);

    // Démarrer l'animation
    group->start();
}

void Mixer::dragEnterEvent(QDragEnterEvent *event)
{
    // Accepter uniquement les glisser-déposer internes de type EffectSlot
    if (event->source() &&
        qobject_cast<EffectSlot*>(event->source()) &&
        event->mimeData()->hasFormat("application/x-visiostudio-effectslot")) {

        // Stocker le slot en cours de déplacement
        m_draggedSlot = qobject_cast<EffectSlot*>(event->source());

        // Afficher l'indicateur de drop
        updateDropIndicator(event->position().toPoint());
        m_dropIndicator->show();

        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void Mixer::dragMoveEvent(QDragMoveEvent *event)
{
    if (m_draggedSlot) {
        // Mettre à jour la position de l'indicateur de drop
        updateDropIndicator(event->position().toPoint());
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void Mixer::dragLeaveEvent(QDragLeaveEvent *event)
{
    // Cacher l'indicateur de drop
    m_dropIndicator->hide();
    m_draggedSlot = nullptr;
    event->accept();
}

void Mixer::dropEvent(QDropEvent *event)
{
    m_dropIndicator->hide();

    if (!m_engine || !m_draggedSlot) {
        m_draggedSlot = nullptr;
        event->ignore();
        return;
    }

    // Récupérer les données
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasFormat("application/x-visiostudio-effectslot")) {
        QByteArray encoded = mimeData->data("application/x-visiostudio-effectslot");
        QDataStream stream(&encoded, QIODevice::ReadOnly);
        int effectId;
        stream >> effectId;

        // Trouver la nouvelle position
        int newPosition = findDropPosition(event->position().toPoint());

        // Déplacer l'effet
        moveEffect(effectId, newPosition);

        event->acceptProposedAction();
    } else {
        event->ignore();
    }

    m_draggedSlot = nullptr;
}

void Mixer::updateDropIndicator(const QPoint &pos)
{
    int dropPosition = findDropPosition(pos);

    // Positionner l'indicateur de drop
    if (dropPosition == 0) {
        // Premier slot
        m_dropIndicator->move(m_effectsLayout->contentsMargins().left(), 10);
    } else if (dropPosition >= m_effectsLayout->count() - 1) {
        // Dernier slot (avant le stretch)
        int lastSlotIndex = m_effectsLayout->count() - 2;
        if (lastSlotIndex >= 0) {
            QLayoutItem *item = m_effectsLayout->itemAt(lastSlotIndex);
            QWidget *widget = item->widget();
            if (widget) {
                m_dropIndicator->move(widget->geometry().right() + 5, 10);
            } else {
                m_dropIndicator->hide();
                return;
            }
        } else {
            // Pas de slot, juste mettre au début
            m_dropIndicator->move(m_effectsLayout->contentsMargins().left(), 10);
        }
    } else {
        // Entre deux slots
        QLayoutItem *item = m_effectsLayout->itemAt(dropPosition - 1);
        QLayoutItem *nextItem = m_effectsLayout->itemAt(dropPosition);

        if (item && nextItem) {
            QWidget *widget = item->widget();
            QWidget *nextWidget = nextItem->widget();

            if (widget && nextWidget) {
                int x = (widget->geometry().right() + nextWidget->geometry().left()) / 2;
                m_dropIndicator->move(x, 10);
            } else {
                m_dropIndicator->hide();
                return;
            }
        } else {
            m_dropIndicator->hide();
            return;
        }
    }

    m_dropIndicator->show();
    m_dropIndicator->raise();
}

void Mixer::clearEffectSlots()
{
    // Supprimer tous les slots d'effets
    for (EffectSlot *slot : m_effectSlots.values()) {
        m_effectsLayout->removeWidget(slot);
        delete slot;
    }

    m_effectSlots.clear();
}

int Mixer::findDropPosition(const QPoint &pos) const
{
    // Trouver la position de drop en fonction de la position horizontale
    int position = 0;

    // Ignorer le dernier élément qui est le stretch
    for (int i = 0; i < m_effectsLayout->count() - 1; ++i) {
        QLayoutItem *item = m_effectsLayout->itemAt(i);
        QWidget *widget = item->widget();

        if (widget) {
            int centerX = widget->geometry().center().x();

            if (pos.x() < centerX) {
                return position;
            }

            position++;
        }
    }

    return position;
}

void Mixer::updateSlotPositions()
{
    if (!m_engine) return;

    // Récupérer la chaîne d'effets actuelle
    QList<Engine::EffectNode> effectChain = m_engine->effectChain();

    // Supprimer tous les widgets du layout (sauf le stretch à la fin)
    while (m_effectsLayout->count() > 1) {
        QLayoutItem *item = m_effectsLayout->takeAt(0);
        if (item->widget()) {
            m_effectsLayout->removeWidget(item->widget());
        }
        delete item;
    }

    // Réinsérer les widgets dans le bon ordre
    for (int i = 0; i < effectChain.size(); ++i) {
        int id = effectChain.at(i).id;

        if (m_effectSlots.contains(id)) {
            m_effectsLayout->insertWidget(i, m_effectSlots[id]);
        }
    }
}

// ============ EffectSlot Implementation ============

EffectSlot::EffectSlot(int id, PluginInterface *plugin, QWidget *parent)
    : QWidget(parent),
    m_id(id),
    m_enabled(true)
{
    if (plugin) {
        m_name = plugin->name();
    } else {
        m_name = tr("Unknown");
    }

    // Créer le layout
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(5, 5, 5, 5);
    m_layout->setSpacing(2);

    // Créer les contrôles
    m_nameLabel = new QLabel(m_name, this);
    m_nameLabel->setAlignment(Qt::AlignCenter);
    m_nameLabel->setFont(QFont("Segoe UI", 9, QFont::Bold));

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(2);

    m_editButton = new QPushButton(tr("Edit"), this);
    m_editButton->setToolTip(tr("Edit effect parameters"));
    m_editButton->setMaximumWidth(50);

    m_removeButton = new QPushButton(tr("X"), this);
    m_removeButton->setToolTip(tr("Remove effect"));
    m_removeButton->setMaximumWidth(30);

    m_enableButton = new QPushButton(tr("On"), this);
    m_enableButton->setToolTip(tr("Enable/disable effect"));
    m_enableButton->setMaximumWidth(50);
    m_enableButton->setCheckable(true);
    m_enableButton->setChecked(true);

    buttonLayout->addWidget(m_enableButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_removeButton);

    m_layout->addWidget(m_nameLabel);
    m_layout->addLayout(buttonLayout);

    // Configurer le widget
    setFixedSize(120, 70);
    setObjectName("effectSlot");
    updateStyle();

    // Connecter les signaux
    connect(m_editButton, &QPushButton::clicked, this, [this]() {
        emit editClicked(m_id);
    });

    connect(m_removeButton, &QPushButton::clicked, this, [this]() {
        emit removeClicked(m_id);
    });

    connect(m_enableButton, &QPushButton::toggled, this, [this](bool checked) {
        m_enabled = checked;
        updateToggleButtonText();
        updateStyle();
        emit enabledToggled(m_id, checked);
    });

    // Activer le glisser-déposer
    setAcceptDrops(true);
}

void EffectSlot::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        m_enableButton->setChecked(enabled);
        updateToggleButtonText();
        updateStyle();
    }
}

bool EffectSlot::isEnabled() const
{
    return m_enabled;
}

void EffectSlot::updateStyle()
{
    // Appliquer un style différent selon l'état d'activation
    if (m_enabled) {
        setStyleSheet("EffectSlot { background-color: #333333; border: 1px solid #444444; border-radius: 3px; border-left: 3px solid #00FFFF; }");
    } else {
        setStyleSheet("EffectSlot { background-color: #222222; border: 1px solid #333333; border-radius: 3px; border-left: 3px solid #666666; color: #888888; }");
    }
}

void EffectSlot::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->position().toPoint();
    }
}

void EffectSlot::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }

    if ((event->position().toPoint() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    // Créer un drag
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    // Encoder l'ID de l'effet
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << m_id;

    mimeData->setData("application/x-visiostudio-effectslot", itemData);
    drag->setMimeData(mimeData);

    // Créer une représentation visuelle du drag
    QPixmap pixmap(size());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setOpacity(0.7);
    render(&painter);

    drag->setPixmap(pixmap);
    drag->setHotSpot(event->position().toPoint() - rect().topLeft());

    // Cacher temporairement ce slot pendant le drag
    hide();

    // Exécuter le drag
    Qt::DropAction dropAction = drag->exec(Qt::MoveAction);

    // Réafficher le slot
    show();

    // Ne pas faire de traitement supplémentaire
    event->accept();
}

void EffectSlot::paintEvent(QPaintEvent *event)
{
    // Dessiner le fond
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    // Laisser le reste du dessin au QWidget
    QWidget::paintEvent(event);
}

void EffectSlot::updateToggleButtonText()
{
    m_enableButton->setText(m_enabled ? tr("On") : tr("Off"));
}
