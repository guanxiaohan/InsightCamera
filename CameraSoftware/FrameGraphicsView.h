#pragma once

#include <QGraphicsView>
#include <QMouseEvent>
#include <QRectF>
#include <QPointF>

class FrameGraphicsView  : public QGraphicsView
{
	Q_OBJECT

public:
	FrameGraphicsView(QWidget *parent);
	~FrameGraphicsView();

protected:
	void mousePressEvent(QMouseEvent *event);
	void updateSceneRect();
};
