#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QStyle>
#include <QPropertyAnimation>
#include <QEasingCurve>

#include "ui_EraserOptionsWidget.h"
#include "ToolBarButton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EraserOptionsWidgetClass; };
QT_END_NAMESPACE

class EraserOptionsWidget : public QWidget
{
	Q_OBJECT

public:
	EraserOptionsWidget(QWidget* parent, int eraserwidth);
	~EraserOptionsWidget();
	void appear();
	void disappear();
	QSharedPointer<QPropertyAnimation> animation;

private:
	Ui::EraserOptionsWidgetClass* ui;
	QWidget* pParent;
	QWidget* mouseReceiver = nullptr;
	void clearClicked();
	void clearAllClicked();
	void finishDisappear();
	void sliderValueChanged(int val);
	int disappearState = 0;

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void closeEvent(QCloseEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* event) override;

signals:
	void clearSignal();
	void clearAllSignal();
	void setEraserWidthSignal(int);
};
