#include "CapturesList.h"

CapturesList::CapturesList(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::CapturesListClass())
{
	ui->setupUi(this);
	setObjectName("CapturesList");
	setStyleSheet(
		"QWidget#CapturesList{"
		"background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 255, 200), stop:1 rgba(255, 255, 255, 100));"
		"border: 4px solid rgba(255, 255, 255, 200);"
		"padding: 4px;"
		"border-radius: 13px;"
		"}"
	);
	ui->SaveButton->setIconid(tr("Save"));
	ui->LoadButton->setIconid(tr("Load"));
	ui->CompareButton->setIconid(tr("Compare"));
	ui->RemoveButton->setIconid(tr("Remove"));
	ui->SaveButton->installEventFilter(this);
	ui->LoadButton->installEventFilter(this);
	ui->CompareButton->installEventFilter(this);
	ui->RemoveButton->installEventFilter(this);

	AboveListWidget = ui->CaptureListAbove;
	BelowListWidget = ui->CaptureListBelow;
}

CapturesList::~CapturesList()
{
	delete ui;
}

void CapturesList::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

bool CapturesList::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::HoverEnter) {
		if (obj == ui->SaveButton) {
			ui->SaveButton->hoverEnter(nullptr);
		}
		else if (obj == ui->CompareButton) {
			ui->CompareButton->hoverEnter(nullptr);
		}
		else if (obj == ui->LoadButton) {
			ui->LoadButton->hoverEnter(nullptr);
		}
		else if (obj == ui->RemoveButton) {
			ui->RemoveButton->hoverEnter(nullptr);
		}
	}
	else if (event->type() == QEvent::HoverLeave) {
		if (obj == ui->SaveButton) {
			ui->SaveButton->hoverLeave(nullptr);
		}
		else if (obj == ui->LoadButton) {
			ui->LoadButton->hoverLeave(nullptr);
		}
		else if (obj == ui->CompareButton) {
			ui->CompareButton->hoverLeave(nullptr);
		}
		else if (obj == ui->RemoveButton) {
			ui->RemoveButton->hoverLeave(nullptr);
		}
	}
	return QWidget::eventFilter(obj, event);
}
