#include "canvas.h"
#include <QPainter>
#include <QScrollBar>
#include <QDebug>
#include <cmath>
#include <QApplication>

// ============ Canvas Implementation ============

Canvas::Canvas(QWidget *parent)
    : QWidget(parent),
    m_zoomFactor(1.0),
    m_showGrid(false),
    m_showRulers(false),
    m_isDragging(false),
    m_isImageCached(false)
{
    // Permettre le suivi de la souris pour les événements mouseMoveEvent
    setMouseTracking(true);

    // Configurer le widget pour accepter le focus
    setFocusPolicy(Qt::StrongFocus);

    // Définir une taille minimale
    setMinimumSize(400, 300);

    // Définir une politique de taille pour s'assurer que le widget peut grandir
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Activer le double buffer pour éviter le scintillement
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
}

Canvas::~Canvas()
{
    // Libérer les ressources
    clearCache();
}

void Canvas::setImage(const QImage &image)
{
    m_image = image;
    clearCache();
    updateCanvasSize();
    update();
    emit imageChanged();
}

const QImage& Canvas::image() const
{
    return m_image;
}

void Canvas::zoomIn()
{
    setZoom(m_zoomFactor * 1.25);
}

void Canvas::zoomOut()
{
    setZoom(m_zoomFactor * 0.8);
}

void Canvas::resetZoom()
{
    setZoom(1.0);
}

void Canvas::setZoom(double factor)
{
    // Limiter le facteur de zoom entre 0.1 et 10
    factor = qBound(0.1, factor, 10.0);

    if (m_zoomFactor != factor) {
        m_zoomFactor = factor;
        clearCache();
        updateCanvasSize();
        update();
        emit zoomChanged(m_zoomFactor);
    }
}

double Canvas::zoomFactor() const
{
    return m_zoomFactor;
}

void Canvas::setShowGrid(bool show)
{
    if (m_showGrid != show) {
        m_showGrid = show;
        update();
    }
}

bool Canvas::isGridVisible() const
{
    return m_showGrid;
}

void Canvas::setShowRulers(bool show)
{
    if (m_showRulers != show) {
        m_showRulers = show;
        update();
    }
}

bool Canvas::areRulersVisible() const
{
    return m_showRulers;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Définir la région de dessin pour n'afficher que la partie visible
    painter.setClipRect(event->rect());

    // Appliquer l'antialiasing pour un rendu plus doux
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Remplir l'arrière-plan avec un motif à damier pour indiquer la transparence
    drawCheckerboard(painter, event->rect());

    // Si l'image est vide, afficher un message
    if (m_image.isNull()) {
        painter.drawText(rect(), Qt::AlignCenter, tr("No image loaded"));
        return;
    }

    // Calculer le rectangle pour l'image
    int scaledWidth = m_image.width() * m_zoomFactor;
    int scaledHeight = m_image.height() * m_zoomFactor;
    QRect targetRect = QRect(0, 0, scaledWidth, scaledHeight);

    // N'afficher que la partie visible de l'image
    QRect visibleRect = event->rect().intersected(targetRect);
    if (!visibleRect.isEmpty()) {
        // Utiliser un cache pour améliorer les performances
        if (!m_isImageCached || m_cachedZoom != m_zoomFactor) {
            createCache();
        }

        // Dessiner l'image depuis le cache
        if (m_isImageCached) {
            painter.drawPixmap(visibleRect, m_cachedImage, visibleRect);
        } else {
            // Fallback en cas d'échec du cache
            QRect sourceRect(
                visibleRect.x() / m_zoomFactor,
                visibleRect.y() / m_zoomFactor,
                visibleRect.width() / m_zoomFactor,
                visibleRect.height() / m_zoomFactor
                );

            painter.drawImage(visibleRect, m_image, sourceRect);
        }
    }

    // Dessiner la grille si activée
    if (m_showGrid) {
        drawGrid(painter, visibleRect);
    }

    // Dessiner les règles si activées
    if (m_showRulers) {
        drawRulers(painter);
    }
}

