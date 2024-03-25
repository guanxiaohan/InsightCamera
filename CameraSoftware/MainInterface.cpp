#include "MainInterface.h"

MainInterface::MainInterface(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainInterfaceClass())
{
	ui->setupUi(this);

	cameraManager = new CameraManager(this);
	videoItem = new BottomVideoGraphicsItem(this);
	captureSession = new QMediaCaptureSession();

	setWindowState(Qt::WindowFullScreen);

	MenuButton = new ToolBarButton(this, "Menu", "Menu");
	SelectButton = new ToolBarButton(this, "Select", "Select");
	PenButton = new ToolBarButton(this, "Pen", "Pen");
	EraserButton = new ToolBarButton(this, "Eraser", "Eraser");
	UndoButton = new ToolBarButton(this, "Undo", "Undo");
	CaptureButton = new ToolBarButton(this, "Capture", "Cptr.");
	
	MenuButton->installEventFilter(this);
	connect(MenuButton, &ToolBarButton::buttonToggledTo, this, &MainInterface::MenuButtonClicked);
	connect(this, &MainInterface::ToolBarButtonHover, MenuButton, &ToolBarButton::hoverEnter);
	connect(this, &MainInterface::ToolBarButtonLeave, MenuButton, &ToolBarButton::hoverLeave);
	SelectButton->installEventFilter(this);
	connect(SelectButton, &ToolBarButton::buttonToggledTo, this, &MainInterface::SelectButtonClicked);
	connect(this, &MainInterface::ToolBarButtonHover, SelectButton, &ToolBarButton::hoverEnter);
	connect(this, &MainInterface::ToolBarButtonLeave, SelectButton, &ToolBarButton::hoverLeave);
	PenButton->installEventFilter(this);
	connect(PenButton, &ToolBarButton::buttonToggledTo, this, &MainInterface::PenButtonClicked);
	connect(this, &MainInterface::ToolBarButtonHover, PenButton, &ToolBarButton::hoverEnter);
	connect(this, &MainInterface::ToolBarButtonLeave, PenButton, &ToolBarButton::hoverLeave);
	EraserButton->installEventFilter(this);
	connect(EraserButton, &ToolBarButton::buttonToggledTo, this, &MainInterface::EraserButtonClicked);
	connect(this, &MainInterface::ToolBarButtonHover, EraserButton, &ToolBarButton::hoverEnter);
	connect(this, &MainInterface::ToolBarButtonLeave, EraserButton, &ToolBarButton::hoverLeave);
	UndoButton->installEventFilter(this);
	connect(UndoButton, &ToolBarButton::buttonClicked, this, &MainInterface::UndoButtonClicked);
	connect(this, &MainInterface::ToolBarButtonHover, UndoButton, &ToolBarButton::hoverEnter);
	connect(this, &MainInterface::ToolBarButtonLeave, UndoButton, &ToolBarButton::hoverLeave);
	CaptureButton->installEventFilter(this);
	connect(CaptureButton, &ToolBarButton::buttonClicked, this, &MainInterface::CaptureButtonClicked);
	connect(this, &MainInterface::ToolBarButtonHover, CaptureButton, &ToolBarButton::hoverEnter);
	connect(this, &MainInterface::ToolBarButtonLeave, CaptureButton, &ToolBarButton::hoverLeave);

	SelectButton->setCheckState(true);
	PenButton->setCheckState(false);
	EraserButton->setCheckState(false);
	UndoButton->setCheckable(false);
	CaptureButton->setCheckable(false);

	RightWidgetButton = new QToolButton(this);
	RightWidgetButton->setProperty("toLeft", 1);
	RightWidgetButton->setStyleSheet(
		"QToolButton{"
		"image: url(:/MainInterface/LeftArrowIcon1);"
		"background-color: rgba(190, 190, 190, 140);"
		"border-radius: 10px;"
		"}"
		"QToolButton:hover{"
		"image: url(:/MainInterface/LeftArrowIcon2);"
		"background-color: rgba(230, 230, 230, 140);"
		"}"
		"QToolButton:pressed{"
		"image: url(:/MainInterface/LeftArrowIcon4);"
		"background-color: rgba(230, 230, 230, 160);"
		"}"
		"QToolButton[toLeft='2']{"
		"image: url(:/MainInterface/RightArrowIcon1);"
		"background-color: rgba(190, 190, 190, 140);"
		"border-radius: 10px;"
		"}"
		"QToolButton:hover[toLeft='2']{"
		"image: url(:/MainInterface/RightArrowIcon2);"
		"background-color: rgba(230, 230, 230, 140);"
		"}"
		"QToolButton:pressed[toLeft='2']{"
		"image: url(:/MainInterface/RightArrowIcon4);"
		"background-color: rgba(230, 230, 230, 160);"
		"}"
	);
	RightWidgetButton->setGeometry(QRect(width() - 36, height() / 2 - 45, 32, 90));
	connect(RightWidgetButton, &QToolButton::clicked, this, &MainInterface::RightWidgetButtonClicked);

	removeToolBar(ui->mainToolBar);
	ToolBar = new UnderToolBar(this);
	ToolBar->addWidget(MenuButton);
	ToolBar->addSeparator();
	ToolBar->addWidget(SelectButton);
	ToolBar->addWidget(PenButton);
	ToolBar->addWidget(EraserButton);
	ToolBar->addWidget(UndoButton);
	ToolBar->addSeparator();
	ToolBar->addWidget(CaptureButton);
	ToolBar->moveAnimation->setStartValue(QRect(width() / 2, height() - 84, 10, 76));
	ToolBar->moveAnimation->setEndValue(QRect(width() / 2 - ToolBar->getWidth() / 2 - ToolBarMargin, height() - 84, ToolBar->getWidth() + ToolBarMargin*2, 76));
	ToolBar->moveAnimation->setEasingCurve(QEasingCurve::OutQuart);
	ToolBar->moveAnimation->setDuration(680);
	ToolBar->moveAnimation->start();

	CapturesWidget = new CapturesList(this);
	CapturesWidget->setGeometry(QRect(width() + 1, height() - CapturesListHeight, CapturesListWidth, CapturesListHeight));
	CapturesWidget->AboveListWidget->addPixmap(QPixmap());
	CapturesWidget->AboveListWidget->setSelectingIndex(0);
	CapturesWidget->BelowListWidget->setProperty("idNeeded", true);
	connect(CapturesWidget->AboveListWidget, &CaptureListWidget::indexChanged, this, &MainInterface::changeAboveSceneIndex);
	connect(CapturesWidget->BelowListWidget, &CaptureListWidget::indexChanged, this, &MainInterface::changeBelowSceneIndex);

	updateTimer = new QTimer();
	updateTimer->setInterval(32);
	connect(updateTimer, &QTimer::timeout, this, &MainInterface::updateCapture);
	updateTimer->start();
	initailizeCamera();

	mainScene = new QGraphicsScene();
	mainScene->addItem(videoItem);
	ui->CameraFrame->setScene(mainScene);
	ui->CameraFrame->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->CameraFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->CameraFrame->bottomItem = videoItem;
	ui->statusBar->hide();

	setCentralWidget(ui->CameraFrame);
	ui->CameraFrame->resetViewPort(width() * 2, height() * 2);
	ToolBar->raise();
	RightWidgetButton->raise();
	CapturesWidget->raise();
}

