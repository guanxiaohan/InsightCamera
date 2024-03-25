#pragma once

#include <QGraphicsVideoItem>
#include <QPainter>

class BottomVideoGraphicsItem : public QGraphicsVideoItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	BottomVideoGraphicsItem(QObject *parent);
	~BottomVideoGraphicsItem();

protected:
	void paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* widget) override;

};
