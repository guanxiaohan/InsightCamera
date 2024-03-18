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
		"font-size: 16px;"
		"color: #444444;"
		"border: 0;"
		"margin: 0;"
		"}"
		"QToolButton:hover{"
		"background-color: rgba(0, 0, 0, 0);"
		"}"
		"QToolButton:pressed{"
		"padding: 0px;"
		"}"
		"QToolButton:checked{"
		"padding: 0px;"
		"}");
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	setToolTip(text);
	setCheckable(true);
	setObjectName(iconid + "Button");

	connect(this, &QToolButton::clicked, this, &ToolBarButton::onClicked);
}

ToolBarButton::~ToolBarButton()
{

}

void ToolBarButton::hoverEnter(ToolBarButton* iconObj)
{
	if (iconObj == this) {
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
	if (iconObj == this) {
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
	case Hover:
		setIcon(iconHover);
	case Checked:
		setIcon(iconChecked);
	case CheckHover:
		setIcon(iconCheckHover);
	}
}

void ToolBarButton::iconChange()
{

}

void ToolBarButton::onClicked()
{
	setChecked(true);
	setIcon(iconCheckHover);
	emit buttonPressed();
}
