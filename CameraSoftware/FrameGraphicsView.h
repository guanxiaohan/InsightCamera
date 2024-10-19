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
#include <QMessageBox>
#include <cmath>

#include "PenGraphicsItem.h"
#include "EraserGraphicsItem.h"
#include "AnimationMenu.h"
#include "Configuration.h"

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
    FrameGraphicsView(Configuration* configObj, QWidget *parent);
    ~FrameGraphicsView();
    void resetViewPort(int width, int height);
    void setFrameDraggable(nowState val);
    void setEraserWidth(int width);
    void setPenWidth(int width);
    void setPenColor(QColor color);
    void clearFrame();
    void clearAllFrame();
    void undoProcess();
    void setScaleFromSlider(int value);
    void menuReturnAction(int);
    QPen pen;
    QPen eraser;
    QGraphicsItem* bottomItem = nullptr;
    bool isMousePressing = false;
    bool isTouchPressing = false;

private:
    Configuration* configObject;
    QPoint posAnchor;
    void recognizePen();
    void functionMenu();
    double scaledRatio = 1.0;
    QPointF dragLastPoint;
    QPointF selectLastPoint;
    QTimer* holdTimer;
    QTimer* menuTimer;
    nowState frameState = FrameSelect;
    PenGraphicsItem* currentPenItem = nullptr;
    EraserGraphicsItem* currentEraserItem = nullptr;
    PenGraphicsItem* operatePenItem = nullptr;
    QList<PenGraphicsItem*> penItems;
    QList<EraserGraphicsItem*> eraserItems;
    QList<UndoItem> undoStack;
    QSharedPointer<AnimationMenu> PopMenu;

protected:
    void customScale(double ratio);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent* event) override;
    bool event(QEvent* event) override;

signals:
    void addSlider(int, int, QPoint);
    void penDown();
};
