#pragma once

#include <QMenu>
#include <QtCore/QPropertyAnimation>
#include <QEasingCurve>
#include <QList>
#include <QAction>
#include <QHideEvent>

class AnimationMenu  : public QMenu
{
	Q_OBJECT
	const int WIDTH = 180;

public:
	AnimationMenu(QWidget *parent);
	~AnimationMenu();
	void addMenuItem(QString actionText, QString iconLocation);
	void exec(QPoint position);
	int getHeight() const;

private:
	QPropertyAnimation* animation;
	QList<QAction* > ActionList;
	void recreateActions();
	void actionReturned();

protected:
	void hideEvent(QHideEvent* event);

signals:
	void returnAction(int);
};