MainInterface::~MainInterface()
{
	delete captureSession;
	delete cameraManager;
	for (auto item : mainScene->items()) {
		mainScene->removeItem(item);
		delete item;
	}
	delete mainScene;
	delete updateTimer;
	/*delete MenuButton;
	delete SelectButton;
	delete PenButton;
	delete EraserButton;
	delete UndoButton;
	delete CaptureButton;*/
	delete RightWidgetButton;
	delete ToolBar;
	for (auto i : captureScenes) {
		for (auto item : i->items()) {
			i->removeItem(item);
			delete item;
		}
		delete i;
	}
	delete ui;
}

void MainInterface::initailizeCamera()
{
	if (cameraManager->checkCameraAvailablity()) {
		cameraManager->setCamera(cameraManager->getAvailableCameras()[0]);
	}
	if (cameraManager->isAvailable()) {
		captureSession->setCamera(cameraManager->camera());
		captureSession->setImageCapture(&imageCapture);
		connect(&imageCapture, &QImageCapture::imageCaptured, this, &MainInterface::captureReturned);
		auto Resolution = cameraManager->getMaxResolution();
		videoItem->setSize(Resolution);
		CapturesWidget->AboveListWidget->setFixedHeight(Resolution.height() / (Resolution.width() / 170) + 10);
	}
	captureSession->setVideoOutput(videoItem);
}

