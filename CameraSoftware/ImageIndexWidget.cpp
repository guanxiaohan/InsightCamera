#include "ImageIndexWidget.h"


ImageIndexWidget::ImageIndexWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::ImageIndexWidgetClass())
{
	ui->setupUi(this);

	setLayout(ui->horizontalLayout);

	moveAnimation = QSharedPointer<QPropertyAnimation>(new QPropertyAnimation(this, "geometry"));
	moveAnimation->setEasingCurve(QEasingCurve::OutQuart);
	moveAnimation->setDuration(400);
	setObjectName("ImageIndexLabel");
	setStyleSheet("QWidget#ImageIndexLabel {"
		"background-color: rgba(255, 255, 255, 170);"
		"border: 1px solid rgba(255, 255, 255, 200);"
		"border-radius: 9px;"
		"}"
		"QLabel {"
		"font-size: 15px;"
		"}"
	);
	raise();
}

ImageIndexWidget::~ImageIndexWidget()
{
	delete ui;
}

void ImageIndexWidget::setLabel(int index)
{
	ui->indexLabel->setText(tr("Now Display: %1").arg(index == -1? tr("Camera Frame") : tr("Capture #%1").arg(index + 1)));
}

void ImageIndexWidget::initGeometry(QPoint position)
{
	setGeometry(QRect(position.x(), position.y(), 0, ImageLabelHeight));
	setLabel(-1);
	xPos = position.x();
	yPos = position.y();
}

void ImageIndexWidget::switchShowing(bool shown)
{
	moveAnimation->stop();
	moveAnimation->setStartValue(geometry());
	if (shown) {
		moveAnimation->setEndValue(QRect(x() - ImageLabelWidth / 2, y(), ImageLabelWidth, ImageLabelHeight));
	}
	else {
		moveAnimation->setEndValue(QRect(xPos, y(), 0, ImageLabelHeight));
	}
	moveAnimation->start();
}

void ImageIndexWidget::sinkBar(int y)
{
	if (sinkState == 1)
		return;
	sinkState = 1;
	if (moveAnimation->state() == QPropertyAnimation::Running) {
		moveAnimation->stop();
	}
	QRect start = moveAnimation->endValue().toRect();
	moveAnimation->setStartValue(start);
	start.setY(yPos + y);
	start.setHeight(ImageLabelHeight);
	moveAnimation->setEndValue(start);
	moveAnimation->start();
}

void ImageIndexWidget::floatBar()
{
	if (sinkState == 0)
		return;
	sinkState = 0;
	if (moveAnimation->state() == QPropertyAnimation::Running) {
		moveAnimation->stop();
	}
	QRect start = moveAnimation->endValue().toRect();
	moveAnimation->setStartValue(start);
	start.setY(yPos);
	start.setHeight(ImageLabelHeight);
	moveAnimation->setEndValue(start);
	moveAnimation->start();
}

void ImageIndexWidget::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
