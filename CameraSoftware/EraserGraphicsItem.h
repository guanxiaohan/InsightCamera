#pragma once

#include <QGraphicsPathItem>
#include <QPainter>

class EraserGraphicsItem  : public QObject, public QGraphicsPathItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	EraserGraphicsItem(QObject *parent);
	~EraserGraphicsItem();

protected:
	void paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* widget) override;

};
