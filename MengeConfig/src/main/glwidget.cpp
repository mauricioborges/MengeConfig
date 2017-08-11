
#include "glwidget.hpp"
#include "QtContext.h"

#include <QtWidgets\qdialog.h>
#include <QtWidgets\qdialogButtonBox.h>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/qgridlayout.h>
#include <QtGui/QIntValidator>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtGui/QMouseEvent>
#include <QtGui/QOpenGLShaderProgram>
#include <QtCore/QCoreApplication>

#include <math.h>
#include <gl/GL.h>
#include "AppLogger.hpp"
#include "ContextManager.hpp"
#include "GridNode.h"

#include <MengeVis/SceneGraph/GLCamera.h>
#include <MengeVis/SceneGraph/GLContextManager.h>
#include <MengeVis/SceneGraph/GLScene.h>
#include <MengeVis/SceneGraph/GLLight.h>
#include <MengeVis/SceneGraph/shapes.h>
#include <MengeVis/Viewer/ViewConfig.h>

#include <iostream>
#include <sstream>

// temporary
#include "ObstacleContext.hpp"

using Menge::Math::Vector2;
using Menge::Math::Vector3;
using MengeVis::GLContextManager;
using MengeVis::SceneGraph::ContextResult;
using MengeVis::SceneGraph::GLCamera;
using MengeVis::SceneGraph::GLLight;
using MengeVis::SceneGraph::GLScene;
using MengeVis::Viewer::ViewConfig;

///////////////////////////////////////////////////////////////////////////
//				IMPLEMENTATION FOR ReferenceGridProperties
///////////////////////////////////////////////////////////////////////////

/*!
 *	@brief		The dialog for editing the properties of the reference grid.
 */
class RefGridPropDialog : public QDialog {
public:
	RefGridPropDialog(const ReferenceGrid * grid, QWidget * parent=0x0) : QDialog(parent) {
		setModal(false);
		setWindowTitle(tr("Edit Reference Grid Properties"));
		QGridLayout * layout = new QGridLayout();

		Vector2 o = grid->getOrigin();
		_xO = makeNumEditor(0, "X Origin", layout, new QDoubleValidator(), QString::number(o.x()));
		_yO = makeNumEditor(1, "Y Origin", layout, new QDoubleValidator(), QString::number(o.y()));
		Vector2 size = grid->getSize();
		_w = makeNumEditor(2, "Width", layout, new QDoubleValidator(), QString::number(size.x()));
		_h = makeNumEditor(3, "Height", layout, new QDoubleValidator(), QString::number(size.y()));
		_majorDist = makeNumEditor(4, "Major Distance", layout, new QDoubleValidator(), QString::number(grid->getMajorDist()));
		_minorCount = makeNumEditor(5, "Number of Minor Lines", layout, new QIntValidator(0, 1000), QString::number(grid->getMinorCount()));

		QDialogButtonBox * bBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
		layout->addWidget(bBox, 6, 0, 1, 2);

		connect(bBox, &QDialogButtonBox::accepted, this, &RefGridPropDialog::accept);
		connect(bBox, &QDialogButtonBox::rejected, this, &RefGridPropDialog::reject);

		setLayout(layout);
	}

	friend class GLWidget;
protected:
	/*!
	 *	@brief		Utility function to add a number editor into the dialog.
	 *
	 *	@param		row			The row on the grid layout to entry the property.
	 *	@param		name		The name of the property.
	 *	@param		layout		The layout to insert the property.
	 *	@param		valid		The validator to use.
	 *	@param		initValue	The initial value.
	 */
	QLineEdit * makeNumEditor(int row, const std::string & name, QGridLayout * layout,  QValidator * valid, QString initValue) {
		layout->addWidget(new QLabel(tr(name.c_str())), row, 0, Qt::AlignRight);
		QLineEdit * edit = new QLineEdit();
		edit->setValidator(valid);
		edit->setText(initValue);
		layout->addWidget(edit, row, 1);
		return edit;
	}