void MainInterface::operationMenuReturn(int index)
{
	MenuButton->setCheckState(false);
	switch (index) {
	case 0:
		QMessageBox::information(this, "About", "Nothing here.");
		break;
	case 1:
		showMinimized();
		break;
	case 2:
		exit(0);
		break;
	}
}

void MainInterface::changeAboveSceneIndex(int)
{
	ui->CameraFrame->setScene(mainScene);
	CapturesWidget->BelowListWidget->setSelectingIndex(-1);
}

void MainInterface::changeBelowSceneIndex(int index)
{
	ui->CameraFrame->setScene(captureScenes[index]);
	CapturesWidget->AboveListWidget->setSelectingIndex(-1);
}

void MainInterface::switchRightSideBar()
{
	RightButtonAnimation = QSharedPointer<QPropertyAnimation>(new QPropertyAnimation(RightWidgetButton, "pos"));
	CapturesWidgetAnimation = QSharedPointer<QPropertyAnimation>(new QPropertyAnimation(CapturesWidget, "pos"));
	if (SideBarShowing == 0) {
		SideBarShowing = 1;
		RightWidgetButton->setProperty("toLeft", 2);
		int startX = RightWidgetButton->x();
		int endX = width() - CapturesListWidth - 4 - 32;
		int startX2 = CapturesWidget->x();
		int endX2 = width() - CapturesListWidth;
		RightButtonAnimation->setEndValue(QPoint(endX, RightWidgetButton->y()));
		RightButtonAnimation->setDuration((startX - endX) * 2.3);
		RightButtonAnimation->setEasingCurve(QEasingCurve::OutQuint);
		RightButtonAnimation->start();
		CapturesWidgetAnimation->setStartValue(QPoint(startX2, CapturesWidget->y()));
		CapturesWidgetAnimation->setEndValue(QPoint(endX2, CapturesWidget->y()));
		CapturesWidgetAnimation->setDuration((startX2 - endX2) * 2.3);
		CapturesWidgetAnimation->setEasingCurve(QEasingCurve::OutQuint);
		CapturesWidgetAnimation->start();
	}
	else if (SideBarShowing == 1){
		SideBarShowing = 0;
		RightWidgetButton->setProperty("toLeft", 1);
		int startX = RightWidgetButton->x();
		int endX = width() - 4 - 32;
		int startX2 = CapturesWidget->x();
		int endX2 = width() + 1;
		RightButtonAnimation->setStartValue(QPoint(startX, RightWidgetButton->y()));
		RightButtonAnimation->setEndValue(QPoint(endX, RightWidgetButton->y()));
		RightButtonAnimation->setDuration((endX - startX) * 2.3);
		RightButtonAnimation->setEasingCurve(QEasingCurve::OutQuint);
		RightButtonAnimation->start();
		RightButtonAnimation->setStartValue(QPoint(startX, RightWidgetButton->y()));
		CapturesWidgetAnimation->setStartValue(QPoint(startX2, CapturesWidget->y()));
		CapturesWidgetAnimation->setEndValue(QPoint(endX2, CapturesWidget->y()));
		CapturesWidgetAnimation->setDuration((endX2 - startX2) * 2.3);
		CapturesWidgetAnimation->setEasingCurve(QEasingCurve::OutQuint);
		CapturesWidgetAnimation->start();
	}
	RightWidgetButton->style()->polish(RightWidgetButton);
}

void MainInterface::updateCapture()
{
	imageCapture.capture();
}

void MainInterface::captureReturned(int, QImage pix)
{
	CapturesWidget->AboveListWidget->updateCapture(0, pix);
	realtimeCapture = pix;
}

void MainInterface::captureNotifyClicked(int index)
{
	changeBelowSceneIndex(index);
	CapturesWidget->BelowListWidget->setSelectingIndex(index);
	CapturesWidget->update();
}

void MainInterface::ResetToolButtons()
{
	SelectButton->setCheckState(false);
	PenButton->setCheckState(false);
	EraserButton->setCheckState(false);
}

void MainInterface::MenuButtonClicked()
{
	PopMenu = QSharedPointer<AnimationMenu>(new AnimationMenu(this));
	PopMenu->addMenuItem("About", "About");
	PopMenu->addMenuItem("Minimize", "Minimize");
	PopMenu->addMenuItem("Exit", "Exit");
	connect(PopMenu.data(), &AnimationMenu::returnAction, this, &MainInterface::operationMenuReturn);
	menuShowing = true;
	PopMenu->exec(QPoint(ToolBar->x(),ToolBar->y() - PopMenu->getHeight() - 10));
}

