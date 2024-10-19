#include "MainInterface.h"

MainInterface::MainInterface(Configuration* configObj, QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainInterfaceClass())
{
	ui->setupUi(this);
	configObject = configObj;
	CameraFrame = new FrameGraphicsView(configObj, centralWidget());
	CameraFrame->setObjectName(QString::fromUtf8("CameraFrame"));
	CameraFrame->setGeometry(QRect(150, 160, 256, 192));

	cameraManager = new CameraManager(this);
	videoItem = new QGraphicsPixmapItem();
	captureSession = new QMediaCaptureSession();
	//captureSession->setVideoOutput(videoItem);

	setWindowState(Qt::WindowFullScreen);
	//resize(1280, 800);

	MenuButton = new ToolBarButton(this, "Menu", tr("Menu"));
	SelectButton = new ToolBarButton(this, "Select", tr("Select"));
	PenButton = new ToolBarButton(this, "Pen", tr("Pen"));
	EraserButton = new ToolBarButton(this, "Eraser", tr("Eraser"));
	UndoButton = new ToolBarButton(this, "Undo", tr("Undo"));
	CaptureButton = new ToolBarButton(this, "Capture", tr("Cptr."));
	
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
	ToolBar->moveAnimation->setStartValue(QRect(width() / 2, height() - ToolBarBottomMargin - ToolBarHeight, 20, ToolBarHeight));
	ToolBar->moveAnimation->setEndValue(QRect(width() / 2 - ToolBar->getWidth() / 2 - ToolBarMargin, height() - ToolBarBottomMargin - ToolBarHeight, ToolBar->getWidth() + ToolBarMargin * 2, ToolBarHeight));
	ToolBar->moveAnimation->setEasingCurve(QEasingCurve::OutQuart);
	ToolBar->moveAnimation->setDuration(700);

	CapturesWidget = new CapturesList(this);
	CapturesWidget->setGeometry(QRect(width() + 1, height() - CapturesListHeight, CapturesListWidth, CapturesListHeight));
	CapturesWidget->AboveListWidget->addPixmap(QPixmap());
	CapturesWidget->AboveListWidget->setSelectingIndex(0);
	CapturesWidget->BelowListWidget->setProperty("idNeeded", true);
	connect(CapturesWidget->AboveListWidget, &CaptureListWidget::indexChanged, this, &MainInterface::changeAboveSceneIndex);
	connect(CapturesWidget->BelowListWidget, &CaptureListWidget::indexChanged, this, &MainInterface::changeBelowSceneIndex);
	
	captureIndexWidget = new ImageIndexWidget(this);
	captureIndexWidget->initGeometry(QPoint(width() / 2, height() - ToolBarHeight - ToolBarBottomMargin - ImageLabelHeight - ImageLabelBottomMargin));
	connect(CameraFrame, &FrameGraphicsView::penDown, [=]() {
		if (configObject->getConfig("autoHidePartToolBar").toBool()) {
			ToolBar->sinkBar(height() - ToolBarHeight - ToolBarBottomMargin + ToolBarSink);
			captureIndexWidget->sinkBar(ToolBarSink);
		}
	});

	updateTimer = new QTimer();
	refreshRate = configObject->getConfig("cameraRefreshRate").toInt();
	updateTimer->setInterval(1000 / refreshRate);
	connect(updateTimer, &QTimer::timeout, this, &MainInterface::updateCapture);
	watchdogTimer = new QTimer();
	watchdogTimer->setInterval(1500);
	connect(watchdogTimer, &QTimer::timeout, this, &MainInterface::initailizeCamera);
	initailizeCamera();
	captureSession->setImageCapture(&imageCapture);
	connect(&imageCapture, &QImageCapture::imageCaptured, this, &MainInterface::captureReturned);
	delayTimer = new QTimer();

	mainScene = new QGraphicsScene();
	mainScene->addItem(videoItem);
	CameraFrame->setScene(mainScene);
	CameraFrame->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	CameraFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	connect(CameraFrame, &FrameGraphicsView::addSlider, this, &MainInterface::frameAddSlider);
	CameraFrame->bottomItem = videoItem;
	ui->statusBar->hide();

	setCentralWidget(CameraFrame);
	CameraFrame->resetViewPort(width() * 2, height() * 2);
	ToolBar->raise();
	ToolBar->moveAnimation->start();
	RightWidgetButton->raise();
	CapturesWidget->raise();
	captureIndexWidget->raise();
	delayTimer->singleShot(720, [=]() {
		captureIndexWidget->switchShowing(true);
		ToolBar->moveAnimation->setDuration(400);
	});

	if (configObject->loadState == Configuration::LSLaterVersion) {
		QMessageBox::information(this, tr("Earlier Version"), tr("InsightCamera last time was opened in a older version.\nThe software may run into fatal problems."));
	}
	else if (configObject->loadState == Configuration::LSError) {
		QMessageBox::information(this, tr("Couldn't Load Settings"), tr("InsightCamera had a problem loading settings from file.\nThe software will run in default settings."));
	}
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
	delete RightWidgetButton;
	delete ToolBar;
	delete delayTimer;
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
	watchdogTimer->stop();
	updateTimer->stop();
	if (configObject->getConfig("preferredCameraDevice").toString().isEmpty()) {
		videoItem->hide();
	}
	else {
		videoItem->show();
	}
	if (cameraManager->checkCameraAvailablity()) {
		auto availables = cameraManager->getAvailableCameras();
		cameraManager->setCamera(cameraManager->getAvailableCameras()[0]);
		for (auto& device : availables) {
			if (configObject->getConfig("preferredCameraDevice") == device.description()) {
				cameraManager->setCamera(device);
				setCameraISO(configObject->getConfig("ISO").toInt());
				break;
			}
		}
		captureSession->setCamera(cameraManager->camera());
		auto Resolution = cameraManager->getMaxResolution();

		auto scale = (double)height() / Resolution.height();
		Resolution.setHeight(Resolution.height() * scale);
		Resolution.setWidth(Resolution.width() * scale);
		videoItemSize = Resolution;
		videoItem->setPos(width() / 2 + (width() - Resolution.width()) / 2,
			height() / 2 + (height() - Resolution.height()) / 2);
		CapturesWidget->AboveListWidget->setFixedHeight(Resolution.height() / (Resolution.width() / 170) + 10);
		updateTimer->start();
	}
	watchdogTimer->start();
}

