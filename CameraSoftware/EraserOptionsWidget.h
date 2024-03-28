#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QStyle>
#include <QPropertyAnimation>
#include "ui_EraserOptionsWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EraserOptionsWidgetClass; };
QT_END_NAMESPACE

class EraserOptionsWidget : public QWidget
{
	Q_OBJECT

public:
	EraserOptionsWidget(QWidget *parent = nullptr);
	~EraserOptionsWidget();
	QPropertyAnimation* animation;

private:
	Ui::EraserOptionsWidgetClass *ui;
	void clearClicked();
	void clearAllClicked();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

signals:
	void clearSignal();
	void clearAllSignal();
};
