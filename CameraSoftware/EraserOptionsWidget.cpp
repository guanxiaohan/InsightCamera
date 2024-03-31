#include "EraserOptionsWidget.h"

EraserOptionsWidget::EraserOptionsWidget(QWidget* parent = nullptr, int eraserwidth = 60)
	: QWidget(parent)
	, ui(new Ui::EraserOptionsWidgetClass())
{
	ui->setupUi(this);
	pParent = parent;
	setObjectName("EraserOptionsWidget");
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint | Qt::Popup);
	setAttribute(Qt::WA_TranslucentBackground);
	setStyleSheet(
		"QWidget#EraserOptionsWidget{"
		"background-color: rgba(255, 255, 255, 0.68);"
		"border: 2px solid rgba(255, 255, 255, 0.9);"
		"border-radius: 14px;"
		"}"
		"QSlider::groove:horizontal{"
		"border: 0px;"
		"}"
		"QSlider::sub-page:horizontal{"
		"background-color: #aaaaaa;"
		"border-radius: 3px;"
		"margin-top: 4px;"
		"margin-bottom: 4px;"
		"}"
		"QSlider::add-page:horizontal{"
		"background-color: #eeeeee;"
		"border: 0px solid #000;"
		"border-radius: 3px;"
		"margin-top: 4px;"
		"margin-bottom: 4px;"
		"}"
		"QSlider::handle:horizontal{"
		"background: #dddddd;"
		"width: 18px;"
		"border: 2px solid #888888;"
		"border-radius: 11px;"
		"}"
		"QSlider::handle:horizontal:hover{"
		"background: #ffffff;"
		"}"
		"QLabel{"
		"font-size: 17px;"
		"color: #333333;"
		"}"
	);

	ui->pushButton->setIconid(tr("Clear"));
	ui->pushButton_2->setIconid(tr("ClearAll"));
	ui->pushButton->setParent(this);
	ui->pushButton_2->setParent(this);
	ui->pushButton->installEventFilter(this);
	ui->pushButton_2->installEventFilter(this);
	ui->sizeSlider->setValue(eraserwidth);
	ui->sizeLabel->setText(QString::number(eraserwidth));
	connect(ui->pushButton, &ToolBarButton::buttonClicked, this, &EraserOptionsWidget::clearClicked);
	connect(ui->pushButton_2, &ToolBarButton::buttonClicked, this, &EraserOptionsWidget::clearAllClicked);
	connect(ui->sizeSlider, &QSlider::valueChanged, this, &EraserOptionsWidget::sliderValueChanged);
	animation = QSharedPointer<QPropertyAnimation>(new QPropertyAnimation(this, "geometry"));
	animation->setDuration(500);
	animation->setEasingCurve(QEasingCurve::OutQuint);
	setMouseTracking(true);
}

EraserOptionsWidget::~EraserOptionsWidget()
{
	delete ui;
}

void EraserOptionsWidget::appear()
{
	auto rectPos = geometry();
	rectPos.setWidth(10);
	animation->setStartValue(rectPos);
	animation->setEndValue(geometry());
	animation->start();
}

void EraserOptionsWidget::disappear()
{
	if (animation->state() == QPropertyAnimation::Running) {
		animation->stop();
	}
	disappearState = 1;
	animation->setStartValue(geometry());
	auto endVal = animation->endValue();
	auto endRect = endVal.toRect();
	endRect.setWidth(230);
	endRect.setX(endRect.x() + 230);
	animation->setEndValue(endRect);
	connect(animation.data(), &QPropertyAnimation::finished, this, &EraserOptionsWidget::finishDisappear);
	animation->setDuration(380);
	animation->start();
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

void EraserOptionsWidget::finishDisappear()
{
	disappearState = 2;
	hide();
}

void EraserOptionsWidget::sliderValueChanged(int val)
{
	ui->sizeLabel->setText(QString::number(val));
	emit setEraserWidthSignal(val);
}

void EraserOptionsWidget::mousePressEvent(QMouseEvent* event)
{
	if (geometry().contains(event->globalPos())) {
		event->accept();
	}
	else {
		close();
		QWidget::mousePressEvent(event);
	}
}

void EraserOptionsWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (disappearState == 0) {
		event->accept();
	}
	else {
		QWidget::mouseMoveEvent(event);
	}
}

void EraserOptionsWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (disappearState == 0) {
		event->accept();
	}
	else {
		QWidget::mouseReleaseEvent(event);
	}
}

void EraserOptionsWidget::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void EraserOptionsWidget::closeEvent(QCloseEvent* event)
{
	if (disappearState == 0) {
		disappear();
		event->ignore();
	}
	else if (disappearState == 1) {
		animation->stop();
		hide();
		event->accept();
	}
	else {
		hide();
		event->accept();
	}
}

bool EraserOptionsWidget::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::HoverEnter) {
		if (obj == ui->pushButton) {
			ui->pushButton->hoverEnter(nullptr);
		}
		else if (obj == ui->pushButton_2) {
			ui->pushButton_2->hoverEnter(nullptr);
		}
	}
	else if (event->type() == QEvent::HoverLeave) {
		if (obj == ui->pushButton) {
			ui->pushButton->hoverLeave(nullptr);
		}
		else if (obj == ui->pushButton_2) {
			ui->pushButton_2->hoverLeave(nullptr);
		}
	}
	return QWidget::eventFilter(obj, event);
}
