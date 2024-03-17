#pragma once

#include <QObject>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraDevice>
#include <QtMultimedia/QMediaDevices>
#include <QtCore/QString>
#include <QtCore/QList>

class CameraManager  : public QObject
{
	Q_OBJECT

public:
	CameraManager(QObject *parent = nullptr);
	~CameraManager();
	bool checkCameraAvailablity();
	const QList<QCameraDevice> getAvailableCameras();
	void setCamera(QCameraDevice device);
	bool setCamera(QString cameraName);
	bool isAvailable() const { return outputAvailable; };
	QCamera* camera() { return inputCamera; };

private:
	bool outputAvailable = false;
	QCamera* inputCamera = nullptr;
	
};
