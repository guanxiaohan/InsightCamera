#include "FrameGraphicsView.h"

FrameGraphicsView::FrameGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setStyleSheet("background: rgb(120, 120, 120);");
    setDragMode(QGraphicsView::ScrollHandDrag);
    pen.setColor(QColor(255, 0, 0));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(4);
    eraser.setWidth(60);
    eraser.setColor(QColor(0, 0, 0, 50));
    eraser.setCapStyle(Qt::RoundCap);
    eraser.setJoinStyle(Qt::RoundJoin);
    frameState = FrameSelect;
}

FrameGraphicsView::~FrameGraphicsView()
{

}

void FrameGraphicsView::resetViewPort(int width, int height)
{
    this->scene()->setSceneRect(0, 0, width, height);
    centerOn(this->scene()->sceneRect().center());
}

void FrameGraphicsView::setFrameDraggable(nowState val)
{
    frameState = val;
    setDragMode(val == FrameSelect ? QGraphicsView::ScrollHandDrag : QGraphicsView::NoDrag);
}

void FrameGraphicsView::setEraserWidth(int width)
{
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
    isMousePressing = true;
    if (frameState == FrameSelect) {
        dragLastPoint = event->pos() - event->pos() + QPointF(width() / 2 / scaledRatio / scaledRatio, height() / 2 / scaledRatio / scaledRatio);
        dragLastPoint = dragLastPoint + mapToScene(0, 0);
        posAnchor = event->pos();
    }
    else if (frameState == FramePen) {
        if (event->button() == Qt::LeftButton) {
            dragLastPoint = event->pos();

            currentPenItem = new PenGraphicsItem(this);
            currentPenItem->setPen(pen);
            currentPenItem->setZValue(1);
            QPainterPath path = currentPenItem->path();
            path.moveTo(mapToScene(dragLastPoint.toPoint()));
            currentPenItem->setPath(path);
            scene()->addItem(currentPenItem);
        }
    }
    else if (frameState == FrameEraser) {
        if (event->button() == Qt::LeftButton) {
            dragLastPoint = event->pos();
            currentEraserItem = new EraserGraphicsItem(this);
            currentEraserItem->setPen(eraser);
            currentEraserItem->setZValue(1);
        }
    }
}

void FrameGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
    if (frameState == FrameSelect) {
        QPointF offsetPos = event->pos() - posAnchor;
        offsetPos.setX(offsetPos.x() / scaledRatio / scaledRatio);
        offsetPos.setY(offsetPos.y() / scaledRatio / scaledRatio);
        if (isMousePressing) {
            centerOn(dragLastPoint - offsetPos);
            scene()->update();
        }
    }
    else if (frameState == FramePen) {
        if (event->buttons() & Qt::LeftButton) {
            QPainterPath path = currentPenItem->path();
            path.lineTo(mapToScene(event->pos()));
            currentPenItem->setPath(path);
            dragLastPoint = event->pos();
        }
    }
    else if (frameState == FrameEraser) {
        if (event->buttons() & Qt::LeftButton) {
            QPainterPath path = currentEraserItem->path();
            path.moveTo(mapToScene(dragLastPoint.toPoint()));
            path.lineTo(mapToScene(event->pos()));
            currentEraserItem->setPath(path);
            dragLastPoint = event->pos();

            for (auto item : penItems) {
                if (currentEraserItem->collidesWithPath(item->path())) {
                    path = item->path();
                    // 0: initail 1: contained 2: new item
                    int nowInItemState = 0;
                    PenGraphicsItem* newItem = nullptr;
                    QPainterPath newPath = QPainterPath();
                    for (int i = 0; i < path.elementCount(); i++) {
                        auto element = path.elementAt(i);
                        if (currentEraserItem->contains(QPointF(element.x, element.y))) {
                            if (nowInItemState == 0) {
                                if (newPath.isEmpty()) {
                                    item->setPath(newPath);
                                }
                                else {
                                    item->setPath(newPath);
                                    newPath = QPainterPath();
                                }
                                nowInItemState = 1;
                                continue;
                            }
                            else if (nowInItemState == 1) {
                                continue;
                            }
                            else if (nowInItemState == 2) {
                                newItem->setPath(newPath);
                                nowInItemState = 1;
                                continue;
                            }
                        }
                        else {
                            if (nowInItemState == 0) {
                                if (newPath.isEmpty()) {
                                    newPath.moveTo(element.x + 0.001, element.y);
                                }
                                newPath.lineTo(element.x, element.y);
                                continue;
                            }
                            else if (nowInItemState == 1) {
                                newItem = new PenGraphicsItem(this);
                                newItem->setPen(pen);
                                penItems.append(newItem);
                                scene()->addItem(newItem);
                                newPath = QPainterPath();
                                newPath.moveTo(element.x, element.y);
                                nowInItemState = 2;
                            }
                            else if (nowInItemState == 2) {
                                newPath.lineTo(element.x, element.y);
                            }
                        }
                    }
                    if (nowInItemState == 0) {
                        
                    }
                    else if (nowInItemState == 1) {

                    }
                    else if (nowInItemState == 2) {
                        newItem->setPath(newPath);
                    }
                }
            }
        }
    }
}

void FrameGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);
    isMousePressing = false;
    if (event->button() == Qt::LeftButton) {
        dragLastPoint = QPoint();
        if (currentPenItem) {
            penItems.append(currentPenItem);
            currentPenItem = nullptr;
        }
        if (currentEraserItem) {
            // eraserItems.append(currentEraserItem);
            scene()->removeItem(currentEraserItem);
            delete currentEraserItem;
            currentEraserItem = nullptr;
        }
    }
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

void FrameGraphicsView::paintEvent(QPaintEvent* event)
{
    QGraphicsView::paintEvent(event);
}