void Canvas::wheelEvent(QWheelEvent *event)
{
    // Zoom avec la molette de la souris tout en maintenant Ctrl
    if (event->modifiers() & Qt::ControlModifier) {
        QPoint numPixels = event->pixelDelta();
        QPoint numDegrees = event->angleDelta() / 8;

        if (!numPixels.isNull()) {
            // Pour les trackpads à haute résolution
            if (numPixels.y() > 0) {
                zoomIn();
            } else {
                zoomOut();
            }
        } else if (!numDegrees.isNull()) {
            // Pour les souris standard
            if (numDegrees.y() > 0) {
                zoomIn();
            } else {
                zoomOut();
            }
        }

        event->accept();
    } else {
        // Laisser l'événement se propager au parent (scroll area)
        event->ignore();
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton ||
        (event->button() == Qt::LeftButton && (event->modifiers() & Qt::AltModifier))) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        event->ignore();
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mousePos = event->pos();

    // Si le glisser-déposer est en cours, déplacer la vue
    if (m_isDragging) {
        QScrollArea *scrollArea = qobject_cast<QScrollArea*>(parent()->parent());
        if (scrollArea) {
            QPoint delta = mousePos - m_lastMousePos;
            scrollArea->horizontalScrollBar()->setValue(
                scrollArea->horizontalScrollBar()->value() - delta.x());
            scrollArea->verticalScrollBar()->setValue(
                scrollArea->verticalScrollBar()->value() - delta.y());
            m_lastMousePos = mousePos;
        }
    }

    // Émettre un signal avec la position de la souris dans les coordonnées de l'image
    QPoint imagePos = mapToImage(mousePos);
    if (rect().contains(mousePos) && !m_image.isNull() &&
        imagePos.x() >= 0 && imagePos.y() >= 0 &&
        imagePos.x() < m_image.width() && imagePos.y() < m_image.height()) {
        emit mouseMoved(imagePos);
    }

    event->accept();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isDragging && (event->button() == Qt::MiddleButton ||
                         (event->button() == Qt::LeftButton && (event->modifiers() & Qt::AltModifier)))) {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    } else {
        event->ignore();
    }
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    // Lors d'un redimensionnement, nous devons vider le cache
    clearCache();
    QWidget::resizeEvent(event);
}

void Canvas::drawCheckerboard(QPainter &painter, const QRect &rect)
{
    // Dessiner un damier pour représenter la transparence
    QColor dark(100, 100, 100);
    QColor light(140, 140, 140);

    int size = 10;
    int startX = (rect.x() / size) * size;
    int startY = (rect.y() / size) * size;
    int endX = rect.right();
    int endY = rect.bottom();

    for (int x = startX; x <= endX; x += size) {
        for (int y = startY; y <= endY; y += size) {
            QRect tileRect(x, y, size, size);
            painter.fillRect(tileRect, ((x / size) + (y / size)) % 2 ? dark : light);
        }
    }
}

void Canvas::drawGrid(QPainter &painter, const QRect &visibleRect)
{
    if (m_image.isNull()) return;

    // Définir le pas de la grille en fonction du zoom
    int gridStep = qMax(10, int(20 / m_zoomFactor));

    // Définir le style de ligne pour la grille
    QPen gridPen(QColor(50, 50, 50, 100));
    gridPen.setStyle(Qt::DotLine);
    painter.setPen(gridPen);

    // Calculer les limites de la grille visible
    int startX = (visibleRect.x() / gridStep) * gridStep;
    int startY = (visibleRect.y() / gridStep) * gridStep;
    int endX = visibleRect.right();
    int endY = visibleRect.bottom();

    // Dessiner les lignes verticales
    for (int x = startX; x <= endX; x += gridStep) {
        int scaledX = x * m_zoomFactor;
        if (scaledX >= visibleRect.left() && scaledX <= visibleRect.right()) {
            painter.drawLine(scaledX, visibleRect.top(), scaledX, visibleRect.bottom());
        }
    }

    // Dessiner les lignes horizontales
    for (int y = startY; y <= endY; y += gridStep) {
        int scaledY = y * m_zoomFactor;
        if (scaledY >= visibleRect.top() && scaledY <= visibleRect.bottom()) {
            painter.drawLine(visibleRect.left(), scaledY, visibleRect.right(), scaledY);
        }
    }
}

void Canvas::drawRulers(QPainter &painter)
{
    if (m_image.isNull()) return;

    int scaledWidth = m_image.width() * m_zoomFactor;
    int scaledHeight = m_image.height() * m_zoomFactor;

    // Définir l'épaisseur des règles
    int rulerThickness = 20;

    // Dessiner le fond des règles
    painter.fillRect(0, 0, scaledWidth, rulerThickness, QColor(30, 30, 30, 200));
    painter.fillRect(0, 0, rulerThickness, scaledHeight, QColor(30, 30, 30, 200));

    // Dessiner les graduations
    painter.setPen(Qt::white);

    // Règle horizontale
    int step = qMax(10, int(50 / m_zoomFactor));
    for (int x = 0; x <= m_image.width(); x += step) {
        int scaledX = x * m_zoomFactor;
        int tickHeight = (x % 100 == 0) ? 10 : ((x % 50 == 0) ? 7 : 5);
        painter.drawLine(scaledX, 0, scaledX, tickHeight);

        if (x % 100 == 0) {
            painter.drawText(scaledX + 2, 15, QString::number(x));
        }
    }

    // Règle verticale
    for (int y = 0; y <= m_image.height(); y += step) {
        int scaledY = y * m_zoomFactor;
        int tickWidth = (y % 100 == 0) ? 10 : ((y % 50 == 0) ? 7 : 5);
        painter.drawLine(0, scaledY, tickWidth, scaledY);

        if (y % 100 == 0) {
            painter.drawText(2, scaledY + 15, QString::number(y));
        }
    }
}