void MainInterface::SelectButtonClicked()
{
	ResetToolButtons();
	SelectButton->setCheckState(true);
	SelectButton->setIconState(ToolBarButton::CheckHover);
	nowTool = Select;
	ui->CameraFrame->setFrameDraggable(FrameGraphicsView::FrameSelect);
}

void MainInterface::PenButtonClicked()
{
	ResetToolButtons();
	PenButton->setCheckState(true);
	PenButton->setIconState(ToolBarButton::CheckHover);
	nowTool = Pen;
	ui->CameraFrame->setFrameDraggable(FrameGraphicsView::FramePen);
}

void MainInterface::EraserButtonClicked()
{
	ResetToolButtons();
	EraserButton->setCheckState(true);
	EraserButton->setIconState(ToolBarButton::CheckHover);
	nowTool = Eraser;
	ui->CameraFrame->setFrameDraggable(FrameGraphicsView::FrameEraser);
}

void MainInterface::UndoButtonClicked()
{
}

void MainInterface::CaptureButtonClicked()
{
	auto scene = new QGraphicsScene();
	auto item = new QGraphicsPixmapItem();
	auto picture = QPixmap::fromImage(realtimeCapture);
	picture = picture.scaled(width(), height(), Qt::KeepAspectRatio);
	item->setPixmap(picture);
	item->setPos(width() / 2, height() / 2);
	scene->addItem(item);
	scene->setSceneRect(0, 0, width()*2, height()*2);
	captureScenes.append(scene);
	CapturesWidget->BelowListWidget->addPixmap(picture);

	if (SideBarShowing == 0) {
		CapturePopNotify = QSharedPointer<CapturedNotify>(new CapturedNotify(picture, this, CapturesWidget->BelowListWidget->count() - 1));
		CapturePopNotify->setGeometry(width() + 1, height() - CapturePopNotify->sizeHint().height() - 5, CapturedNotifyWidth, CapturePopNotify->sizeHint().height());
		connect(CapturePopNotify.data(), &CapturedNotify::switchToPixmap, this, &MainInterface::captureNotifyClicked);
		CapturePopNotify->installEventFilter(this);
		CapturePopNotify->show();
		CapturePopNotify->raise();
		CapturePopNotify->appear();
	}
}

void MainInterface::RightWidgetButtonClicked()
{
	if (not CapturePopNotify.isNull()) {
		CapturePopNotify->tryDisappear();
	}
	switchRightSideBar();
}

void MainInterface::resizeEvent(QResizeEvent* event) {
	auto windowSize = size();
	videoItem->setSize(windowSize);
	videoItem->setPos(windowSize.width() / 2, windowSize.height() / 2);
}

bool MainInterface::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::HoverEnter){
		if (obj == MenuButton) {
			emit ToolBarButtonHover(MenuButton);
		}
		else if (obj == SelectButton) {
			emit ToolBarButtonHover(SelectButton);
		}
		else if (obj == PenButton) {
			emit ToolBarButtonHover(PenButton);
		}
		else if (obj == EraserButton) {
			emit ToolBarButtonHover(EraserButton);
		}
		else if (obj == UndoButton) {
			emit ToolBarButtonHover(UndoButton);
		}
		else if (obj == CaptureButton) {
			emit ToolBarButtonHover(CaptureButton);
		}
	}
	else if (event->type() == QEvent::HoverLeave){
		if (obj == MenuButton) {
			emit ToolBarButtonLeave(MenuButton);
		}
		else if (obj == SelectButton) {
			emit ToolBarButtonLeave(SelectButton);
		}
		else if (obj == PenButton) {
			emit ToolBarButtonLeave(PenButton);
		}
		else if (obj == EraserButton) {
			emit ToolBarButtonLeave(EraserButton);
		}
		else if (obj == UndoButton) {
			emit ToolBarButtonLeave(UndoButton);
		}
		else if (obj == CaptureButton) {
			emit ToolBarButtonLeave(CaptureButton);
		}
	}
	if (obj == CapturePopNotify.data()) {
		if (event->type() == QEvent::FocusOut) {
			CapturePopNotify->noFocused();
		}
	}
	return QWidget::eventFilter(obj, event);
}
