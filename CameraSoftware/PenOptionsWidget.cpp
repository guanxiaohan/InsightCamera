#include "PenOptionsWidget.h"

QString generateButtonsQSS() {
	QString result = "";
	const QString mode = QString(
		"#colorButton%1::indicator{"
		"background-color: rgb(%2, %3, %4);"
		"}"
	);
	const QList<QString> buttonIds = { "00", "01", "02", "10", "11", "12" };
	for (int i = 0; i < buttonIds.count(); i++) {
		result += mode.arg(buttonIds[i], QString::number(PenColors[i].red()),
			QString::number(PenColors[i].green()), QString::number(PenColors[i].blue()));
	}
	return result;
}

PenOptionsWidget::PenOptionsWidget(QWidget *parent, int penwidth, QColor color)
	: QWidget(parent)
	, ui(new Ui::PenOptionsWidgetClass())
{
	ui->setupUi(this);
	pParent = parent;
	setObjectName("PenOptionsWidget");
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint | Qt::Popup);
	setAttribute(Qt::WA_TranslucentBackground);
	setStyleSheet(
		"QWidget#PenOptionsWidget{"
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
		"QRadioButton:checked{"
		"border: 3px solid rgba(10, 230, 60, 200);"
		"border-radius: 13px;"
		"background-color: rgba(0, 0, 0, 0);"
		"}"
		"QRadioButton:unchecked{"
		"margin-left: 3px;"
		"border: 0px solid rgb(0, 0, 0);"
		"border-radius: 9px;"
		"background-color: rgba(0, 0, 0, 0);"
		"}"
		"QRadioButton::indicator{"
		"margin-left: 3px;"
		"width: 46px;"
		"height: 46px;"
		"border-radius: 9px;"
		"border: 0px solid rgba(255, 255, 255, 0);"
		"}" + generateButtonsQSS()
	);
	buttons.append(ui->colorButton00);
	buttons.append(ui->colorButton01);
	buttons.append(ui->colorButton02);
	buttons.append(ui->colorButton10);
	buttons.append(ui->colorButton11);
	buttons.append(ui->colorButton12);
	ui->colorButton00->setProperty("index", 0);
	ui->colorButton01->setProperty("index", 1);
	ui->colorButton02->setProperty("index", 2);
	ui->colorButton10->setProperty("index", 3);
	ui->colorButton11->setProperty("index", 4);
	ui->colorButton12->setProperty("index", 5);
	connect(ui->sizeSlider, &QSlider::valueChanged, this, &PenOptionsWidget::sliderValueChanged);
	connect(ui->colorButton00, &QRadioButton::clicked, this, &PenOptionsWidget::colorSelected);
	connect(ui->colorButton01, &QRadioButton::clicked, this, &PenOptionsWidget::colorSelected);
	connect(ui->colorButton02, &QRadioButton::clicked, this, &PenOptionsWidget::colorSelected);
	connect(ui->colorButton10, &QRadioButton::clicked, this, &PenOptionsWidget::colorSelected);
	connect(ui->colorButton11, &QRadioButton::clicked, this, &PenOptionsWidget::colorSelected);
	connect(ui->colorButton12, &QRadioButton::clicked, this, &PenOptionsWidget::colorSelected);

	buttons[PenColors.indexOf(color)]->setChecked(true);
	ui->sizeLabel->setText(QString::number(penwidth));
	ui->sizeSlider->setValue(penwidth);

	animation = QSharedPointer<QPropertyAnimation>(new QPropertyAnimation(this, "geometry"));
	animation->setDuration(500);
	animation->setEasingCurve(QEasingCurve::OutQuint);
}

PenOptionsWidget::~PenOptionsWidget()
{
	delete ui;
}

void PenOptionsWidget::appear()
{
	auto rectPos = geometry();
	rectPos.setWidth(10);
	animation->setStartValue(rectPos);
	animation->setEndValue(geometry());
	animation->start();
}

void PenOptionsWidget::disappear()
{
	setAttribute(Qt::WA_TransparentForMouseEvents, true);
	if (animation->state() == QPropertyAnimation::Running) {
		animation->stop();
	}
	disappearState = 1;
	animation->setStartValue(geometry());
	auto endVal = animation->endValue();
	auto endRect = endVal.toRect();
	endRect.setWidth(PenPanelWidth);
	endRect.setX(endRect.x() + PenPanelWidth);
	animation->setEndValue(endRect);
	connect(animation.data(), &QPropertyAnimation::finished, this, &PenOptionsWidget::finishDisappear);
	animation->setDuration(380);
	animation->start();
}

void PenOptionsWidget::finishDisappear()
{
	disappearState = 2;
	hide();
}

void PenOptionsWidget::colorSelected()
{
	auto data = sender()->property("index").toInt();
	emit setColorSignal(PenColors[data]);
}

void PenOptionsWidget::sliderValueChanged(int val)
{
	ui->sizeLabel->setText(QString::number(val));
	emit setPenWidthSignal(val);
}

void PenOptionsWidget::transMouseEvents(QMouseEvent* event)
{
	if (this->pParent) {
		this->setAttribute(Qt::WA_TransparentForMouseEvents, true);

		QPoint pt = this->mapTo(this->pParent, event->pos());
		QWidget* wid = this->pParent->childAt(pt);
		if (wid) {
			pt = wid->mapFrom(this->pParent, pt);
			QMouseEvent* mEvent = new QMouseEvent(event->type(), pt, event->button(), event->buttons(), event->modifiers());
			QApplication::postEvent(wid, mEvent);
		}
	}
}

void PenOptionsWidget::mousePressEvent(QMouseEvent* event)
{
	if (geometry().contains(event->globalPos())) {
		event->accept();
	}
	else {
		close();
		QWidget::mousePressEvent(event);
	}
}

void PenOptionsWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (disappearState == 0) {
		event->accept();
	}
	else {
		QWidget::mouseMoveEvent(event);
	}
}

void PenOptionsWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (disappearState == 0) {
		event->accept();
	}
	else {
		QWidget::mouseReleaseEvent(event);
	}
}

void PenOptionsWidget::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void PenOptionsWidget::closeEvent(QCloseEvent* event)
{
	if (disappearState == 0) {
		disappear();
		event->ignore();
	}
	else if (disappearState == 1) {
		animation->stop();
		hide();
		event->ignore();
	}
	else {
		hide();
		event->accept();
	}
}

bool PenOptionsWidget::eventFilter(QObject* obj, QEvent* event)
{
	return QWidget::eventFilter(obj, event);
}
