#pragma once

#include <QWidget>
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

private:
	Ui::EraserOptionsWidgetClass *ui;
};
