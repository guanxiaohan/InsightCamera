#include "UnderToolBar.h"

UnderToolBar::UnderToolBar(QWidget *parent)
	: QWidget(parent)
{
	widgetList = QList<QWidget*>();
	moveAnimation = QSharedPointer<QPropertyAnimation>(new QPropertyAnimation(this, "geometry"));
	setObjectName("UnderToolBar");
	setStyleSheet("QWidget#UnderToolBar {"
		"background-color: rgba(255, 255, 255, 170);"
		"border: 2px solid rgba(255, 255, 255, 200);"
		"border-radius: 8px;"
		"}"
		"QFrame{"
		"color: #666666"
		"}");
	raise();
}

UnderToolBar::~UnderToolBar()
{
	for (auto i : widgetList) {
		delete i;
	}
}

void UnderToolBar::addWidget(QToolButton* widget)
{
	widget->setParent(this);
	widget->resize(ButtonWidth, ToolBarHeight);
	widget->move(ToolBarMargin+getWidth(), 0);
	widget->setIconSize(QSize(60, 32));
	widgetList.append(widget);
	widget->show();
}

void UnderToolBar::addSeparator()
{
	auto line = new QFrame(this);
	line->setFrameShape(QFrame::VLine);
	line->resize(SeparatorWidth, ToolBarHeight - 16);
	line->move(ToolBarMargin+getWidth(), 8);
	widgetList.append(line);
	line->show();
}

void UnderToolBar::reGeometry()
{
}

void UnderToolBar::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

int UnderToolBar::getWidth()
{
	int totalWidth = 0;
	for (auto i : widgetList) {
		totalWidth = totalWidth + i->width();
	}
	return totalWidth;
}

void UnderToolBar::sinkBar(int y)
{
	if (sinkState == 1)
		return;
	sinkState = 1;
	if (moveAnimation->state() == QPropertyAnimation::Running) {
		moveAnimation->stop();
	}
	QRect start = moveAnimation->endValue().toRect();
	moveAnimation->setStartValue(start);
	start.setY(y);
	moveAnimation->setEndValue(start);
	moveAnimation->start();
}

void UnderToolBar::floatBar(int y)
{
	if (sinkState == 0)
		return;
	sinkState = 0;
	if (moveAnimation->state() == QPropertyAnimation::Running) {
		moveAnimation->stop();
	}
	QRect start = moveAnimation->endValue().toRect();
	moveAnimation->setStartValue(start);
	start.setY(y);
	moveAnimation->setEndValue(start);
	moveAnimation->start();
}
