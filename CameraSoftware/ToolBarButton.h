#pragma once

#include <QToolButton>
#include <QMouseEvent>
#include <QObject>
#include <QString>

class ToolBarButton  : public QToolButton
{
	Q_OBJECT

public:
	ToolBarButton(QWidget *parent, QString iconid, QString text);
	~ToolBarButton();

	enum IconState {Normal, Hover, Checked, CheckHover};

	void hoverEnter(ToolBarButton* iconObj);
	void hoverLeave(ToolBarButton* iconObj);
	void setCheckState(bool check);
	void setIconState(IconState iconstate);

private:
	QIcon iconNormal;
	QIcon iconHover;
	QIcon iconChecked;
	QIcon iconCheckHover;
	QString iconId;

	void iconChange();
	void onClicked();
	void onToggledTo();

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

signals:
	void buttonClicked();
	void buttonToggledTo();
};