	/*!
	 *	@brief		The editor for the x-origin value.
	 */
	QLineEdit * _xO;
	
	/*!
	*	@brief		The editor for the y-origin value.
	*/
	QLineEdit * _yO;
	
	/*!
	*	@brief		The editor for the width value.
	*/
	QLineEdit * _w;
	
	/*!
	*	@brief		The editor for the height value.
	*/
	QLineEdit * _h;
	
	/*!
	*	@brief		The editor for the major line distance value.
	*/
	QLineEdit * _majorDist;
	
	/*!
	*	@brief		The editor for the minor line count value.
	*/
	QLineEdit * _minorCount;
};

///////////////////////////////////////////////////////////////////////////
//				IMPLEMENTATION FOR GLWidget
///////////////////////////////////////////////////////////////////////////

GLWidget::GLWidget(QWidget *parent)
	: QOpenGLWidget(parent),
	_scene(0x0), _context(0x0), _cameras(), _currCam(0), _downPos(), _lights(), _drawWorldAxis(false),
  _activeGrid(true), _hSnap(false), _vSnap(false), _isTopView(true)
{
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
	// TODO: Handle cameras in some other way
	GLCamera camera;
	camera.setPosition(0.f, 0.01f, 10.f);
	camera.setTarget(0.f, 0.f, 0.f);
	camera.setFarPlane(100.f);
	camera.setNearPlane(0.1f);
	camera.setFOV(45.f);
	camera.setPersp();
  camera.viewZAxis( false /* down positive */ );
	_cameras.push_back(camera);

	_scene = new GLScene();
	ContextManager * mgr = ContextManager::instance();
	connect(mgr, &ContextManager::activated, this, &GLWidget::activated);
	connect(mgr, &ContextManager::deactivated, this, &GLWidget::deactivated);

	_grid = new GridNode();
  _grid->setOrigin( -50.f, -50.f );
	_grid->setSize(100.f, 100.f);
	_grid->setMajorDist(5.f);
	_grid->setMinorCount(4);
}

///////////////////////////////////////////////////////////////////////////

GLWidget::~GLWidget() {
	cleanup();
}

///////////////////////////////////////////////////////////////////////////

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

///////////////////////////////////////////////////////////////////////////

