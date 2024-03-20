#include "MainInterface.h"

MainInterface::MainInterface(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainInterfaceClass())
{
	ui->setupUi(this);

	cameraManager = new CameraManager(this);
	videoItem = new QGraphicsVideoItem();
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
	RightWidgetButton->setProperty("toLeft", true);
	RightWidgetButton->setStyleSheet("QToolButton[toLeft=true]{"
		"image: url(:/MainInterface/LeftArrowIcon1);"
		"background-color: rgba(190, 190, 190, 140);"
		"border-radius: 10px;"
		"}"
		"QToolButton:hover[toLeft=true]{"
		"image: url(:/MainInterface/LeftArrowIcon2);"
		"background-color: rgba(230, 230, 230, 140);"
		"}"
		"QToolButton:pressed[toLeft=true]{"
		"image: url(:/MainInterface/LeftArrowIcon4);"
		"background-color: rgba(230, 230, 230, 160);"
		"}"
		"QToolButton[toLeft=false]{"
		"image: url(:/MainInterface/LeftRightIcon1);"
		"background-color: rgba(190, 190, 190, 140);"
		"border-radius: 10px;"
		"}"
		"QToolButton:hover[toLeft=false]{"
		"image: url(:/MainInterface/LeftRightIcon2);"
		"background-color: rgba(230, 230, 230, 140);"
		"}"
		"QToolButton:pressed[toLeft=false]{"
		"image: url(:/MainInterface/LeftRightIcon4);"
		"background-color: rgba(230, 230, 230, 160);"
		"}"
	);
	RightWidgetButton->setGeometry(QRect(width() - 36, height() / 2 - 45, 32, 90));

	ui->mainToolBar->setParent(this);
	removeToolBar(ui->mainToolBar);
	ui->mainToolBar->setStyleSheet("QToolBar#mainToolBar {"
		"background-color: rgba(255, 255, 255, 180);"
		"border: 2px solid rgba(255, 255, 255, 200);"
		"border-radius: 8px;"
		"padding: 0px 5px 0px;"
		"}");
	ui->mainToolBar->setContentsMargins(0, 0, 0, 0);
	ui->mainToolBar->setFloatable(true);
	ui->mainToolBar->setMovable(false);
	ui->mainToolBar->addWidget(MenuButton);
	ui->mainToolBar->addSeparator();
	ui->mainToolBar->addWidget(SelectButton);
	ui->mainToolBar->addWidget(PenButton);
	ui->mainToolBar->addWidget(EraserButton);
	ui->mainToolBar->addWidget(UndoButton);
	ui->mainToolBar->addSeparator();
	ui->mainToolBar->addWidget(CaptureButton);
	ui->mainToolBar->show();

	updateTimer = new QTimer();
	updateTimer->setInterval(10);
	connect(updateTimer, &QTimer::timeout, this, &MainInterface::updateCapture);
	updateTimer->start();
	initailizeCamera();

	mainScene = new QGraphicsScene();
	mainScene->addItem(videoItem);
	ui->CameraFrame->setScene(mainScene);
	ui->CameraFrame->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->CameraFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->statusBar->hide();

	toolBarPosAnimation = QSharedPointer<QPropertyAnimation>(new QPropertyAnimation(ui->mainToolBar, "geometry"));
	toolBarPosAnimation->setStartValue(QRect(width() / 2, height() - 84, 10, 76));
	toolBarPosAnimation->setEndValue(QRect(width() / 2 - 187, height() - 84, 374, 76));
	toolBarPosAnimation->setEasingCurve(QEasingCurve::OutQuart);
	toolBarPosAnimation->setDuration(670);
	toolBarPosAnimation->start();

	setCentralWidget(ui->CameraFrame);
	ui->CameraFrame->resetViewPort(width() * 2, height() * 2);
	ui->mainToolBar->raise();
	RightWidgetButton->raise();
}

MainInterface::~MainInterface()
{
	delete videoItem;
	delete captureSession;
	delete cameraManager;
	delete mainScene;
	delete updateTimer;
	delete MenuButton;
	delete SelectButton;
	delete PenButton;
	delete EraserButton;
	delete UndoButton;
	delete CaptureButton;
	delete RightWidgetButton;
	delete ui;
}

void MainInterface::initailizeCamera()
{
	if (cameraManager->checkCameraAvailablity()) {
		cameraManager->setCamera(cameraManager->getAvailableCameras()[0]);
	}
	if (cameraManager->isAvailable()) {
		captureSession->setCamera(cameraManager->camera());
		int maxIndex = 0;
		int maxSize = 0;
		int i = 0;
		const QList<QSize> resolutions = cameraManager->camera()->cameraDevice().photoResolutions();
		for (const QSize size : resolutions) {
			int area = size.height() * size.width();
			if (area > maxSize) {
				maxSize = area;
				maxIndex = i;
			}
			i++;
		}
		videoItem->setSize(resolutions[maxIndex]);
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

void MainInterface::updateCapture()
{
	if (cameraManager->isAvailable())
		captureSession->imageCapture();
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
	PopMenu->exec(QPoint(ui->mainToolBar->x(), ui->mainToolBar->y() - PopMenu->getHeight() - 10));
}

void MainInterface::SelectButtonClicked()
{
	ResetToolButtons();
	SelectButton->setCheckState(true);
	SelectButton->setIconState(ToolBarButton::CheckHover);
	nowTool = Select;
	ui->CameraFrame->setFrameDraggable(true);
}

void MainInterface::PenButtonClicked()
{
	ResetToolButtons();
	PenButton->setCheckState(true);
	PenButton->setIconState(ToolBarButton::CheckHover);
	nowTool = Pen;
	ui->CameraFrame->setFrameDraggable(false);
}

void MainInterface::EraserButtonClicked()
{
	ResetToolButtons();
	EraserButton->setCheckState(true);
	EraserButton->setIconState(ToolBarButton::CheckHover);
	nowTool = Eraser;
	ui->CameraFrame->setFrameDraggable(false);
}

void MainInterface::UndoButtonClicked()
{
}

void MainInterface::CaptureButtonClicked()
{
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
	return QWidget::eventFilter(obj, event);
}
