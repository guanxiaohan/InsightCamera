#include "EraserOptionsWidget.h"

EraserOptionsWidget::EraserOptionsWidget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::EraserOptionsWidgetClass())
{
	ui->setupUi(this);
	setObjectName("EraserOptionsWidget");
	this->setWindowFlags(Qt::Popup);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
	setStyleSheet(
		"QWidget#EraserOptionsWidget{"
		"background-color: rgba(255, 255, 255, 0.68);"
		"border: 2px solid rgba(255, 255, 255, 0.9);"
		"border-radius: 14px;"
		"}"
	);
	connect(ui->pushButton, &QPushButton::clicked, this, &EraserOptionsWidget::clearClicked);
	connect(ui->pushButton_2, &QPushButton::clicked, this, &EraserOptionsWidget::clearAllClicked);
}

EraserOptionsWidget::~EraserOptionsWidget()
{
	delete ui;
}

void EraserOptionsWidget::clearClicked()
{
	emit clearSignal();
	close();
}

void EraserOptionsWidget::clearAllClicked()
{
	emit clearAllSignal();
	close();
}

void EraserOptionsWidget::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
}

void EraserOptionsWidget::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}