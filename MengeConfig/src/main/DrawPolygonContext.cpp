#include "DrawPolygonContext.h"

#include "AppLogger.hpp"
#include "GLPolygon.h"
#include "glwidget.hpp"
#include "MCException.h"

#include <QtCore/qpoint.h>

#include <Math/vector.h>
using namespace Menge::Math;

#include <cassert>
#include <gl/GL.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of DrawPolygonContext
/////////////////////////////////////////////////////////////////////////////////////////////

DrawPolygonContext::DrawPolygonContext() : QtContext(), _state(WAITING), _polygon(0x0), _dragging(false), _newPolyCB(0x0) {

}

/////////////////////////////////////////////////////////////////////////////////////////////

DrawPolygonContext::~DrawPolygonContext() {
	if (_polygon) {
		delete _polygon;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

Menge::SceneGraph::ContextResult DrawPolygonContext::handleMouse(QMouseEvent * evt, GLWidget * view) {
	Menge::SceneGraph::ContextResult result = QtContext::handleMouse(evt, view);

	if (!result.isHandled()) {
		Qt::KeyboardModifiers mods = evt->modifiers();
		if (mods == Qt::NoModifier) {
			if (evt->type() == QEvent::MouseButtonPress) {
				if (evt->button() == Qt::LeftButton) {
					if (_state == WAITING) {
						_polygon = new GLPolygon();
						_state = DRAWING;
					}
					Vector2 v;
					view->getWorldPos(evt->pos(), v);
					_polygon->addVertex(Vector3(v.x(), v.y(), 0.f));
					// TODO: Log the addition of the point
					_dragging = true;
					result.set(true, true);
				}
				else if (_state == DRAWING && evt->button() == Qt::RightButton) {
					// Log the creation/destrution of active polygon
					if (_polygon->_vertices.size() < 3) {
						deleteActive();
					}
					if (_newPolyCB == 0x0 || !_newPolyCB->newPolygon(_polygon)) {
						deleteActive();
					}
					_polygon = 0x0;
					_state = WAITING;
					result.set(true, true);
				}
			}
			else if (evt->type() == QEvent::MouseButtonRelease) {
				if (_dragging && evt->button() == Qt::LeftButton) {
					_dragging = false;
				}
			}
			else if (evt->type() == QEvent::MouseMove) {
				if (_dragging) {
					Vector2 v;
					view->getWorldPos(evt->pos(), v);
					_polygon->_vertices[_polygon->_vertices.size() - 1] = Vector3(v.x(), v.y(), 0.f);
					result.set(true, true);
				}
			}
		}
	}

	if (!result.isHandled()) {
		// The key event is accepted by default -- ignore must be called explicitly to pass it up.
		//	http://doc.qt.io/qt-5/qkeyevent.html#details
		evt->ignore();
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////

Menge::SceneGraph::ContextResult DrawPolygonContext::handleKeyboard(QKeyEvent * evt, GLWidget * view) {
	Menge::SceneGraph::ContextResult result = QtContext::handleKeyboard(evt, view);

	if (!result.isHandled()) {
		Qt::KeyboardModifiers mods = evt->modifiers();
		if (evt->type() == QEvent::KeyPress) {
			if (mods & Qt::ControlModifier && evt->key() == Qt::Key_Z) {
				result.set(true, popVertex());
			}
			else if (mods == Qt::NoModifier && evt->key() == Qt::Key_Delete) {
				result.set(true, deleteActive());
			}
			else if (mods == Qt::NoModifier && evt->key() == Qt::Key_F) {
				result.set(true, reverseWinding());
			}
		}
	}
	if (!result.isHandled()) {
		// The key event is accepted by default -- ignore must be called explicitly to pass it up.
		//	http://doc.qt.io/qt-5/qkeyevent.html#details
		evt->ignore();
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool DrawPolygonContext::reverseWinding() {
	bool changed = false;
	if (_polygon && _polygon->_vertices.size() > 1 ) {
		_polygon->reverseWinding();
		changed = true;
	}
	return changed;
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool DrawPolygonContext::popVertex() {
	bool changed = _polygon != 0x0;
	if (_polygon) {
		size_t count = _polygon->popVertex();
		if (count == 0) {
			delete _polygon;
			_polygon = 0x0;
			_state = WAITING;
		} 
	}
	return changed;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void DrawPolygonContext::activate() {
	assert( _polygon == 0x0 && "Activating the context that already has a polygon");
	_state = WAITING;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void DrawPolygonContext::deactivate() {
	deleteActive();
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool DrawPolygonContext::deleteActive() {
	if (_polygon) {
		delete _polygon;
		_polygon = 0x0;
		_state = WAITING;
		_dragging = false;
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void DrawPolygonContext::registerNewPolyCallback(PolygonCreatedCB * callback, bool force) {
	if (_newPolyCB) {
		if (force) {
			_newPolyCB = callback;
		}
		else {
			throw MCException("Context already has callback.");
		}
	}
	else {
		_newPolyCB = callback;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool DrawPolygonContext::unregisterNewPolyCallback(PolygonCreatedCB * callback) {
	if (_newPolyCB == callback) {
		_newPolyCB = 0x0;
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void DrawPolygonContext::draw3DGL(bool select) {
	// TODO: Write status to the view's status.
	if (_state == DRAWING) {
		assert(_polygon != 0x0 && "In drawing state but no polygon available");
		// draw the polygon
		glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		// Draw the lines
		if (_polygon->isCCW()) {
			glColor3f(0.2f, 0.2f, 1.f);
		}
		else {
			glColor3f(1.f, 0.2f, 0.2f);
		}
		glLineWidth(2.0f);
		glBegin(GL_LINE_STRIP);
		for (Vector3 v : _polygon->_vertices) {
			glVertex3f(v.x(), v.y(), v.z());
		} 
		const Vector3 & v0 = _polygon->_vertices[0];
		glVertex3f(v0.x(), v0.y(), v0.z());
		glEnd();

		// draw point outlines
		const float PT_SIZE = 4.f;
		glColor3f(0.f, 0.f, 0.f);
		glPointSize(PT_SIZE + 2.f);
		glBegin(GL_POINTS);
		for (Vector3 v : _polygon->_vertices) {
			glVertex3f(v.x(), v.y(), v.z());
		}
		glEnd();

		// draw points
		glColor3f(1.f, 0.9f, 0.f);
		glPointSize(PT_SIZE);
		glBegin(GL_POINTS);
		for (Vector3 v : _polygon->_vertices) {
			glVertex3f(v.x(), v.y(), v.z());
		}
		glEnd();

		glPopAttrib();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////