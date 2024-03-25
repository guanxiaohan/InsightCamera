#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QSharedPointer>
#include <QPropertyAnimation>
#include <QPoint>
#include <QTimer>
#include <QEasingCurve>
#include <QMouseEvent>
#include <QCursor>
#include "ui_CapturedNotify.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CapturedNotifyClass; };
QT_END_NAMESPACE

constexpr int CapturedNotifyWidth = 260;

class CapturedNotify : public QWidget
{
	Q_OBJECT

public:
	CapturedNotify(QPixmap pixmap, QWidget *parent, int ind);
	~CapturedNotify();
	QSharedPointer<QPropertyAnimation> moveAnimation;
	void appear();
	void disappear();
	void noFocused();
	void tryDisappear();

private:
	void recall();
	void disappearFinished();
	void appearFinished();
	bool appearing = true;
	Ui::CapturedNotifyClass *ui;
	QTimer* disappearTimer;
	int endXPos;
	int returnIndex;

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* event) override;

signals:
	void switchToPixmap(int);
	void disappeared();
};
