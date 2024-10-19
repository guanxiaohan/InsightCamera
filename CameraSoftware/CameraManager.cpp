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
	//QMutex mutex;
	//mutex.lock();
	//if (device.description().startsWith("OBS")) {
	//	//mutex.unlock();
	//	return;
	//}
	if (inputCamera) {
		delete inputCamera;
		/*inputCamera = nullptr;*/
	}
	inputCamera = new QCamera(device);
	inputCamera->setActive(true);
	outputAvailable = true;
	//mutex.unlock();
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

bool CameraManager::isAvailable() const
{
	return outputAvailable;
}

QSize CameraManager::getMaxResolution()
{
	int maxIndex = 0;
	int maxSize = 0;
	int i = 0;
	if (camera() == nullptr)
		return QSize(0, 0);
	auto cameraDevice = camera()->cameraDevice();
	if (cameraDevice.isNull())
		return QSize(0, 0);
	const QList<QSize> resolutions = cameraDevice.photoResolutions();
	for (const QSize size : resolutions) {
		int area = size.height() * size.width();
		if (area > maxSize) {
			maxSize = area;
			maxIndex = i;
		}
		i++;
	}
	return resolutions[maxIndex];
}
