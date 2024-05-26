#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QMessageBox>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraDevice>
#include <QtMultimedia/QMediaDevices>
#include <QtMultimedia/QImageCapture>
#include <QtMultimedia/QVideoSink>
#include <QtCore/QString>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QEasingCurve>
#include <QtCore/QTimer>
#include <QtCore/QSharedPointer>
#include <QtCore/QScopedPointer>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QGraphicsVideoItem>
#include <QMediaCaptureSession>
#include <QResizeEvent>

#include "ui_MainInterface.h"
#include "CameraManager.h"
#include "FrameGraphicsView.h"
#include "ToolBarButton.h"
#include "AnimationMenu.h"
#include "UnderToolBar.h"
#include "CapturesList.h"
#include "CapturedNotify.h"
#include "BottomVideoGraphicsItem.h"
#include "EraserOptionsWidget.h"
#include "PenOptionsWidget.h"
#include "AnimationSlider.h"

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
    void frameAddSlider(int maxValue, int value, QPoint position);
    void operationMenuReturn(int);
    void changeAboveSceneIndex(int);
    void changeBelowSceneIndex(int);
    enum Tools{Null, Select, Pen, Eraser};

private:
    Ui::MainInterfaceClass *ui;
    CameraManager *cameraManager;
    QGraphicsScene *mainScene;
    BottomVideoGraphicsItem* videoItem;
    QMediaCaptureSession* captureSession;
    QImageCapture imageCapture;
    QTimer* updateTimer;
    QTimer* watchdogTimer;
    Tools nowTool = Select;
    bool menuShowing = false;
    bool captureShowing = false;
    int SideBarShowing = 0;
    QImage realtimeCapture;
    QList<QGraphicsScene*> captureScenes;

    ToolBarButton* MenuButton;
    ToolBarButton* SelectButton;
    ToolBarButton* PenButton;
    ToolBarButton* EraserButton;
    ToolBarButton* UndoButton;
    ToolBarButton* CaptureButton;
    QToolButton* RightWidgetButton;
    UnderToolBar* ToolBar;
    CapturesList* CapturesWidget;
    QSharedPointer<EraserOptionsWidget> EraserPanel;
    QSharedPointer<PenOptionsWidget> PenPanel;
    QSharedPointer<AnimationMenu> PopMenu;
    QSharedPointer<CapturedNotify> CapturePopNotify;
    QSharedPointer<AnimationSlider> FrameScaleSlider;

    QSharedPointer<QPropertyAnimation> RightButtonAnimation;
    QSharedPointer<QPropertyAnimation> CapturesWidgetAnimation;

    void switchRightSideBar();
    void updateCapture();
    void captureReturned(int, QImage);
    void captureNotifyClicked(int);
    void ResetToolButtons();
    void MenuButtonClicked();
    void SelectButtonClicked();
    void PenButtonClicked();
    void EraserButtonClicked();
    void UndoButtonClicked();
    void CaptureButtonClicked();
    void RightWidgetButtonClicked();
    void ClearButtonClicked();
    void ClearAllButtonClicked();

protected:
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject* obj, QEvent* event);

signals:
    void ToolBarButtonHover(ToolBarButton* button);
    void ToolBarButtonLeave(ToolBarButton* button);
};