QSize GLWidget::sizeHint() const {
    return QSize(400, 400);
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::setScene(GLScene * scene) {
	if (_scene && scene != _scene) {
		delete _scene;
	}
	_scene = scene;
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::setView( const ViewConfig& view_config ) {
  _cameras.clear();
  view_config.setCameras( _cameras );
  auto w = width();
  auto h = height();
  for ( size_t i = 0; i < _cameras.size(); ++i ) {
    _cameras[ i ].setViewport( w, h );
  }
  emit setViewPerspective( _cameras[ 0 ].getProjection() == GLCamera::PERSP );
  _lights.clear();
  view_config.setLights( _lights );
  makeCurrent();
  initLighting();
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

void GLWidget::set_camera( size_t camera_index ) {
  if ( _currCam != camera_index ) {
    bool reproject = 
      _cameras[ _currCam ].getProjection() != _cameras[ camera_index ].getProjection();
    _currCam = camera_index;
    // Dirty the projection matrix.
    if ( reproject ) _cameras[ _currCam ].setActive();
    emit setViewPerspective( _cameras[ _currCam ].getProjection() == GLCamera::PERSP );
  }
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::cleanup() {
    makeCurrent();
	// TODO: Notify the scene that the window is being destroyed.
    doneCurrent();
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::activated(size_t id) {
	ContextManager * mgr = ContextManager::instance();
	QtContext * ctx = mgr->getContext(id);
	// TODO: Test that the context in question applies to the viewer.
	_context = ctx;
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::deactivated(size_t id) {
	ContextManager * mgr = ContextManager::instance();
	QtContext * ctx = mgr->getContext(id);
	if (_context == ctx) _context = 0x0;
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::initializeGL() {
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
	glClearColor(0.91f, 0.91f, 0.9f, 1);
	glClearDepth( 1.f );
	glEnable(GL_DEPTH_TEST);
  initLighting();
  glEnable( GL_LIGHTING );

	glEnable( GL_COLOR_MATERIAL );

  MengeVis::SceneGraph::initShapes();
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	if (_scene) {
		_scene->drawGL(_cameras[_currCam], _lights, width(), height());
	}
  _grid->drawGL( false );
	// various view decorations
	// world axis
	if (_drawWorldAxis) drawWorldAxis();

	if (_context) {
		_context->drawGL(width(), height());
	}
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::resizeGL(int w, int h) {
	newGLContext();
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::mousePressEvent(QMouseEvent *event) {
	if (_context) {
		ContextResult result = _context->handleMouse(event, this);
		if (result.needsRedraw()) {
			update();
		}
		if (result.isHandled()) return;
	}

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

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
	if (_context) {
		ContextResult result = _context->handleMouse(event, this);
		if (result.needsRedraw()) {
			update();
		}
		if (result.isHandled()) return;
	}
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
	Vector2 worldPos;
	if (getWorldPos(event->pos(), worldPos)) {
		emit currWorldPos(worldPos.x(), worldPos.y());
	}

	if (_context) {
		ContextResult result = _context->handleMouse(event, this);
		if (result.needsRedraw()) {
			update();
		}
		if (result.isHandled()) return;
	}

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
			_isTopView = false;
			emit userRotated();
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
	if (_context) {
		ContextResult result = _context->handleWheel(event, this);
		if (result.needsRedraw()) {
			update();
		}
		if (result.isHandled()) return;
	}

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

void GLWidget::keyPressEvent(QKeyEvent *event) {
	if (_context) {
		ContextResult result = _context->handleKeyboard(event, this);
		if (result.needsRedraw()) {
			update();
		}
		if (result.isHandled()) return;
	}
	QOpenGLWidget::keyPressEvent(event);
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
	if (_context) {
		ContextResult result = _context->handleKeyboard(event, this);
		if (result.needsRedraw()) {
			update();
		}
		if (result.isHandled()) return;
	}
	QOpenGLWidget::keyReleaseEvent(event);
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
	//MengeVis::SceneGraph::TextWriter::Instance()->resize(w, h);
	//MengeVis::SceneGraph::TextWriter::Instance()->newGLContext();
	//initializeGL();
	GLContextManager::newGLContext();
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
	if (state != _drawWorldAxis) {
		_drawWorldAxis = state;
		update();
	}
}


///////////////////////////////////////////////////////////////////////////

void GLWidget::toggleProjection(bool isPerspective) {
	// this assumes that the boolean argument actually represents a change.
	if (isPerspective) {
		_cameras[_currCam].setPersp();
	}
	else {
		_cameras[_currCam].setOrtho();
	}
	update();
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::toggleReferenceGrid(bool isActive) {
	if (_activeGrid != isActive) {
		_grid->setVisible(isActive);
		_activeGrid = isActive;
		update();
	}
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::toggleHorizontalSnap(bool isActive) {
	_hSnap = isActive;
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::toggleVerticalSnap(bool isActive) {
	_vSnap = isActive;
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::editGridProperties() {
	RefGridPropDialog dlg(_grid);
	if (dlg.exec() == QDialog::Accepted) {
		_grid->setOrigin(dlg._xO->text().toFloat(), dlg._yO->text().toFloat());
		_grid->setSize(dlg._w->text().toFloat(), dlg._h->text().toFloat());
		_grid->setMajorDist(dlg._majorDist->text().toFloat());
		_grid->setMinorCount(dlg._minorCount->text().toInt());
		AppLogger::logStream << AppLogger::INFO_MSG << "Modifying reference grid properties:\n";
		AppLogger::logStream << (*_grid) << "\n" << AppLogger::END_MSG;
		update();
	}

}

///////////////////////////////////////////////////////////////////////////

bool GLWidget::getWorldPos(const QPoint & screenPos, Vector2 & worldPos, bool ignoreSnap) {
	// TODO: worldPos should be in R3
	if (_isTopView) {
		// THIS IS A TOTAL HACK
		//	There should be a 4-byte enumeration at the beginning of the camera which reports
		//	if it is perspective or orthographic
		//  Replace this hack with proper calls into the camera when the interface is extended to
		//	report projection type.
		int persp = *(int*)(&_cameras[_currCam]);

		float w = (float)width();
		float h = (float)height();
		float u = screenPos.x() / w;
		float v = (h - screenPos.y()) / h;
		float wHalfWidth, wHalfHeight;
		
		Vector3 pos = _cameras[_currCam].getPosition();
		if (persp == 0) {	// orthographic
			wHalfWidth = 0.5f / _cameras[_currCam].getOrthoScaleFactor() * _cameras[_currCam].targetDistance();
			wHalfHeight = wHalfWidth * h / w;
		}
		else {		// perspective
			wHalfHeight = pos.z() * tan(_cameras[_currCam].getFOV() * 0.5f / 180.f * 3.141597f);
			wHalfWidth = wHalfHeight * w / h;
		}
		float x = pos.x() - wHalfWidth + (u * wHalfWidth * 2.f);
		float y = pos.y() - wHalfHeight + (v * wHalfHeight * 2.f);
		worldPos.set(x, y);

		if (!ignoreSnap ) {
			worldPos = snap(worldPos);
		}
		return true;
	} 
	return false;
}

///////////////////////////////////////////////////////////////////////////

Vector2 GLWidget::snap(const Vector2 & pos) {
	if ( _activeGrid) {
		if (_hSnap && _vSnap) {
			return _grid->snap(pos);
		}
		else if (_hSnap) {
			return _grid->snapHorizontal(pos);
		}
		else if (_vSnap) {
			return _grid->snapVertical(pos);
		}
	}
	return pos;
}

///////////////////////////////////////////////////////////////////////////

float GLWidget::getWorldScale(float len) {
	float length = len;
	if (_isTopView) {
		// THIS IS A TOTAL HACK
		//	There should be a 4-byte enumeration at the beginning of the camera which reports
		//	if it is perspective or orthographic
		//  Replace this hack with proper calls into the camera when the interface is extended to
		//	report projection type.
		int persp = *(int*)(&_cameras[_currCam]);

		float scale = 1.f;
		if (persp == 0) {	// orthographic
			float wWidth = _cameras[_currCam].targetDistance() / _cameras[_currCam].getOrthoScaleFactor();
			scale = wWidth / width();
		}
		else {		// perspective
			Vector3 pos = _cameras[_currCam].getPosition();
			float wHeight = 2.f * pos.z() * tan(_cameras[_currCam].getFOV() * 0.5f / 180.f * 3.141597f);
			scale = wHeight / height();
		}
		length *= scale;
	}
	return length;
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::setViewDirection(int direction){
	_isTopView = false;
	if (direction > 0) {
		switch (direction) {
		case 1:	// +x
			_cameras[_currCam].viewXAxis();
			break;
		case 2: // -x
			_cameras[_currCam].viewXAxis(false);
			break;
		case 3:	// +y
			_cameras[_currCam].viewYAxis();
			break;
		case 4:	// -y
			_cameras[_currCam].viewYAxis(false);
			break;
		case 5: // +z
			_cameras[_currCam].viewZAxis();
			break;
		case 6: // -z
			_cameras[_currCam].viewZAxis(false);
			_isTopView = true;
			break;
		}
		update();
	}
}

///////////////////////////////////////////////////////////////////////////

void GLWidget::initLighting() {
  if ( _lights.size() > 0 ) {
    glEnable( GL_LIGHTING );
    for ( size_t i = 0; i < _lights.size(); ++i ) {
      _lights[ i ].initGL( (int)i, GLLight::CAMERA );
    }
  } else {
    glDisable( GL_LIGHTING );
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