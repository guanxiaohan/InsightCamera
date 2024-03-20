#pragma once

#include <QWidget>
#include "ui_CapturesList.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CapturesListClass; };
QT_END_NAMESPACE

class CapturesList : public QWidget
{
	Q_OBJECT

public:
	CapturesList(QWidget *parent = nullptr);
	~CapturesList();

private:
	Ui::CapturesListClass *ui;
};
