#include "AnimationSlider.h"

AnimationSlider::AnimationSlider(QWidget* parent, int maxValue, int value, QPoint centerGeometry, bool horizontal = true)
	: QSlider(parent)
{
	animation = QSharedPointer<QPropertyAnimation>(new QPropertyAnimation(this, "geometry"));

	setMinimum(0);
	setMaximum(maxValue);
	setValue(value);
	setOrientation(horizontal ? Qt::Horizontal : Qt::Vertical);
	setStyleSheet(""
		"QSlider::groove:horizontal{"
		"border: 0px;"
		"}"
		"QSlider::sub-page:horizontal{"
		"background-color: #aaaaaadd;"
		"border-radius: 3px;"
		"}"
		"QSlider::add-page:horizontal{"
		"background-color: #eeeeeedd;"
		"border: 0px solid #000;"
		"border-radius: 3px;"
		"}"
		"QSlider::handle:horizontal{"
		"background: rgba(255, 255, 255, 200);"
		"width: 28px;"
		"border: 2px solid #888888;"
		"border-radius: 11px;"
		"}"
		"QSlider::handle:horizontal:hover{"
		"background: #ffffff;"
		"}"
		"QSlider::groove:vertical{"
		"border: 0px;"
		"}"
		"QSlider::sub-page:vertical{"
		"background-color: #aaaaaadd;"
		"border-radius: 3px;"
		"}"
		"QSlider::add-page:vertical{"
		"background-color: #eeeeeedd;"
		"border: 0px solid #000;"
		"border-radius: 3px;"
		"}"
		"QSlider::handle:vertical{"
		"background: rgba(255, 255, 255, 200);"
		"width: 28px;"
		"border: 2px solid #888888;"
		"border-radius: 15px;"
		"}"
		"QSlider::handle:vertical:hover{"
		"background: #ffffff;"
		"}"
	);
	show();

	// Position Algorithm
	animation->setDuration(390);
	animation->setEasingCurve(QEasingCurve::OutQuint);
	animation->setStartValue(QRect(centerGeometry - QPoint(CircleWidth / 2, CircleWidth / 2), centerGeometry + QPoint(CircleWidth / 2, CircleWidth / 2)));
	double percent = (double) value / maxValue;
	if (horizontal) {
		animation->setEndValue(QRect(centerGeometry - QPoint(CircleWidth / 2 + SliderLength * percent, CircleWidth / 2), centerGeometry + QPoint(CircleWidth / 2 + SliderLength * (1.0 - percent), CircleWidth / 2)));
	}
	animation->start();
	setMouseTracking(true);
}

AnimationSlider::~AnimationSlider()
{
	releaseMouse();
}

void AnimationSlider::disappearFinished()
{
	if (disappearing)
		hide();
}

void AnimationSlider::mouseReleaseEvent(QMouseEvent* event)
{
	releaseMouse();
	setMouseTracking(false);
	disappearing = true;
	animation->stop();
	animation->setStartValue(geometry());
	animation->setEndValue(QRect(geometry().x() + geometry().width() / 2, geometry().y(), 0, geometry().height()));
	animation->setCurrentTime(0);
	animation->start();
	connect(animation.data(), &QPropertyAnimation::finished, this, &AnimationSlider::disappearFinished);
}

void AnimationSlider::mousePressEvent(QMouseEvent* event)
{
	QSlider::mousePressEvent(event);
}

void AnimationSlider::mouseMoveEvent(QMouseEvent* event)
{
	QSlider::mouseMoveEvent(event);
	double pos = (event->pos().x() - 14) / ((double)width() - 28);
	double value = pos * (maximum() - minimum()) + minimum();
	if (value > maximum()) {
		value = maximum();
	}
	if (value < minimum()) {
		value = minimum();
	}
	
	setValue(value + 0.5);
}
