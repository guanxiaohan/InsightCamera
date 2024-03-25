#include "ToolBarButton.h"

ToolBarButton::ToolBarButton(QWidget *parent, QString iconid, QString text)
	: QToolButton(parent)
{
	iconNormal = QIcon(":/MainInterface/" + iconid + "Icon1");
	iconHover = QIcon(":/MainInterface/" + iconid + "Icon2");
	iconChecked = QIcon(":/MainInterface/" + iconid + "Icon3");
	iconCheckHover = QIcon(":/MainInterface/" + iconid + "Icon4");

	iconId = iconid;
	setText(text);
	setIcon(iconNormal);
	setStyleSheet("QToolButton{"
		"font-size: 17px;"
		"color: #424242;"
		"border: 0;"
		"margin: 0 0;"
		"padding: 5px 0;"
		"}"
		"QToolButton:hover{"
		"background-color: rgba(0, 0, 0, 0);"
		"color: #666666;"
		"}"
		"QToolButton:pressed{"
		"padding: 5px 0;"
		"}"
		"QToolButton:checked{"
		"padding: 5px 0;"
		"color: #00AC55;"
		"}"
		"QToolButton:checked:hover{"
		"padding: 5px 0;"
		"color: #00BB55;"
		"}");
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	setToolTip(text);
	setCheckable(true);
	setObjectName(iconid + "Button");

	connect(this, &QToolButton::clicked, this, &ToolBarButton::onClicked);
}

ToolBarButton::ToolBarButton(QWidget* parent)
{
	setStyleSheet("QToolButton{"
		"font-size: 14px;"
		"color: #424242;"
		"border: 0;"
		"margin: 0 0;"
		"padding: 5px 0;"
		"}"
		"QToolButton:hover{"
		"background-color: rgba(0, 0, 0, 0);"
		"color: #666666;"
		"}"
		"QToolButton:pressed{"
		"padding: 5px 0;"
		"}"
		"QToolButton:checked{"
		"padding: 5px 0;"
		"color: #00AC55;"
		"}"
		"QToolButton:checked:hover{"
		"padding: 5px 0;"
		"color: #00BB55;"
		"}");
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	setObjectName("ToolBarButton");
	setCheckable(false);

	connect(this, &QToolButton::clicked, this, &ToolBarButton::onClicked);
}

ToolBarButton::~ToolBarButton()
{

}

void ToolBarButton::hoverEnter(ToolBarButton* iconObj)
{
	if (iconObj == this or iconObj == nullptr) {
		if (isChecked()) {
			setIcon(iconCheckHover);
		}
		else {
			setIcon(iconHover);
		}
	}
}

void ToolBarButton::hoverLeave(ToolBarButton* iconObj)
{
	if (iconObj == this or iconObj == nullptr) {
		if (isChecked()) {
			setIcon(iconChecked);
		}
		else {
			setIcon(iconNormal);
		}
	}
}

void ToolBarButton::setCheckState(bool check)
{
	if (not isCheckable()) 
		return;
	setChecked(check);
	if (check) {
		setIcon(iconChecked);
	}
	else {
		setIcon(iconNormal);
	}
}

void ToolBarButton::setIconState(IconState iconstate)
{
	switch (iconstate) {
	case Normal:
		setIcon(iconNormal);
		break;
	case Hover:
		setIcon(iconHover);
		break;
	case Checked:
		setIcon(iconChecked);
		break;
	case CheckHover:
		setIcon(iconCheckHover);
		break;
	}
}

void ToolBarButton::setIconid(QString iconid)
{
	iconNormal = QIcon(":/MainInterface/" + iconid + "Icon1");
	iconHover = QIcon(":/MainInterface/" + iconid + "Icon2");
	iconChecked = QIcon(":/MainInterface/" + iconid + "Icon3");
	iconCheckHover = QIcon(":/MainInterface/" + iconid + "Icon4");
	iconId = iconid;
	setIcon(iconNormal);
	setObjectName(iconid + "Button");
}

void ToolBarButton::iconChange()
{

}

void ToolBarButton::onClicked()
{
	emit buttonClicked();
}

void ToolBarButton::onToggledTo()
{
	emit buttonToggledTo();
}

void ToolBarButton::mousePressEvent(QMouseEvent* event)
{
	if (not isCheckable())
		setIcon(iconCheckHover);
	else {
		setCheckState(true);
		setIconState(CheckHover);
		onToggledTo();
	}
}

void ToolBarButton::mouseReleaseEvent(QMouseEvent* event)
{
	if (not isCheckable()) {
		setIcon(iconHover);
		auto g = geometry();
		if (event->pos().x() > 0 && event->pos().y() > 0 && event->pos().x() < g.width() && event->pos().y() < g.height()) {
			onClicked();
		}
	}
}
