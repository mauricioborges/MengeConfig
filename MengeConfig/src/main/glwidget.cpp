
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
	// TODO: Handle cameras in some other way
	Menge::SceneGraph::GLCamera camera;
	camera.setPosition(0.f, 0.f, 10.f);
	camera.setTarget(0.f, 0.f, 0.f);
	camera.setFarPlane(100.f);
	camera.setNearPlane(0.1f);
	camera.setFOV(45.f);
	camera.setPersp();
	_cameras.push_back(camera);

	_scene = new Menge::SceneGraph::GLScene();
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
	Qt::KeyboardModifiers mods = event->modifiers();
	bool hasCtrl = (mods & Qt::CTRL) > 0;
	bool hasAlt = (mods & Qt::ALT) > 0;
	bool hasShift = (mods & Qt::SHIFT) > 0;

	Qt::MouseButton btn = event->button();
	if (btn == Qt::LeftButton) {
		_downPos = event->pos();
		if (!(hasCtrl || hasAlt || hasShift) && _scene != 0x0) {
			std::cout << "Selection is not implemented yet!\n";
		}
	} 
	
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	Qt::KeyboardModifiers mods = event->modifiers();
	bool hasCtrl = (mods & Qt::CTRL) > 0;
	bool hasAlt = (mods & Qt::ALT) > 0;
	bool hasShift = (mods & Qt::SHIFT) > 0;

	bool rotate = hasCtrl && !(hasAlt || hasShift);
	bool pan = !hasAlt && hasCtrl && hasShift;
	bool zoom = hasShift && !(hasAlt || hasCtrl);

	Qt::MouseButtons btn = event->buttons();
	if (btn == Qt::LeftButton) {
		QPoint delta = event->pos() - _downPos;
		bool cameraMoved = false;
		if (rotate) {
			_cameras[_currCam].orbitHorizontalAxis(delta.y() * 0.0075f);
			_cameras[_currCam].orbitVerticalAxis(-delta.x() * 0.0075f);
			cameraMoved = true;
		}
		else if (pan) {
			_cameras[_currCam].truck(-delta.x() * 0.0025f);
			_cameras[_currCam].crane(delta.y() * 0.0025f);
			cameraMoved = true;
		}
		else if (zoom) {
			const float scale = 1.f / 5.0f;
			_cameras[_currCam].zoom(-delta.y() * scale);
			cameraMoved = true;
		}
		_downPos = event->pos();
		if (cameraMoved) update();
	}
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::wheelEvent(QWheelEvent *event) {
	Qt::KeyboardModifiers mods = event->modifiers();
	bool hasCtrl = (mods & Qt::CTRL) > 0;
	bool hasAlt = (mods & Qt::ALT) > 0;
	bool hasShift = (mods & Qt::SHIFT) > 0;
	float amount = event->delta() / 10.f;
	if (hasCtrl) amount *= 2;
	if (hasAlt) amount *= 2;
	if (hasShift) amount *= 2;
	_cameras[_currCam].zoom(amount);
	update();
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