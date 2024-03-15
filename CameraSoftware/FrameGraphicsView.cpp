#include "FrameGraphicsView.h"

FrameGraphicsView::FrameGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
}

FrameGraphicsView::~FrameGraphicsView()
{

}

void FrameGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        /*QMenu* mouseLeftMenu = new QMenu(this);
        QAction* rotateLeft = new QAction(tr("rotateLeft"), this);
        QAction* rotateRight = new QAction(tr("rotateRight"), this);
        QAction* zoomIn = new QAction(tr("zoomIn"), this);
        QAction* zoomOut = new QAction(tr("zoomOut"), this);

        mouseLeftMenu->addAction(rotateLeft);
        mouseLeftMenu->addAction(rotateRight);
        mouseLeftMenu->addAction(zoomIn);
        mouseLeftMenu->addAction(zoomOut);

        mouseLeftMenu->move(cursor().pos());
        mouseLeftMenu->show();

        connect(rotateLeft, SIGNAL(triggered()), this, SLOT(slot_rotateLeft()));
        connect(rotateRight, SIGNAL(triggered()), this, SLOT(slot_rotateRight()));
        connect(zoomIn, SIGNAL(triggered()), this, SLOT(slot_zoomIn()));
        connect(zoomOut, SIGNAL(triggered()), this, SLOT(slot_zoomOut()));*/
    }
    else
    {
        QGraphicsView::mousePressEvent(event);
    }
}

void FrameGraphicsView::updateSceneRect()
{
    QRectF rectTmp = scene()->sceneRect();
    QPointF ptTopLeft = rectTmp.topLeft();
    QPointF ptBottomRight = rectTmp.bottomRight();
    QPointF ptW_H = 0.5 * QPointF(rect().width(), rect().height());
    ptTopLeft -= ptW_H;
    ptBottomRight += ptW_H;
    rectTmp.setTopLeft(ptTopLeft);
    rectTmp.setBottomRight(ptBottomRight);
    scene()->setSceneRect(rectTmp);
}


