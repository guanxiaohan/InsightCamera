#pragma once

#include <QWidget>
#include <QPainter>
#include <QStyleOption>
#include <QLabel>
#include <QPaintEvent>
#include <QEvent>
#include "ui_CapturesList.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CapturesListClass; };
QT_END_NAMESPACE

constexpr int CapturesListWidth = 225;
constexpr int CapturesListHeight = 768;

class CapturesList : public QWidget
{
	Q_OBJECT

public:
	CapturesList(QWidget *parent = nullptr);
	~CapturesList();
	CaptureListWidget* AboveListWidget;
	CaptureListWidget* BelowListWidget;

private:
	Ui::CapturesListClass *ui;

protected:
	void paintEvent(QPaintEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* event) override;
};
