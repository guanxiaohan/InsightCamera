#include "CaptureListWidget.h"

CaptureListWidget::CaptureListWidget(QWidget *parent)
	: QListWidget(parent)
{
	setIconSize(QSize(170, 130));
	setViewMode(QListView::IconMode);
	setResizeMode(QListView::Adjust);
	setMovement(QListView::Static);
	setItemDelegate(new CustomItemDelegate(this));
	setSelectionMode(QListWidget::NoSelection);
	setDragEnabled(false);
	//setFixedHeight(180);
	setStyleSheet(
		"QListWidget{"
		"background: transparent;"
		"padding: 4px;"
		"border: 0px;"
		"margin: 0px;"
		"}"
		"QListWidget::item:selected{"
		"outline: 0;"
		"}"
		""
	);
	setProperty("idNeeded", false);
	connect(this, &CaptureListWidget::itemClicked, this, &CaptureListWidget::clickedItem);
}

CaptureListWidget::~CaptureListWidget()
{
	for (int i = 0; i < count(); i++) {
		delete takeItem(0);
	}
}

void CaptureListWidget::addPixmap(QPixmap pixmap)
{
	auto Item = new QListWidgetItem();
	Item->setIcon(QIcon(pixmap));
	if (not pixmap.isNull()) {
		auto Size = QSize(170, 1);
		Size.setHeight(pixmap.height() / (pixmap.width() / 170) + 1);
		setIconSize(Size);
	}
	addItem(Item);
}

void CaptureListWidget::updatePixmap(int index, QPixmap pixmap)
{
	if (!pixmap.isNull()) {
		item(index)->setIcon(QIcon(pixmap));
	}
}

void CaptureListWidget::updateCapture(int index, QImage pixmap)
{
	if (!pixmap.isNull()) {
		item(index)->setIcon(QIcon(QPixmap::fromImage(pixmap)));
	}
}

void CaptureListWidget::deselectAll()
{

}

void CaptureListWidget::removePixmap(int index)
{

}

void CaptureListWidget::setSelectingIndex(int index)
{
	selectingIndex = index;
	setProperty("selectingIndex", index);
}

void CaptureListWidget::clickedItem(QListWidgetItem* pos)
{
	auto row = indexFromItem(pos).row();
	if (row != selectingIndex) {
		selectingIndex = row;
		setProperty("selectingIndex", row);
		emit indexChanged(row);
	}
}

void CaptureListWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		dragStart = event->pos();
	}
	QListWidget::mousePressEvent(event);
	dragged = false;
}

void CaptureListWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton) {
		QPoint distance = event->pos() - dragStart;
		verticalScrollBar()->setValue(verticalScrollBar()->value() - distance.y());
		dragStart = event->pos();
	}
	dragged = true;
	QListWidget::mouseMoveEvent(event);
}

void CaptureListWidget::mouseReleaseEvent(QMouseEvent* event)
{
	dragStart = QPoint();
	if (not dragged)
		QListWidget::mouseReleaseEvent(event);
	dragged = false;
}

void CustomItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyleOptionViewItem opt = option;
	initCustomStyleOption(&opt, index);
	QStyledItemDelegate::paint(painter, opt, index);
	auto font = painter->font();
	font.setPixelSize(16);
	painter->setFont(font);
	opt.rect.adjust(-6, 0, -6, 0);
	painter->drawText(opt.rect, parent()->property("idNeeded").toBool() ? QString::number(index.row() + 1) : ">");
	opt.rect.adjust(22, 3, 0, 0);
	opt.rect.setSize(QSize(21, 21));
	if (index.row() == parent()->property("selectingIndex")) {
		painter->drawPixmap(opt.rect, QPixmap(":/MainInterface/CheckedIcon"));
	}
}

QSize CustomItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setWidth(size.width() + 16);
	return size;
}

void CustomItemDelegate::initCustomStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
	QVariant value = index.data(Qt::FontRole);
	if (value.isValid() && !value.isNull()) {
		option->font = qvariant_cast<QFont>(value).resolve(option->font);
		option->font.setPixelSize(17);
		option->fontMetrics = QFontMetrics(option->font);
	}
	value = index.data(Qt::ForegroundRole);
	if (value.canConvert<QBrush>())
		option->palette.setBrush(QPalette::Text, qvariant_cast<QBrush>(value));
	option->index = index;
	option->features |= QStyleOptionViewItem::HasDisplay;
	option->text = displayText(value, option->locale); //displayText(value, option->locale);
	option->backgroundBrush = qvariant_cast<QBrush>(index.data(Qt::BackgroundRole));
	
	option->rect.adjust(16, 0, 20, 0);
	option->styleObject = nullptr;
}

