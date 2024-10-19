#pragma once

#include <QDialog>
#include <QObject>
#include <QLabel>
#include <QComboBox>
#include <QColor>
#include <QCheckBox>
#include <QPushButton>
#include <QSlider>
#include <QCloseEvent>

#include "ui_ConfigDialog.h"
#include "Configuration.h"
#include "CameraManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ConfigDialogUi; };
QT_END_NAMESPACE

class ConfigDialog : public QDialog
{
	Q_OBJECT

public:
	ConfigDialog(Configuration* configObj, CameraManager* cameraObj, QWidget* parent = nullptr);
	~ConfigDialog();

	void initailizeUi();

private:
	Configuration* configObject;
	CameraManager* cameraObject;
	Ui::ConfigDialogUi *ui;

	void initCameraDeviceComboBox();
	void initResolutionLabel();
	void initExposureComponsation();

protected:
	void closeEvent(QCloseEvent* event) override;

signals:
	void switchCamera(int index); // -1 for None, -2 for Error
	void changeRefreshRate(int rate);
	void changeISO(int iso);
	void switchCurrentFrameShowing(bool shown);
};
