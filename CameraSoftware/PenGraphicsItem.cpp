#include "PenGraphicsItem.h"

PenGraphicsItem::PenGraphicsItem(QObject *parent)
	: QGraphicsPathItem()
{
	//pParent = parent;
}

PenGraphicsItem::~PenGraphicsItem()
{}

//void PenGraphicsItem::addRegion(QRegion region)
//{
//	paintRegion += region;
//}
//
//void PenGraphicsItem::updateRegion()
//{
//	
//}

//void PenGraphicsItem::paint(QPainter * p, const QStyleOptionGraphicsItem * opt, QWidget * widget)
//{
//	QGraphicsPathItem::paint(p, opt, widget);
//}

int PenGraphicsItem::type() const
{
	return 1145;
}
