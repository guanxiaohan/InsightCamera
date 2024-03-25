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
#include <cmath>

#include "PenGraphicsItem.h"
#include "EraserGraphicsItem.h"

const int MAXIMUM_X_OFFSET = 0;
const int MAXIMUM_Y_OFFSET = 0;

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
    QPen pen;
    QPen eraser;
    QGraphicsItem* bottomItem = nullptr;

private:
    double scaledRatio = 1.0;
    QPointF dragLastPoint;
    bool isMousePressing = false;
    QPoint posAnchor;
    nowState frameState = FrameSelect;
    PenGraphicsItem* currentPenItem = nullptr;
    EraserGraphicsItem* currentEraserItem = nullptr;
    QList<PenGraphicsItem*> penItems;
    QList<EraserGraphicsItem*> eraserItems;

protected:
    void customScale(double ratio);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent* event) override;

};
