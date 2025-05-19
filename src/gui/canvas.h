#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollArea>
#include <QGraphicsView>

/**
 * @brief Canvas pour afficher et manipuler l'image
 *
 * Widget personnalisé qui gère l'affichage de l'image avec zoom,
 * défilement et interaction de base.
 */
class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();

    // Gestion de l'image
    void setImage(const QImage &image);
    const QImage& image() const;

    // Zoom
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void setZoom(double factor);
    double zoomFactor() const;
    void handleWheelEvent(QWheelEvent *event) {
        wheelEvent(event);
    }

    // Grid
    void setShowGrid(bool show);
    bool isGridVisible() const;

    // Rulers
    void setShowRulers(bool show);
    bool areRulersVisible() const;

signals:
    void zoomChanged(double factor);
    void imageChanged();
    void mouseMoved(const QPoint &pos);

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QImage m_image;
    QPoint m_lastMousePos;
    double m_zoomFactor;
    bool m_showGrid;
    bool m_showRulers;
    bool m_isDragging;

    // Cache pour optimiser le rendu
    QPixmap m_cachedImage;
    double m_cachedZoom;
    bool m_isImageCached;

    // Helpers
    void drawCheckerboard(QPainter &painter, const QRect &rect);
    void drawGrid(QPainter &painter, const QRect &visibleRect);
    void drawRulers(QPainter &painter);
    void createCache();
    void clearCache();
    void updateCanvasSize();
    QPoint mapToImage(const QPoint &widgetPoint) const;
    QPoint mapFromImage(const QPoint &imagePoint) const;
};

/**
 * @brief ScrollArea qui contient le Canvas
 *
 * Extension de QScrollArea pour mieux gérer les interactions
 * avec le Canvas qu'il contient.
 */
class CanvasView : public QScrollArea
{
    Q_OBJECT

public:
    explicit CanvasView(QWidget *parent = nullptr);

    Canvas* canvas() const;

signals:
    void zoomChanged(double factor);
    void mouseMoved(const QPoint &pos);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void setDragMode(QGraphicsView::DragMode mode) {
        // Cette méthode est un stub pour la compatibilité avec le code existant
        // Elle sera implémentée correctement plus tard
    }

private:
    Canvas *m_canvas;
};

#endif // CANVAS_H
