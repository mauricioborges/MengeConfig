
#include "glwidget.hpp"
#include <QtGui/QMouseEvent>
#include <QtGui/QOpenGLShaderProgram>
#include <QtCore/QCoreApplication>
#include <math.h>
#include <gl/GL.h>
#include "GLCamera.h"
#include "GLScene.h"
#include "GLLight.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////
//				IMPLEMENTATION FOR GLWidget
///////////////////////////////////////////////////////////////////////////

GLWidget::GLWidget(QWidget *parent)
	: QOpenGLWidget(parent),
	_scene(0x0), _cameras(), _currCam(0), _downPos(), _lights(), _drawWorldAxis(true)
{
}

///////////////////////////////////////////////////////////////////////////

GLWidget::~GLWidget()
{
    cleanup();
}

///////////////////////////////////////////////////////////////////////////

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

///////////////////////////////////////////////////////////////////////////

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::setScene(Menge::SceneGraph::GLScene * scene) {
	if (_scene && scene != _scene) {
		delete _scene;
	}
	_scene = scene;
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::setCameraFOV(int i, float fov) { 
	_cameras[i].setFOV(fov); 
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::setCameraFarPlane(int i, float dist) {
	_cameras[i].setFarPlane(dist); 
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::cleanup()
{
    makeCurrent();
	// TODO: Notify the scene that the window is being destroyed.
    doneCurrent();
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::initializeGL()
{
	std::cout << "GLWidget::initializeGL\n";
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();

	glEnable(GL_NORMALIZE);
	glShadeModel( GL_SMOOTH );
	glClearColor(0, 0, 0, 1);
	//glClearColor( _bgColor.x(), _bgColor.y(), _bgColor.z(), 0.f );
	glClearDepth( 1.f );
	glEnable(GL_DEPTH_TEST);
	if ( _lights.size() > 0 ) {
		initLighting();
	} else {
		glDisable( GL_LIGHTING );
	}

	glEnable( GL_COLOR_MATERIAL );

}

///////////////////////////////////////////////////////////////////////////

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (_scene) {
		_scene->drawGL(_cameras[_currCam], _lights, width(), height());
	}
	// various view decorations
	// world axis
	if (_drawWorldAxis) drawWorldAxis();

}

///////////////////////////////////////////////////////////////////////////

void GLWidget::resizeGL(int w, int h)
{
	newGLContext();
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	_downPos = event->pos();
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - _downPos.x();
	int dy = event->y() - _downPos.y();

    if (event->buttons() & Qt::LeftButton) {
    } else if (event->buttons() & Qt::RightButton) {
    }
	_downPos = event->pos();
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::newGLContext() {
	int w = width();
	int h = height();
	// TODO: in QT resizing does *not* invalidate the context.
	//	I need to see which of this is actually necessary.
	//	In fact, context manager should be called in initializeGL
	// I need to determine what the text writer needs to do when things resize
	//		do I need to actually call newGLContext on it?
	// TODO: If new gl context is not necessary for a resize, then I should
	//	move the resizing stuff to resize, and the new gl to initializeGL.
	//Menge::SceneGraph::TextWriter::Instance()->resize(w, h);
	//Menge::SceneGraph::TextWriter::Instance()->newGLContext();
	//initializeGL();
	//Menge::GLContextManager::newGLContext();
	if (_scene) {
		_scene->newGLContext();
	}
	// Re-initialize the cameras
	for (size_t i = 0; i < _cameras.size(); ++i) {
		_cameras[i].setViewport(w, h);
	}
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::setDrawAxis(bool state) {
	_drawWorldAxis = state;
	update();
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::initLighting() {
	glEnable(GL_LIGHTING);
	for (size_t i = 0; i < _lights.size(); ++i) {
		_lights[i].initGL((int)i, Menge::SceneGraph::GLLight::CAMERA);
	}
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::drawWorldAxis() {
	// NOTE: This doesn't GUARANTEE that it's being drawn in the correct space
	//		It assumes that the modelview matrix is the camera matrix
	glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glLineWidth(2.0f);
	const float AXIS_SCALE = 4.f;
	glBegin(GL_LINES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(AXIS_SCALE, 0.f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, AXIS_SCALE, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, AXIS_SCALE);
	glEnd();
	glPopAttrib();
}