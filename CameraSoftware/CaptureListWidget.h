#pragma once

#include <QListWidget>
#include <QCheckBox>
#include <QLabel>
#include <QListWidgetItem>
#include <QIcon>
#include <QList>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>

class CaptureListWidget  : public QListWidget
{
	Q_OBJECT

public:
	CaptureListWidget(QWidget* parent);
	~CaptureListWidget();
	void addPixmap(QPixmap);
	void updatePixmap(int index, QPixmap pixmap);
	void updateCapture(int index, QImage pixmap);
	void deselectAll();
	void removePixmap(int index);
	void setSelectingIndex(int index);

private:
	void clickedItem(QListWidgetItem* pos);
	int selectingIndex = -1;

signals:
	void indexChanged(int);
};

class CustomItemDelegate : public QStyledItemDelegate
{
public:
	CustomItemDelegate(QWidget* pParent) :QStyledItemDelegate(pParent) {};
	virtual ~CustomItemDelegate() {};

private:
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void initCustomStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const;
};
