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
#include <cmath>

const int MAXIMUM_X_OFFSET = 0;
const int MAXIMUM_Y_OFFSET = 0;

class FrameGraphicsView  : public QGraphicsView
{
    Q_OBJECT

public:
    FrameGraphicsView(QWidget *parent);
    ~FrameGraphicsView();
    void resetViewPort(int width, int height);
    void setFrameDraggable(bool val);

private:
    double scaledRatio = 1.0;
    QPointF dragLastPoint;
    bool isMousePressing = false;
    QPoint posAnchor;
    bool nowDraggable;

protected:
    void customScale(double ratio);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);
    
};
