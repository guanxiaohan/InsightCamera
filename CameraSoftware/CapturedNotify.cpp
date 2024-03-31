#include "CapturedNotify.h"

CapturedNotify::CapturedNotify(QPixmap pixmap, QWidget *parent, int ind)
	: QWidget(parent)
	, ui(new Ui::CapturedNotifyClass()),
	returnIndex(ind)
{
	ui->setupUi(this);
	setParent(parent);
	appearing = true;
	endXPos = parent->width();
	setStyleSheet("QWidget#CapturedNotifyClass{"
		"background-color: rgba(255, 255, 255, 180);"
		"border-radius: 12px;"
		"}"
		"QLabel{"
		"color: #222222;"
		"font-size: 15px;"
		"}"
		"QPushButton{"
		"outline: 0;"
		"border: 0;"
		"}"
	);
	disappearTimer = new QTimer();
	moveAnimation = QSharedPointer<QPropertyAnimation>(new QPropertyAnimation(this, "pos"));
	moveAnimation->setEasingCurve(QEasingCurve::OutQuint);
	ui->AboveLabel->setText(tr("New Capture Saved."));
	ui->PixmapLabel->setPixmap(pixmap.scaled(CapturedNotifyWidth, 150, Qt::KeepAspectRatio));
	ui->DescriptionLabel->setText("Click to switch to this capture.");
	ui->DescriptionLabel->setWordWrap(true);
	disappearTimer->setInterval(6000);
	disappearTimer->setSingleShot(true);
	ui->CloseButton->setIcon(QIcon(":/MainInterface/CrossIcon1"));
	ui->CloseButton->setIconSize(QSize(11, 11));
	ui->CloseButton->installEventFilter(this);
	ui->CloseButton->setText(QString());
	setFocus();
	connect(disappearTimer, &QTimer::timeout, this, &CapturedNotify::tryDisappear);
	connect(ui->CloseButton, &QPushButton::clicked, this, &CapturedNotify::tryDisappear);
}

CapturedNotify::~CapturedNotify()
{
	delete disappearTimer;
	delete ui;
}

void CapturedNotify::appear()
{
	moveAnimation->setStartValue(QPoint(x(), y()));
	moveAnimation->setEndValue(QPoint(x() - CapturedNotifyWidth - 5, y()));
	moveAnimation->setDuration((CapturedNotifyWidth + 5) * 2.2);
	moveAnimation->start();
	connect(moveAnimation.data(), &QPropertyAnimation::finished, this, &CapturedNotify::appearFinished);
	disappearTimer->start();
}

void CapturedNotify::disappear()
{
	disappearTimer->stop();
	moveAnimation->stop();
	moveAnimation->setStartValue(QPoint(x(), y()));
	moveAnimation->setEndValue(QPoint(endXPos + 1, y()));
	moveAnimation->setDuration((endXPos - x()) * 2.2);
	moveAnimation->start();
	connect(moveAnimation.data(), &QPropertyAnimation::finished, this, &CapturedNotify::disappearFinished);
}

void CapturedNotify::noFocused()
{
	if (geometry().contains(QCursor::pos()) && not ui->CloseButton->underMouse()) {
		recall();
	}
	else {
		tryDisappear();
	}
}

void CapturedNotify::tryDisappear()
{
	if (appearing && moveAnimation->state() == QPropertyAnimation::Running) {
		moveAnimation->stop();
		disappear();
	}
	else if (moveAnimation->state() == QPropertyAnimation::Stopped && not appearing) {
		disappear();
	}
}

void CapturedNotify::recall()
{
	disappear();
	emit switchToPixmap(returnIndex);
}

void CapturedNotify::disappearFinished() 
{
	emit disappeared();
}

void CapturedNotify::appearFinished() 
{
	appearing = false;
}

void CapturedNotify::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CapturedNotify::mousePressEvent(QMouseEvent* event)
{
	recall();
	QWidget::mousePressEvent(event);
}

bool CapturedNotify::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == ui->CloseButton && event->type() == QEvent::HoverEnter) {
		ui->CloseButton->setIcon(QIcon(":/MainInterface/CrossIcon2"));
	}
	else if (obj == ui->CloseButton && event->type() == QEvent::HoverLeave) {
		ui->CloseButton->setIcon(QIcon(":/MainInterface/CrossIcon1"));
	}
	return QWidget::eventFilter(obj, event);
}
