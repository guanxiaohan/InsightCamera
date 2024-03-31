#include "FrameGraphicsView.h"

FrameGraphicsView::FrameGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setStyleSheet("background: rgb(120, 120, 120);");
    setDragMode(QGraphicsView::ScrollHandDrag);
    setAttribute(Qt::WA_AcceptTouchEvents);
    pen.setColor(QColor(255, 0, 0));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(4);
    eraser.setWidth(60);
    eraser.setColor(QColor(0, 0, 0, 50));
    eraser.setCapStyle(Qt::RoundCap);
    eraser.setJoinStyle(Qt::RoundJoin);
    frameState = FrameSelect;

    holdTimer = new QTimer();
    holdTimer->setSingleShot(true);
    connect(holdTimer, &QTimer::timeout, this, &FrameGraphicsView::recognizePen);
}

FrameGraphicsView::~FrameGraphicsView()
{
    delete holdTimer;
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
    if (frameState == FrameEraser) {
        QPixmap pixmap = QPixmap(":/MainInterface/ErasorCursorIcon");
        pixmap = pixmap.scaled(eraser.width(), eraser.width());
        QCursor cursor = QCursor(pixmap);
        setCursor(cursor);
    }
    else {
        QCursor cursor = QCursor();
        setCursor(cursor);
    }
}

void FrameGraphicsView::setEraserWidth(int width)
{
    eraser.setWidth(width);
    if (frameState == FrameEraser) {
        QPixmap pixmap = QPixmap(":/MainInterface/ErasorCursorIcon");
        pixmap = pixmap.scaled(width, width);
        QCursor cursor = QCursor(pixmap);
        setCursor(cursor);
    }
}

void FrameGraphicsView::setPenWidth(int width)
{
    pen.setWidth(width);
}

void FrameGraphicsView::setPenColor(QColor color)
{
    pen.setColor(color);
}

void FrameGraphicsView::clearFrame()
{
    for (auto item : scene()->items()) {
        if (item->type() == 1145) {
            scene()->removeItem(item);
            penItems.removeOne(item);
            delete item;
        }
    }
    undoStack.clear();
}

void FrameGraphicsView::clearAllFrame()
{
    for (auto item : penItems) {
        if (item->scene()) {
            item->scene()->removeItem(item);
        }
        delete item;
    }
    penItems.clear();
    undoStack.clear();
}

void FrameGraphicsView::undoProcess()
{
    if (undoStack.isEmpty()) {
        return;
    }
    auto process = undoStack.last();
    if (process.type == UndoItem::UndoPen) {
        process.drewPenItem->scene()->removeItem(process.drewPenItem);
        penItems.removeOne(process.drewPenItem);
        delete process.drewPenItem;
        undoStack.removeLast();
    }
    else if (process.type == UndoItem::UndoEraser) {
        do {
            auto process = undoStack.last();
            for (auto i : process.erasedPenQueue) {
                i->scene()->removeItem(i);
                penItems.removeOne(i);
                delete i;
            }
            process.drewPenItem->setPath(process.erasedPenPath);
            undoStack.removeLast();
        } while (undoStack.last().type == UndoItem::UndoEraser);
    }
    else if (process.type == UndoItem::UndoSplitter) {
        undoStack.removeLast();
        do {
            auto process = undoStack.last();
            for (auto i : process.erasedPenQueue) {
                i->scene()->removeItem(i);
                penItems.removeOne(i);
                delete i;
            }
            process.drewPenItem->setPath(process.erasedPenPath);
            undoStack.removeLast();
        } while (undoStack.last().type == UndoItem::UndoEraser);
    }
}

