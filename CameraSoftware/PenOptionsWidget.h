#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QStyle>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include "ui_PenOptionsWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PenOptionsWidgetClass; };
QT_END_NAMESPACE

constexpr int PenPanelWidth = 242;
constexpr int PenPanelHeight = 187;
const QList<QColor> PenColors = QList<QColor>(
	{QColor(255, 255, 255), 
	QColor(0, 0, 0),
	QColor(255, 0, 0),
	QColor(255, 255, 0),
	QColor(0, 230, 80),
	QColor(0, 80, 240)});

QString generateButtonsQSS();

class PenOptionsWidget : public QWidget
{
	Q_OBJECT

public:
	PenOptionsWidget(QWidget *parent, int penwidth, QColor color);
	~PenOptionsWidget();
	void appear();
	void disappear();
	QSharedPointer<QPropertyAnimation> animation;

private:
	Ui::PenOptionsWidgetClass* ui;
	QWidget* pParent;
	QWidget* mouseReceiver = nullptr;
	QList<QRadioButton*> buttons;
	void finishDisappear();
	void colorSelected();
	void sliderValueChanged(int val);
	void transMouseEvents(QMouseEvent* event);
	int disappearState = 0;

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void closeEvent(QCloseEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* event) override;

signals:
	void setColorSignal(QColor);
	void setPenWidthSignal(int);
};
