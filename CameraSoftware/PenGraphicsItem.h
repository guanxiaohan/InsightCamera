#pragma once

#include <QGraphicsPathItem>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QRegion>

class PenGraphicsItem  : public QObject, public QGraphicsPathItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	PenGraphicsItem(QObject *parent);
	~PenGraphicsItem();
	//void addRegion(QRegion region);
	//void updateRegion();

private:
	//QObject* pParent;
	//QRegion paintRegion;

protected:
	//void paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* widget) override;
	virtual int type() const override;
};
