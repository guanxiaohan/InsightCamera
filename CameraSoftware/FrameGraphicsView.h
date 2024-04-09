#pragma once

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QScrollBar>
#include <QObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QRectF>
#include <QPointF>
#include <QTransform>
#include <QPen>
#include <QList>
#include <QGraphicsVideoItem>
#include <QRegion>
#include <QTimer>
#include <cmath>

#include "PenGraphicsItem.h"
#include "EraserGraphicsItem.h"

const int MAXIMUM_X_OFFSET = 0;
const int MAXIMUM_Y_OFFSET = 0;

class UndoItem 
{
public:
    UndoItem();
    UndoItem(PenGraphicsItem* penitem);
    UndoItem(QPainterPath originalpath, QList<PenGraphicsItem*> newItems, PenGraphicsItem* originItem);
    enum UndoType {UndoEraser, UndoPen, UndoSplitter};
    UndoType type;
    PenGraphicsItem* drewPenItem = nullptr;
    QPainterPath erasedPenPath;
    QList<PenGraphicsItem*> erasedPenQueue;
};

class FrameGraphicsView  : public QGraphicsView
{
    Q_OBJECT

public:
    enum nowState{FrameSelect, FramePen, FrameEraser};
    FrameGraphicsView(QWidget *parent);
    ~FrameGraphicsView();
    void resetViewPort(int width, int height);
    void setFrameDraggable(nowState val);
    void setEraserWidth(int width);
    void setPenWidth(int width);
    void setPenColor(QColor color);
    void clearFrame();
    void clearAllFrame();
    void undoProcess();
    QPen pen;
    QPen eraser;
    QGraphicsItem* bottomItem = nullptr;

private:
    void recognizePen();
    double scaledRatio = 1.0;
    QPointF dragLastPoint;
    bool isMousePressing = false;
    bool isTouchPressing = false;
    QPoint posAnchor;
    QTimer* holdTimer;
    nowState frameState = FrameSelect;
    PenGraphicsItem* currentPenItem = nullptr;
    EraserGraphicsItem* currentEraserItem = nullptr;
    QList<PenGraphicsItem*> penItems;
    QList<EraserGraphicsItem*> eraserItems;
    QList<UndoItem> undoStack;

protected:
    void customScale(double ratio);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent* event) override;
    bool event(QEvent* event) override;

};