void MainInterface::frameAddSlider(int maxValue, int value, QPoint position)
{
	/*if (childAt(position) != CameraFrame) {
		qDebug() << childAt(position) << CameraFrame;
		qDebug() << position;
		return;
	}*/
	CameraFrame->isMousePressing = false;
	CameraFrame->isTouchPressing = false;
	FrameScaleSlider = QSharedPointer<AnimationSlider>(new AnimationSlider(this, maxValue, value, position, true));
	FrameScaleSlider->setParent(this);
	connect(FrameScaleSlider.data(), &AnimationSlider::valueChanged, CameraFrame, &FrameGraphicsView::setScaleFromSlider);
	FrameScaleSlider->raise();
	FrameScaleSlider->show();
	CameraFrame->releaseMouse();
	QPoint pt = QCursor().pos();
	QMouseEvent* mEvent = new QMouseEvent(QEvent::MouseButtonPress, pt, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QApplication::postEvent(FrameScaleSlider.data(), mEvent);
	QMouseEvent* nEvent = new QMouseEvent(QEvent::MouseButtonRelease, pt, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QApplication::postEvent(CameraFrame, nEvent);
	FrameScaleSlider->grabMouse();
}

void MainInterface::operationMenuReturn(int index)
{
	MenuButton->setCheckState(false);
	switch (index) {
	case 0:
	{
		const QString message = tr("InsightCamera Software\nVersion %1\nCreator: Guan Xiaohan\n"
			"Testers: Guan Xiaohan & All teachers in Grade 2022 Class 4 that used this software\n"
			"To replace the old and limited camera software.\n"
			"Project is available on GitHub: https://github.com/guanxiaohan/InsightCamera"
		).arg(VERSION_STR);
		QMessageBox::information(this, tr("About"), message);
		break;
	}
	case 1:
		showSettingsDialog();
		break;
	case 2:
		showMinimized();
		break;
	case 3:
		configObject->saveConfig();
		exit(0);
		break;
	}
}

void MainInterface::changeAboveSceneIndex(int)
{
	CameraFrame->setScene(mainScene);
	CapturesWidget->BelowListWidget->setSelectingIndex(-1);
	captureIndexWidget->setLabel(-1);
	if (captureShowing) {
		captureShowing = false;
		CaptureButton->changeIcon("Capture");
		CaptureButton->changeText(tr("Cptr."));
	}
}

void MainInterface::changeBelowSceneIndex(int index)
{
	CameraFrame->setScene(captureScenes[index]);
	CapturesWidget->AboveListWidget->setSelectingIndex(-1);
	captureIndexWidget->setLabel(index);
	if (not captureShowing) {
		captureShowing = true;
		CaptureButton->changeIcon("BackCamera");
		CaptureButton->changeText(tr("Back"));
	}
}

void MainInterface::setCameraISO(int iso)
{
	if (iso == -1)
		cameraManager->camera()->setAutoIsoSensitivity();
	else
		cameraManager->camera()->setManualIsoSensitivity(iso);
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

void MainInterface::showSettingsDialog()
{
	auto configDialog = ConfigDialog(configObject, cameraManager, this);
	configDialog.initailizeUi();
	connect(&configDialog, &ConfigDialog::switchCamera, [=](int index) {
		if (index == -2) {
			QMessageBox::critical(this, tr("Camera Not Found"), tr("We couldn't find the camera.\nPerhaps it has disconnected from your PC.\nRefresh the camera list."));
		}
		else if (index == -1) {
			videoItem->hide();
		}
		else {
			initailizeCamera();
			videoItem->show();
		}
	});
	connect(&configDialog, &ConfigDialog::changeRefreshRate, [=](int rate) {
		refreshRate = rate;
		updateTimer->setInterval(1000 / refreshRate);;
	});
	connect(&configDialog, &ConfigDialog::changeISO, this, &MainInterface::setCameraISO);
	connect(&configDialog, &ConfigDialog::switchCurrentFrameShowing, captureIndexWidget.data(), &ImageIndexWidget::switchShowing);
	configDialog.exec();
}

void MainInterface::updateCapture()
{
	imageCapture.capture();
}

void MainInterface::captureReturned(int, QImage pix)
{
	QPixmap pixmap = QPixmap::fromImage(pix);
	CapturesWidget->AboveListWidget->updatePixmap(0, pixmap);
	pixmap = pixmap.scaled(videoItemSize, Qt::KeepAspectRatio);
	realtimeCapture = pixmap;
	videoItem->setPixmap(pixmap);
	watchdogTimer->stop();
	watchdogTimer->start();
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
	ToolBar->floatBar(height() - ToolBarBottomMargin - ToolBarHeight);
	captureIndexWidget->floatBar();
}

void MainInterface::MenuButtonClicked()
{
	int ToolBarY = height() - ToolBarBottomMargin - ToolBarHeight;
	ToolBar->floatBar(ToolBarY);
	captureIndexWidget->floatBar();

	PopMenu = QSharedPointer<AnimationMenu>(new AnimationMenu(this));
	PopMenu->addMenuItem(tr("About"), "About");
	PopMenu->addMenuItem(tr("Settings"), "Settings");
	PopMenu->addMenuItem(tr("Minimize"), "Minimize");
	PopMenu->addMenuItem(tr("Exit"), "Exit");
	connect(PopMenu.data(), &AnimationMenu::returnAction, this, &MainInterface::operationMenuReturn);
	menuShowing = true;
	PopMenu->exec(QPoint(ToolBar->x(),ToolBarY - PopMenu->getHeight() - 8));
}

void MainInterface::SelectButtonClicked()
{
	ResetToolButtons();
	SelectButton->setCheckState(true);
	SelectButton->setIconState(ToolBarButton::CheckHover);
	nowTool = Select;
	CameraFrame->setFrameDraggable(FrameGraphicsView::FrameSelect);
}

void MainInterface::PenButtonClicked()
{
	if (nowTool != Pen) {
		ResetToolButtons();
		PenButton->setCheckState(true);
		PenButton->setIconState(ToolBarButton::CheckHover);
		nowTool = Pen;
		CameraFrame->setFrameDraggable(FrameGraphicsView::FramePen);
	}
	else {
		PenPanel = QSharedPointer<PenOptionsWidget>(new PenOptionsWidget(this, CameraFrame->pen.width(), CameraFrame->pen.color()));
		PenPanel->setParent(this);
		auto geometry = ToolBar->geometry();
		auto _y = height() - ToolBarBottomMargin - ToolBarHeight;
		geometry.setY(_y);
		geometry.adjust(135, -PenPanelHeight - 6, 0, 0);
		geometry.setSize(QSize(PenPanelWidth, PenPanelHeight));
		PenPanel->setGeometry(geometry);
		connect(PenPanel.data(), &PenOptionsWidget::setColorSignal, CameraFrame, &FrameGraphicsView::setPenColor);
		connect(PenPanel.data(), &PenOptionsWidget::setPenWidthSignal, CameraFrame, &FrameGraphicsView::setPenWidth);
		PenPanel->appear();
		PenPanel->show();
		PenPanel->raise();
	}
}

void MainInterface::EraserButtonClicked()
{
	if (nowTool != Eraser) {
		ResetToolButtons();
		EraserButton->setCheckState(true);
		EraserButton->setIconState(ToolBarButton::CheckHover);
		nowTool = Eraser;
		CameraFrame->setFrameDraggable(FrameGraphicsView::FrameEraser);
	}
	else {
		EraserPanel = QSharedPointer<EraserOptionsWidget>(new EraserOptionsWidget(this, CameraFrame->eraser.width()));
		EraserPanel->setParent(this);
		auto geometry = ToolBar->geometry();
		auto _y = height() - ToolBarBottomMargin - ToolBarHeight;
		geometry.setY(_y);
		geometry.adjust(185, -156, 0, 0);
		geometry.setSize(QSize(230, 150));
		EraserPanel->setGeometry(geometry);
		connect(EraserPanel.data(), &EraserOptionsWidget::clearSignal, this, &MainInterface::ClearButtonClicked);
		connect(EraserPanel.data(), &EraserOptionsWidget::clearAllSignal, this, &MainInterface::ClearAllButtonClicked);
		connect(EraserPanel.data(), &EraserOptionsWidget::setEraserWidthSignal, CameraFrame, &FrameGraphicsView::setEraserWidth);
		EraserPanel->appear();
		EraserPanel->show();
		EraserPanel->raise();
	}
}

void MainInterface::UndoButtonClicked()
{
	ToolBar->floatBar(height() - ToolBarBottomMargin - ToolBarHeight);
	captureIndexWidget->floatBar();
	CameraFrame->undoProcess();
}

void MainInterface::CaptureButtonClicked()
{
	ToolBar->floatBar(height() - ToolBarBottomMargin - ToolBarHeight);
	captureIndexWidget->floatBar();

	if (not captureShowing) {
		auto scene = new QGraphicsScene();
		auto item = new QGraphicsPixmapItem();
		auto picture = realtimeCapture;
		if (not realtimeCapture.isNull()) {
			picture = picture.scaled(width(), height(), Qt::KeepAspectRatio);
			item->setPixmap(picture);
			item->setPos(width() / 2 + (width() - picture.width()) / 2,
				height() / 2 + (height() - picture.height()) / 2);
			scene->addItem(item);
			scene->setSceneRect(0, 0, width() * 2, height() * 2);
			captureScenes.append(scene);
			CapturesWidget->BelowListWidget->addPixmap(picture);

			if (SideBarShowing == 0 and configObject->getConfig("clickToScreenshotWidget").toBool()) {
				CapturePopNotify = QSharedPointer<CapturedNotify>(new CapturedNotify(picture, this, CapturesWidget->BelowListWidget->count() - 1));
				CapturePopNotify->setGeometry(width() + 1, height() - CapturePopNotify->sizeHint().height() - 5, CapturedNotifyWidth, CapturePopNotify->sizeHint().height());
				connect(CapturePopNotify.data(), &CapturedNotify::switchToPixmap, this, &MainInterface::captureNotifyClicked);
				CapturePopNotify->installEventFilter(this);
				CapturePopNotify->show();
				CapturePopNotify->raise();
				CapturePopNotify->appear();
			}
		}
	}
	else {
		changeAboveSceneIndex(0);
		CapturesWidget->AboveListWidget->setSelectingIndex(0);
	}
}

void MainInterface::RightWidgetButtonClicked()
{
	if (not CapturePopNotify.isNull()) {
		CapturePopNotify->tryDisappear();
	}
	switchRightSideBar();
}

void MainInterface::ClearButtonClicked()
{
	CameraFrame->clearFrame();
	ResetToolButtons();
	PenButton->setCheckState(true);
	PenButton->setIconState(ToolBarButton::CheckHover);
	nowTool = Pen;
	CameraFrame->setFrameDraggable(FrameGraphicsView::FramePen);
}

void MainInterface::ClearAllButtonClicked()
{
	CameraFrame->clearAllFrame();
	ResetToolButtons();
	PenButton->setCheckState(true);
	PenButton->setIconState(ToolBarButton::CheckHover);
	nowTool = Pen;
	CameraFrame->setFrameDraggable(FrameGraphicsView::FramePen);
}

void MainInterface::resizeEvent(QResizeEvent* event) {
	auto windowSize = size();
	//videoItem->setSize(windowSize);
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
	else if (obj == CapturePopNotify.data()) {
		if (event->type() == QEvent::FocusOut) {
			CapturePopNotify->noFocused();
		}
	}
	return QWidget::eventFilter(obj, event);
}
