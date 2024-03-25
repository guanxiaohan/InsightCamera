#include "EraserOptionsWidget.h"

EraserOptionsWidget::EraserOptionsWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::EraserOptionsWidgetClass())
{
	ui->setupUi(this);
}

EraserOptionsWidget::~EraserOptionsWidget()
{
	delete ui;
}
