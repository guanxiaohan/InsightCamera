#pragma once

#include <QObject>
#include 

class CameraFrame  : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	Q_OBJECT

public:
	explicit CameraFrame(QWidget *parent = nullptr);
	~CameraFrame();

protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();

signals:

};
