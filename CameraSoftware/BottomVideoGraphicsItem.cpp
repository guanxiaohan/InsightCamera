#include "BottomVideoGraphicsItem.h"

BottomVideoGraphicsItem::BottomVideoGraphicsItem(QObject *parent)
	: QGraphicsVideoItem()
{}

BottomVideoGraphicsItem::~BottomVideoGraphicsItem()
{}

void BottomVideoGraphicsItem::paint(QPainter * p, const QStyleOptionGraphicsItem * opt, QWidget * widget)
{
	//p->setCompositionMode(QPainter::CompositionMode_DestinationOver);
	QGraphicsVideoItem::paint(p, opt, widget);
}