void FrameGraphicsView::recognizePen()
{
    if (not currentPenItem) {
        return;
    }

    auto path = currentPenItem->path();
    int elementCount = path.elementCount();
    auto x1 = path.elementAt(0).x;
    auto y1 = path.elementAt(0).y;
    auto x2 = path.elementAt(elementCount - 1).x;
    auto y2 = path.elementAt(elementCount - 1).y;
    for (int i = 0; i < elementCount; i++) {
        auto x0 = path.elementAt(i).x;
        auto y0 = path.elementAt(i).y;
        auto distance = fabs(((y1 - y2) * x0 - (x1 - x2) * y0 + (x1 * y2 - x2 * y1)) / sqrt(pow(y1 - y2, 2) + pow(x1 - x2, 2)));
        if (distance > 45) {
            return;
        }
    }
    auto newStraightItem = new PenGraphicsItem(this);
    newStraightItem->setPen(pen);
    QPainterPath newPath = QPainterPath();
    newPath.moveTo(x1 + 0.001, y1);
    newPath.lineTo(x1, y1);
    for (int i = 0; i < 20; i++) {
        newPath.lineTo(x1 + (x2 - x1) / 20 * (i + 1), y1 + (y2 - y1) / 20 * (i + 1));
    }
    newStraightItem->setPath(newPath);
    scene()->addItem(newStraightItem);
    penItems.append(newStraightItem);
    undoStack.removeLast();
    undoStack.append(newStraightItem);
    penItems.removeOne(currentPenItem);
    scene()->removeItem(currentPenItem);
    delete currentPenItem;
    currentPenItem = nullptr;
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
    if (event->source() != Qt::MouseEventNotSynthesized) {
        //event->ignore();
        return;
    }
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
            path.lineTo(mapToScene(dragLastPoint.toPoint()) + QPointF(0.001, 0));
            currentPenItem->setPath(path);
            scene()->addItem(currentPenItem);
            penItems.append(currentPenItem);
            undoStack.append(UndoItem(currentPenItem));
        }
    }
    else if (frameState == FrameEraser) {
        if (event->button() == Qt::LeftButton) {
            dragLastPoint = event->pos();
            currentEraserItem = new EraserGraphicsItem(this);
            scene()->addItem(currentEraserItem);
            currentEraserItem->setPen(eraser);
            currentEraserItem->setZValue(1); 
            QPainterPath path = currentEraserItem->path();
            path.moveTo(mapToScene(dragLastPoint.toPoint()));
            path.lineTo(mapToScene(event->pos()) + QPointF(0.001, 0));
            currentEraserItem->setPath(path);
        }
    }
}

void FrameGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if (event->source() != Qt::MouseEventNotSynthesized) {
        //event->ignore();
        return;
    }
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
            if (not currentPenItem) {
                dragLastPoint = event->pos();
                currentPenItem = new PenGraphicsItem(this);
                currentPenItem->setPen(pen);
                currentPenItem->setZValue(1);
                QPainterPath path = currentPenItem->path();
                path.moveTo(mapToScene(dragLastPoint.toPoint()));
                path.lineTo(mapToScene(dragLastPoint.toPoint()) + QPointF(0.001, 0));
                currentPenItem->setPath(path);
                scene()->addItem(currentPenItem); 
                penItems.append(currentPenItem);
                undoStack.append(UndoItem(currentPenItem));
            }
            else {
                QPainterPath path = currentPenItem->path();
                path.lineTo(mapToScene(event->pos()));
                currentPenItem->setPath(path);
                dragLastPoint = event->pos();
                holdTimer->stop();
                holdTimer->start(700);
            }
        }
    }
    else if (frameState == FrameEraser) {
        if (event->buttons() & Qt::LeftButton) {
            // Create Eraser item or extend its path
            if (not currentEraserItem) {
                dragLastPoint = event->pos();
                currentEraserItem = new EraserGraphicsItem(this);
                currentEraserItem->setPen(eraser);
                currentEraserItem->setZValue(1);
            }
            else {
                QPainterPath path = currentEraserItem->path();
                path.moveTo(mapToScene(dragLastPoint.toPoint()));
                path.lineTo(mapToScene(event->pos()));
                currentEraserItem->setPath(path);
                dragLastPoint = event->pos();
            }

            // Remove excess path points
            if (currentEraserItem->path().elementCount() > 6) {
                auto oldPath = currentEraserItem->path();
                auto newPath = QPainterPath();
                newPath.moveTo(oldPath.elementAt(2));
                for (int i = 3; i < oldPath.elementCount(); i++) {
                    newPath.lineTo(oldPath.elementAt(i));
                }
                currentEraserItem->setPath(newPath);
            }

            // Detect if collides
            auto newItemList = QList<PenGraphicsItem*>();
            for (PenGraphicsItem* item : penItems) {
                if (currentEraserItem->scene() == item->scene() and currentEraserItem->collidesWithPath(item->path())) {
                    auto newItemListforUndo = QList<PenGraphicsItem*>();
                    // Collides with an item, entering collide position detecting & splitting
                    QPainterPath path = item->path();
                    // 0: initail, only first-one 1: containing 2: new item without collision
                    int nowInItemState = 0;
                    // Assigning variables, newItem's for the last judgement.
                    PenGraphicsItem* newItem = item;
                    QPainterPath newPath = QPainterPath();

                    for (int i = 0; i < path.elementCount(); i++) {
                        // Checking every element point
                        auto element = path.elementAt(i);
                        // Check if the current point's collided with the Eraser Path.
                        if (currentEraserItem->contains(QPointF(element.x, element.y))) {
                            // The point's in the eraser's path, or collided.
                            // Now there's three kinds of situation:
                            if (nowInItemState == 0) {
                                // First, if it's the first time for Eraser to collide with the path.
                                // Here we need to end the previous path, or the first path.
                                // If the Eraser collides with the path's beginning, then the newPath
                                // will contains nothing.
                                if (newPath.isEmpty()) {
                                    item->setPath(newPath);
                                    // And the newPath will be reset.
                                    newPath = QPainterPath();
                                }
                                else {
                                    item->setPath(newPath);
                                    newPath = QPainterPath();
                                }
                                // Switch the collision state to "colliding with element".
                                nowInItemState = 1;
                                // Get into next element's judgement.
                                continue;
                            }
                            else if (nowInItemState == 1) {
                                // Second, The previous point and the current point are both collided with
                                // the Eraser path.
                                // Do nothing and get into next cycle.
                                continue;
                            }
                            else if (nowInItemState == 2) {
                                // Third, if the previous point was in a new splitted path and this
                                // point is collided with the Eraser path again.
                                // End the previous item's path and start a new path.
                                newItem->setPath(newPath);
                                newPath = QPainterPath();
                                // Switch the collision state to "colliding".
                                nowInItemState = 1;
                                // Get into nect cycle.
                                continue;
                            }
                        }
                        else {
                            // The current element point is at outside of the Eraser path.
                            // Here also three kinds of situations.
                            if (nowInItemState == 0) {
                                // If the item's never collided with the Eraser path, continue the
                                // previous path's elements.
                                if (newPath.isEmpty()) {
                                    // If this is the first element in the path, then initailize
                                    // the newPath's position.
                                    // Adding 0.001 to x to avoid not rendering a single-point path.
                                    newPath.moveTo(element.x + 0.001, element.y);
                                }
                                // Copying the previous path's point.
                                newPath.lineTo(element.x, element.y);
                                continue;
                            }
                            else if (nowInItemState == 1) {
                                // The most complex one: The previous point's collided but the current
                                // point doesn't.
                                // We need to create a new Pen object.
                                newItem = new PenGraphicsItem(this);
                                // Copy the item's pen properties to it.
                                newItem->setPen(item->pen());
                                // Append it to the framework.
                                newItemList.append(newItem);
                                newItemListforUndo.append(newItem);
                                scene()->addItem(newItem);
                                // Give it a new Path with the current point.
                                newPath = QPainterPath();
                                newPath.moveTo(element.x + 0.001, element.y);
                                newPath.lineTo(element.x, element.y);
                                nowInItemState = 2;
                            }
                            else if (nowInItemState == 2) {
                                // If the previous point's the new and not collided one, just copy the
                                // current point's pos.
                                newPath.lineTo(element.x, element.y);
                            }
                        }
                    }
                    // After the cycle process:
                    if (nowInItemState == 0) {
                        // If the item's not collided at all (in fact it's a bug):
                        // Nothing need to do.
                    }
                    else if (nowInItemState == 1) {
                        // If the item ends with collision:
                        //newItem->setPath(newPath);
                    }
                    else if (nowInItemState == 2) {
                        // If the item's ending with new element(s):
                        newItem->setPath(newPath);
                    }
                    undoStack.append(UndoItem(path, newItemListforUndo, item));
                }
            }
            for (auto i : newItemList) {
                penItems.append(i);
            }
        }
    }
}

void FrameGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->source() != Qt::MouseEventNotSynthesized) {
        //event->ignore();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
    isMousePressing = false;
    if (event->button() == Qt::LeftButton) {
        dragLastPoint = QPoint();
        if (currentPenItem) {
            holdTimer->stop();
            currentPenItem = nullptr;
        }
        if (currentEraserItem) {
            currentEraserItem->scene()->removeItem(currentEraserItem);
            delete currentEraserItem;
            undoStack.append(UndoItem());
            currentEraserItem = nullptr;
        }
    }
}

void FrameGraphicsView::wheelEvent(QWheelEvent* event)
{
    QPoint prev_viewPos = QPoint(event->position().x(), event->position().y());
    QPointF prev_scenePos = this->mapToScene(prev_viewPos);
    if (event->angleDelta().y() > 0) {
        customScale(0.05);
    }
    else {
        customScale(-0.05);
    }
    
}

void FrameGraphicsView::paintEvent(QPaintEvent* event)
{
    QGraphicsView::paintEvent(event);
}

bool FrameGraphicsView::viewportEvent(QEvent* event)
{
    if (event->type() == QEvent::TouchBegin) {
        QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
        if (touchEvent->pointCount() >= 2) {
            return true;
        }
        if (touchEvent->pointCount() == 0) {
            return true;
        }
        isTouchPressing = true;
        auto touchPointPos = touchEvent->touchPoints().first().pos().toPoint();
        if (frameState == FrameSelect) {
            dragLastPoint = touchPointPos - touchPointPos + QPointF(width() / 2 / scaledRatio / scaledRatio, height() / 2 / scaledRatio / scaledRatio);
            dragLastPoint = dragLastPoint + mapToScene(0, 0);
            posAnchor = touchPointPos;
        }
        else if (frameState == FramePen) {
            dragLastPoint = touchPointPos;
            currentPenItem = new PenGraphicsItem(this);
            currentPenItem->setPen(pen);
            currentPenItem->setZValue(1);
            QPainterPath path = currentPenItem->path();
            path.moveTo(mapToScene(dragLastPoint.toPoint()));
            path.lineTo(mapToScene(dragLastPoint.toPoint()) + QPointF(0.001, 0));
            currentPenItem->setPath(path);
            scene()->addItem(currentPenItem);
            penItems.append(currentPenItem);
            undoStack.append(UndoItem(currentPenItem));
        }
        else if (frameState == FrameEraser) {
            dragLastPoint = touchPointPos;
            currentEraserItem = new EraserGraphicsItem(this);
            scene()->addItem(currentEraserItem);
            currentEraserItem->setPen(eraser);
            currentEraserItem->setZValue(1);
            QPainterPath path = currentEraserItem->path();
            path.moveTo(mapToScene(dragLastPoint.toPoint()));
            path.lineTo(mapToScene(touchPointPos) + QPointF(0.001, 0));
            currentEraserItem->setPath(path);
        }
        return true;
    }
    else if (event->type() == QEvent::TouchUpdate) {
        QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
        if (touchEvent->pointCount() >= 2) {
            return true;
        }
        if (touchEvent->pointCount() == 0) {
            return true;
        }
        isTouchPressing = true;
        auto touchPointPos = touchEvent->touchPoints().first().pos().toPoint();
        if (frameState == FrameSelect) {
            QPointF offsetPos = touchPointPos - posAnchor;
            offsetPos.setX(offsetPos.x() / scaledRatio / scaledRatio);
            offsetPos.setY(offsetPos.y() / scaledRatio / scaledRatio);
            if (isTouchPressing) {
                centerOn(dragLastPoint - offsetPos);
                scene()->update();
            }
        }
        else if (frameState == FramePen) {
            if (not currentPenItem) {
                dragLastPoint = touchPointPos;
                currentPenItem = new PenGraphicsItem(this);
                currentPenItem->setPen(pen);
                currentPenItem->setZValue(1);
                QPainterPath path = currentPenItem->path();
                path.moveTo(mapToScene(dragLastPoint.toPoint()));
                path.lineTo(mapToScene(dragLastPoint.toPoint()) + QPointF(0.001, 0));
                currentPenItem->setPath(path);
                scene()->addItem(currentPenItem);
                penItems.append(currentPenItem);
                undoStack.append(UndoItem(currentPenItem));
            }
            else {
                auto movedVal = touchPointPos - dragLastPoint;
                if (abs(movedVal.x()) + abs(movedVal.y()) >= 2) {
                    QPainterPath path = currentPenItem->path();
                    path.lineTo(mapToScene(touchPointPos));
                    currentPenItem->setPath(path);
                    dragLastPoint = touchPointPos;
                    holdTimer->start(700);
                }
            }
        }
        else if (frameState == FrameEraser) {
            // Create Eraser item or extend its path
            if (not currentEraserItem) {
                dragLastPoint = touchPointPos;
                currentEraserItem = new EraserGraphicsItem(this);
                currentEraserItem->setPen(eraser);
                currentEraserItem->setZValue(1);
            }
            else {
                QPainterPath path = currentEraserItem->path();
                path.moveTo(mapToScene(dragLastPoint.toPoint()));
                path.lineTo(mapToScene(touchPointPos));
                currentEraserItem->setPath(path);
                dragLastPoint = touchPointPos;
            }

            // Remove excess path points
            if (currentEraserItem->path().elementCount() > 10) {
                auto oldPath = currentEraserItem->path();
                auto newPath = QPainterPath();
                newPath.moveTo(oldPath.elementAt(2));
                for (int i = 3; i < oldPath.elementCount(); i++) {
                    newPath.lineTo(oldPath.elementAt(i));
                }
                currentEraserItem->setPath(newPath);
            }

            // Detect if collides
            auto newItemList = QList<PenGraphicsItem*>();
            for (PenGraphicsItem* item : penItems) {
                if (currentEraserItem->scene() == item->scene() and currentEraserItem->collidesWithPath(item->path())) {
                    auto newItemListforUndo = QList<PenGraphicsItem*>();
                    // Collides with an item, entering collide position detecting & splitting
                    QPainterPath path = item->path();
                    // 0: initail, only first-one 1: containing 2: new item without collision
                    int nowInItemState = 0;
                    // Assigning variables, newItem's for the last judgement.
                    PenGraphicsItem* newItem = item;
                    QPainterPath newPath = QPainterPath();

                    for (int i = 0; i < path.elementCount(); i++) {
                        // Checking every element point
                        auto element = path.elementAt(i);
                        // Check if the current point's collided with the Eraser Path.
                        if (currentEraserItem->contains(QPointF(element.x, element.y))) {
                            // The point's in the eraser's path, or collided.
                            // Now there's three kinds of situation:
                            if (nowInItemState == 0) {
                                // First, if it's the first time for Eraser to collide with the path.
                                // Here we need to end the previous path, or the first path.
                                // If the Eraser collides with the path's beginning, then the newPath
                                // will contains nothing.
                                if (newPath.isEmpty()) {
                                    item->setPath(newPath);
                                    // And the newPath will be reset.
                                    newPath = QPainterPath();
                                }
                                else {
                                    item->setPath(newPath);
                                    newPath = QPainterPath();
                                }
                                // Switch the collision state to "colliding with element".
                                nowInItemState = 1;
                                // Get into next element's judgement.
                                continue;
                            }
                            else if (nowInItemState == 1) {
                                // Second, The previous point and the current point are both collided with
                                // the Eraser path.
                                // Do nothing and get into next cycle.
                                continue;
                            }
                            else if (nowInItemState == 2) {
                                // Third, if the previous point was in a new splitted path and this
                                // point is collided with the Eraser path again.
                                // End the previous item's path and start a new path.
                                newItem->setPath(newPath);
                                newPath = QPainterPath();
                                // Switch the collision state to "colliding".
                                nowInItemState = 1;
                                // Get into nect cycle.
                                continue;
                            }
                        }
                        else {
                            // The current element point is at outside of the Eraser path.
                            // Here also three kinds of situations.
                            if (nowInItemState == 0) {
                                // If the item's never collided with the Eraser path, continue the
                                // previous path's elements.
                                if (newPath.isEmpty()) {
                                    // If this is the first element in the path, then initailize
                                    // the newPath's position.
                                    // Adding 0.001 to x to avoid not rendering a single-point path.
                                    newPath.moveTo(element.x + 0.001, element.y);
                                }
                                // Copying the previous path's point.
                                newPath.lineTo(element.x, element.y);
                                continue;
                            }
                            else if (nowInItemState == 1) {
                                // The most complex one: The previous point's collided but the current
                                // point doesn't.
                                // We need to create a new Pen object.
                                newItem = new PenGraphicsItem(this);
                                // Copy the item's pen properties to it.
                                newItem->setPen(item->pen());
                                // Append it to the framework.
                                newItemList.append(newItem);
                                newItemListforUndo.append(newItem);
                                scene()->addItem(newItem);
                                // Give it a new Path with the current point.
                                newPath = QPainterPath();
                                newPath.moveTo(element.x + 0.001, element.y);
                                newPath.lineTo(element.x, element.y);
                                nowInItemState = 2;
                            }
                            else if (nowInItemState == 2) {
                                // If the previous point's the new and not collided one, just copy the
                                // current point's pos.
                                newPath.lineTo(element.x, element.y);
                            }
                        }
                    }
                    // After the cycle process:
                    if (nowInItemState == 0) {
                        // If the item's not collided at all (in fact it's a bug):
                        // Nothing need to do.
                    }
                    else if (nowInItemState == 1) {
                        // If the item ends with collision:
                        //newItem->setPath(newPath);
                    }
                    else if (nowInItemState == 2) {
                        // If the item's ending with new element(s):
                        newItem->setPath(newPath);
                    }
                    undoStack.append(UndoItem(path, newItemListforUndo, item));
                }
            }
            for (auto i : newItemList) {
                penItems.append(i);
            }
        }
        return true;
    }
    else if (event->type() == QEvent::TouchEnd or event->type() == QEvent::TouchCancel) {
        isTouchPressing = false;
        dragLastPoint = QPoint();
        if (currentPenItem) {
            holdTimer->stop();
            currentPenItem = nullptr;
        }
        if (currentEraserItem) {
            currentEraserItem->scene()->removeItem(currentEraserItem);
            delete currentEraserItem;
            undoStack.append(UndoItem());
            currentEraserItem = nullptr;
        }
        return true;
    }

    return QAbstractScrollArea::viewportEvent(event);
}

UndoItem::UndoItem()
{
    type = UndoSplitter;
}

UndoItem::UndoItem(PenGraphicsItem* penitem)
{
    type = UndoPen;
    drewPenItem = penitem;
}

UndoItem::UndoItem(QPainterPath originalpath, QList<PenGraphicsItem*> newItems, PenGraphicsItem* originItem)
{
    type = UndoEraser;
    erasedPenPath = originalpath;
    drewPenItem = originItem;
    erasedPenQueue = newItems;
}
