#pragma once

#include <QtCore/QPropertyAnimation>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QEvent>
#include <QtCore/QList>
#include <QtCore/QScopedPointer>
#include <QtCore/QSharedPointer>
#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QStyleOption>
#include <QtWidgets/QStyle>
#include <QtWidgets/QWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QToolButton>

constexpr int ButtonWidth = 55;
constexpr int SeparatorWidth = 14;
constexpr int ToolBarHeight = 76;
constexpr int ToolBarMargin = 8;
constexpr int ToolBarBottomMargin = -4;
constexpr int ToolBarSink = 28;

class UnderToolBar  : public QWidget
{
	Q_OBJECT

public:
	UnderToolBar(QWidget* parent);
	~UnderToolBar();
	void addWidget(QToolButton* widget);
	void addSeparator();
	int getWidth();
	QSharedPointer<QPropertyAnimation> moveAnimation;

	void sinkBar(int y);
	void floatBar(int y);

	int sinkState = 0; // 0: up 1: sinking

private:
	QList<QWidget*> widgetList;
	void reGeometry();

protected:
	void paintEvent(QPaintEvent* event) override;
};
