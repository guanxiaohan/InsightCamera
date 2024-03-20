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
#include <QtCore/QString>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QEasingCurve>
#include <QtCore/QTimer>
#include <QtCore/QSharedPointer>
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
    void operationMenuReturn(int);
    enum Tools{Null, Select, Pen, Eraser};

private:
    Ui::MainInterfaceClass *ui;
    CameraManager *cameraManager;
    QGraphicsScene *mainScene;
    QGraphicsVideoItem* videoItem;
    QMediaCaptureSession* captureSession;
    QTimer* updateTimer;
    Tools nowTool = Select;
    bool menuShowing = false;
    QList<QPixmap*> capturesCache;

    ToolBarButton* MenuButton;
    ToolBarButton* SelectButton;
    ToolBarButton* PenButton;
    ToolBarButton* EraserButton;
    ToolBarButton* UndoButton;
    ToolBarButton* CaptureButton;
    QToolButton* RightWidgetButton;
    QSharedPointer<AnimationMenu> PopMenu;

    QSharedPointer<QPropertyAnimation> toolBarPosAnimation;

    void updateCapture();
    void ResetToolButtons();
    void MenuButtonClicked();
    void SelectButtonClicked();
    void PenButtonClicked();
    void EraserButtonClicked();
    void UndoButtonClicked();
    void CaptureButtonClicked();

protected:
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject* obj, QEvent* event);

signals:
    void ToolBarButtonHover(ToolBarButton* button);
    void ToolBarButtonLeave(ToolBarButton* button);
};
