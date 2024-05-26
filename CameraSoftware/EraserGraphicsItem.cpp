#include "EraserGraphicsItem.h"

EraserGraphicsItem::EraserGraphicsItem(QObject *parent)
	: QGraphicsPathItem()
{
	
}

EraserGraphicsItem::~EraserGraphicsItem()
{
	
}

void EraserGraphicsItem::paint(QPainter * p, const QStyleOptionGraphicsItem * opt, QWidget * widget)
{
	QGraphicsPathItem::paint(p, opt, widget);
}
