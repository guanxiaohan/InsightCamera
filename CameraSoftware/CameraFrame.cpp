#include "CameraFrame.h"

CameraFrame::CameraFrame(QWidget *parent)
	: QOpenGLWidget(parent)
{

}

CameraFrame::~CameraFrame()
{

}

void CameraFrame::initializeGL()
{
    initializeOpenGLFunctions();
}

void CameraFrame::resizeGL(int w, int h)
{

}

void CameraFrame::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}