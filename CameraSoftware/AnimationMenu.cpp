#include "AnimationMenu.h"

AnimationMenu::AnimationMenu(QWidget* parent)
	: QMenu(parent)
{
	animation = new QPropertyAnimation(this, "geometry");
	animation->setEasingCurve(QEasingCurve::OutQuart);
	animation->setDuration(350);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setObjectName("AnimationMenu");
	setStyleSheet("QMenu{"
		"padding: 2px 2px;"
		"background: rgba(255,255,255,0.68);"
		"border-radius: 8px;"
		"color: #222222;"
		"}"
		"QMenu::item{"
		"color: #222222;"
		"font-size: 17px;"
		"height: 48px;"
		"background-color: transparent;"
		"width: 166px;"
		"border-radius: 8px;"
		"padding: 0px 5px;"
		"}"
		"QMenu::item:selected,QMenu::item:hover,QMenu::item:pressed{"
		"background-color: rgba(255,255,255,0.8);"
		"}");
}

AnimationMenu::~AnimationMenu()
{
	for (auto action : ActionList) {
		delete action;
	}
	delete animation;
}

void AnimationMenu::addMenuItem(QString actionText, QString iconLocation)
{
	auto action = new QAction(actionText);
	action->setProperty("index", ActionList.count());
	connect(action, &QAction::triggered, this, &AnimationMenu::actionReturned);
	ActionList.append(action);
}

void AnimationMenu::recreateActions()
{
	for (auto action : ActionList) {
		addAction(action);
	}
}

void AnimationMenu::actionReturned()
{
	int index = sender()->property("index").toInt();
	emit returnAction(index);
}

void AnimationMenu::hideEvent(QHideEvent* event)
{
	emit returnAction(-1);
	event->accept();
}

void AnimationMenu::exec(QPoint pos)
{
	clear();
	recreateActions();
	int height = getHeight();
	animation->setStartValue(QRect(pos.x(), pos.y(), 1, height));
	animation->setEndValue(QRect(pos.x(), pos.y(), WIDTH, height));
	animation->start();
	show();
}

int AnimationMenu::getHeight() const
{
	return ActionList.count() * 48 + 4;
}