void Canvas::createCache()
{
    // Ne pas créer de cache pour des images trop grandes
    if (m_image.isNull() || m_image.width() * m_image.height() > 4000 * 4000) {
        clearCache();
        return;
    }

    // Créer une image mise à l'échelle pour le cache
    QImage scaledImage = m_image.scaled(
        m_image.width() * m_zoomFactor,
        m_image.height() * m_zoomFactor,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );

    if (!scaledImage.isNull()) {
        m_cachedImage = QPixmap::fromImage(scaledImage);
        m_cachedZoom = m_zoomFactor;
        m_isImageCached = true;
    } else {
        clearCache();
    }
}

void Canvas::clearCache()
{
    m_cachedImage = QPixmap();
    m_isImageCached = false;
}

void Canvas::updateCanvasSize()
{
    if (m_image.isNull()) {
        // Taille minimale si pas d'image
        setFixedSize(400, 300);
    } else {
        // Taille basée sur l'image et le zoom
        int width = m_image.width() * m_zoomFactor;
        int height = m_image.height() * m_zoomFactor;
        setFixedSize(width, height);
    }
}

QPoint Canvas::mapToImage(const QPoint &widgetPoint) const
{
    if (m_image.isNull()) return QPoint();

    // Convertir les coordonnées du widget en coordonnées de l'image
    int imageX = widgetPoint.x() / m_zoomFactor;
    int imageY = widgetPoint.y() / m_zoomFactor;

    // S'assurer que les coordonnées sont dans les limites de l'image
    imageX = qBound(0, imageX, m_image.width() - 1);
    imageY = qBound(0, imageY, m_image.height() - 1);

    return QPoint(imageX, imageY);
}

QPoint Canvas::mapFromImage(const QPoint &imagePoint) const
{
    // Convertir les coordonnées de l'image en coordonnées du widget
    int widgetX = imagePoint.x() * m_zoomFactor;
    int widgetY = imagePoint.y() * m_zoomFactor;

    return QPoint(widgetX, widgetY);
}

// ============ CanvasView Implementation ============

CanvasView::CanvasView(QWidget *parent)
    : QScrollArea(parent)
{
    // Créer le canvas
    m_canvas = new Canvas(this);
    setWidget(m_canvas);

    // Configurer le comportement du scroll area
    setAlignment(Qt::AlignCenter);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setWidgetResizable(false);

    // Connecter les signaux
    connect(m_canvas, &Canvas::zoomChanged, this, &CanvasView::zoomChanged);
    connect(m_canvas, &Canvas::mouseMoved, this, &CanvasView::mouseMoved);

    // Configuration esthétique
    setFrameShape(QFrame::NoFrame);

    // Éviter le scintillement lors du défilement
    // setViewportUpdateMode(QAbstractScrollArea::MinimalUpdate);
}

Canvas* CanvasView::canvas() const
{
    return m_canvas;
}

void CanvasView::wheelEvent(QWheelEvent *event)
{
    // Si Ctrl est appuyé, gérer le zoom
    if (event->modifiers() & Qt::ControlModifier) {
        // Le Canvas gère le zoom, nous passons simplement l'événement
        m_canvas->handleWheelEvent(event);  // <- LIGNE REMPLACÉE
    } else {
        // Sinon, traiter comme un événement de défilement normal
        QScrollArea::wheelEvent(event);
    }
}

void CanvasView::keyPressEvent(QKeyEvent *event)
{
    // Raccourcis clavier
    if (event->key() == Qt::Key_Plus && (event->modifiers() & Qt::ControlModifier)) {
        m_canvas->zoomIn();
        event->accept();
    } else if (event->key() == Qt::Key_Minus && (event->modifiers() & Qt::ControlModifier)) {
        m_canvas->zoomOut();
        event->accept();
    } else if (event->key() == Qt::Key_0 && (event->modifiers() & Qt::ControlModifier)) {
        m_canvas->resetZoom();
        event->accept();
    } else if (event->key() == Qt::Key_Space) {
        // Basculer temporairement en mode déplacement
        setDragMode(QGraphicsView::ScrollHandDrag);
        event->accept();
    } else {
        QScrollArea::keyPressEvent(event);
    }
}
