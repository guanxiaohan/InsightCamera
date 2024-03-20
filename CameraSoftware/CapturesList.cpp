#include "CapturesList.h"

CapturesList::CapturesList(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::CapturesListClass())
{
	ui->setupUi(this);
}

CapturesList::~CapturesList()
{
	delete ui;
}
