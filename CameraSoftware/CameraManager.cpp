#include "CameraManager.h"

CameraManager::CameraManager(QObject *parent)
	: QObject(parent)
{
	
}

CameraManager::~CameraManager()
{
	delete inputCamera;
}

bool CameraManager::checkCameraAvailablity()
{
	if (QMediaDevices::videoInputs().count() > 0) {
		return true;
	}
	return false;
}

const QList<QCameraDevice> CameraManager::getAvailableCameras()
{
	return QMediaDevices::videoInputs();
}

void CameraManager::setCamera(QCameraDevice device)
{
	inputCamera = new QCamera(device);
	inputCamera->setActive(true);
	outputAvailable = true;
}

bool CameraManager::setCamera(QString cameraName)
{
	QList<QCameraDevice> cameras = getAvailableCameras();
	for (const QCameraDevice& device : cameras) {
		if (device.description() == cameraName) {
			setCamera(device);
			return true;
		}
	}
	return false;
}

