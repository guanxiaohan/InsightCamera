#pragma once

#include <QGraphicsPixmapItem>

class BottomPixmapGraphicsItem : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	BottomPixmapGraphicsItem(QObject *parent);
	~BottomPixmapGraphicsItem();
};
