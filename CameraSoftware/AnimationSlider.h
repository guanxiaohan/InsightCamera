#pragma once

#include <QSlider>
#include <QRect>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QSharedPointer>
#include <QMouseEvent>

constexpr int SliderLength = 280;
constexpr int CircleWidth = 28;

class AnimationSlider  : public QSlider
{
	Q_OBJECT

public:
	AnimationSlider(QWidget* parent, int maxValue, int value, QPoint centerGeometry, bool horizontal);
	~AnimationSlider();

private:
	QSharedPointer<QPropertyAnimation> animation;
	bool disappearing = false;
	void disappearFinished();

protected:
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
};
