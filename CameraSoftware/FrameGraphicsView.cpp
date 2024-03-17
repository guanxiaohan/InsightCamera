#include "FrameGraphicsView.h"

FrameGraphicsView::FrameGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setStyleSheet("background: rgb(0,0,0);");
    setDragMode(QGraphicsView::ScrollHandDrag);
}

FrameGraphicsView::~FrameGraphicsView()
{

}

void FrameGraphicsView::resetViewPort(int width, int height)
{
    this->scene()->setSceneRect(0, 0, width, height);
    centerOn(this->scene()->sceneRect().center());
}

void FrameGraphicsView::setFrameDraggable(bool val)
{
    nowDraggable = val;
    setDragMode(val? QGraphicsView::ScrollHandDrag: QGraphicsView::NoDrag);
}

void FrameGraphicsView::customScale(double ratio)
{
    if (scaledRatio + ratio < 0.99 || scaledRatio + ratio > 4) {
        return;
    }
    scaledRatio = scaledRatio + ratio;
    double calulatedRatio = scaledRatio * scaledRatio;
    QRectF nowRect = sceneRect();
    double scaleRatio = width() / nowRect.width();
    QTransform _transform = transform();
    _transform.setMatrix(1, _transform.m12(), _transform.m13(), 
        _transform.m21(), 1, _transform.m23(), 
        _transform.m31(), _transform.m32(), _transform.m33());
    setTransform(_transform);
    scale(calulatedRatio, calulatedRatio);
}

void FrameGraphicsView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);
    dragLastPoint = event->pos() - event->pos() + QPointF(width() / 2 / scaledRatio / scaledRatio, height() / 2 / scaledRatio / scaledRatio);
    dragLastPoint = dragLastPoint + mapToScene(0, 0);
    posAnchor = event->pos();
    isMousePressing = true;
}

void FrameGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
    QPointF offsetPos = event->pos() - posAnchor;
    offsetPos.setX(offsetPos.x() / scaledRatio / scaledRatio);
    offsetPos.setY(offsetPos.y() / scaledRatio / scaledRatio);
    if (isMousePressing && nowDraggable) {
        centerOn(dragLastPoint - offsetPos);
        scene()->update();
    }
}

void FrameGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);
    isMousePressing = false;
}

void FrameGraphicsView::wheelEvent(QWheelEvent* event)
{
    QPoint prev_viewPos = QPoint(event->position().x(), event->position().y());
    QPointF prev_scenePos = this->mapToScene(prev_viewPos);
    if (event->angleDelta().y() > 0) {
        customScale(0.07);
    }
    else {
        customScale(-0.07);
    }
    
}
