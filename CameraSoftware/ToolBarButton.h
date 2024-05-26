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
	ToolBarButton(QWidget *parent);
	~ToolBarButton();

	enum IconState {Normal, Hover, Checked, CheckHover};

	void hoverEnter(ToolBarButton* iconObj);
	void hoverLeave(ToolBarButton* iconObj);
	void setCheckState(bool check);
	void setIconState(IconState iconstate);
	void setIconid(QString iconid);
	void changeIcon(QString iconid);
	void changeText(QString text);

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
