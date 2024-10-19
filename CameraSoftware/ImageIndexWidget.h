#pragma once

#include <QWidget>
#include "ui_ImageIndexWidget.h"

#include <QtCore/QPropertyAnimation>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QSharedPointer>
#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QStyleOption>
#include <QtWidgets/QStyle>
#include <QtWidgets/QWidget>
#include <QtWidgets/QToolButton>
#include <QtCore/QPoint>

constexpr int ImageLabelWidth = 220;
constexpr int ImageLabelHeight = 28;
constexpr int ImageLabelBottomMargin = 3;

class ImageIndexWidget : public QWidget
{
	Q_OBJECT

public:
	ImageIndexWidget(QWidget *parent = nullptr);
	~ImageIndexWidget();

	QSharedPointer<QPropertyAnimation> moveAnimation;
	void setLabel(int index);
	void initGeometry(QPoint position);
	void switchShowing(bool shown);

	void sinkBar(int y);
	void floatBar();

private:
	Ui::ImageIndexWidgetClass *ui;
	bool showing = false;
	int xPos;
	int yPos;
	int sinkState = 0;

protected:
	void paintEvent(QPaintEvent* event) override;
};
