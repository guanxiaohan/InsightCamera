#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraDevice>
#include <QtMultimedia/QMediaDevices>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QMediaCaptureSession>
#include <QResizeEvent>
#include "ui_MainInterface.h"
#include "CameraManager.h"
#include "FrameGraphicsView.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainInterfaceClass; };
QT_END_NAMESPACE

class MainInterface : public QMainWindow
{
    Q_OBJECT

public:
    MainInterface(QWidget *parent = nullptr);
    ~MainInterface();
    void initailizeCamera();

private:
    Ui::MainInterfaceClass *ui;
    CameraManager *cameraManager;
    QGraphicsScene *mainScene;
    QGraphicsVideoItem* videoItem;
    QMediaCaptureSession* captureSession;
    QTimer* updateTimer;

    void updateCapture();

protected:
    void resizeEvent(QResizeEvent *event);

};
