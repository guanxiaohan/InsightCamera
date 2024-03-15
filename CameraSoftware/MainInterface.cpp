#include "MainInterface.h"

MainInterface::MainInterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainInterfaceClass())
{
    ui->setupUi(this);

    cameraManager = new CameraManager(this);
    videoItem = new QGraphicsVideoItem();
    captureSession = new QMediaCaptureSession();
    updateTimer = new QTimer();
    updateTimer->setInterval(16);
    updateTimer->start();
    initailizeCamera();
    connect(updateTimer, &QTimer::timeout, this, &MainInterface::updateCapture);

    mainScene = new QGraphicsScene();
    mainScene->addItem(videoItem);
    ui->CameraFrame->setScene(mainScene);
    ui->CameraFrame->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->CameraFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mainToolBar->hide();
    ui->statusBar->hide();

    setStyleSheet(QString("QMainWindow{background-color:#000000;}"));
    setWindowState(Qt::WindowFullScreen);
    setCentralWidget(ui->CameraFrame);
}

MainInterface::~MainInterface()
{
    delete cameraManager;
    delete mainScene;
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

void MainInterface::updateCapture()
{
    if (cameraManager->isAvailable())
        captureSession->imageCapture();
}

void MainInterface::resizeEvent(QResizeEvent *event) {
    auto windowSize = size();
    //ui->CameraFrame->resize(windowSize.width()*2, windowSize.height()*2);
    videoItem->setSize(windowSize);
}

