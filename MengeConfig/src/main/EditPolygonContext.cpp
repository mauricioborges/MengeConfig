#include "EditPolygonContext.h"

#include "AppLogger.hpp"
#include "EditPolygonWidget.hpp"
#include "LiveObstacleSet.h"
#include "GLPolygon.h"
#include "glwidget.hpp"

#include <Math/vector.h>
using namespace Menge::Math;

#include <cassert>
#include <gl/GL.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of EditPolygonContext
/////////////////////////////////////////////////////////////////////////////////////////////

EditPolygonContext::EditPolygonContext(LiveObstacleSet * polygons) : QtContext(), _obstacleSet(polygons), _activePoly(0x0), _activeVert(), _dragging(false), _mode(VERTEX) {
	_widget = new EditPolygonWidget(this);
}

/////////////////////////////////////////////////////////////////////////////////////////////

EditPolygonContext::~EditPolygonContext() {

}

/////////////////////////////////////////////////////////////////////////////////////////////

Menge::SceneGraph::ContextResult EditPolygonContext::handleMouse(QMouseEvent * evt, GLWidget * view) {
	Menge::SceneGraph::ContextResult result = QtContext::handleMouse(evt, view);
	if (!result.isHandled()) {
		Qt::KeyboardModifiers mods = evt->modifiers();
		if (mods == Qt::NoModifier) {
			Vector2 world;
			view->getWorldPos(evt->pos(), world, true);
			float worldDist = view->getWorldScale(15);	// 15 pixel minimum distance to be selectable

			if (evt->type() == QEvent::MouseButtonPress) {
				if (evt->button() == Qt::LeftButton) {
					_downPos.set(world);
					if (_activePoly) {
						_downOrigin.set(_activePoly->_vertices[0].x(), _activePoly->_vertices[0].y());
						_polyVertices.resize(_activePoly->_vertices.size());
						for (size_t i = 0; i < _activePoly->_vertices.size(); ++i) {
							_polyVertices[i].set(_activePoly->_vertices[i].x() - _downOrigin.x(), _activePoly->_vertices[i].y() - _downOrigin.y());
						}
						_dragging = true;
					} else if (_activeVert.isValid()) {
						_downOrigin.set(_activeVert.x(), _activeVert.y());
						_dragging = true;
					} else if (_activeEdge.isValid()) {
						_downOrigin.set(_activeEdge._v0->x(), _activeEdge._v0->y());
						_edgeOffset.set(_activeEdge._v1->x() - _activeEdge._v0->x(),
							_activeEdge._v1->y() - _activeEdge._v0->y());
						_dragging = true;
					}
					result.setHandled(true);
				} else if (evt->button() == Qt::RightButton && _dragging) {
					if (_activeVert.isValid()) {
						if (_mode == EDGE) {
							// inserted a vertex -- cancellation requires removal
							_obstacleSet->removeVertex(_activeVert);
							_activeVert.clear();
						} else {
							// assume vertex edit mode -- simply reposition the vertex
							_activeVert.set(_downOrigin.x(), _downOrigin.y(), _activeVert.z());
						}
					} else if (_activeEdge.isValid()) {
						_activeEdge.set0(_downOrigin);
						_activeEdge.set1(_downOrigin + _edgeOffset);
					} else if (_activePoly) {
						for (size_t i = 0; i < _activePoly->_vertices.size(); ++i) {
							_activePoly->_vertices[i].set(_downOrigin.x() + _polyVertices[i].x(),
								_downOrigin.y() + _polyVertices[i].y(), 
								_activePoly->_vertices[i].z());
						}
					}
					_dragging = false;
					result.set(true, true);
				} else if (evt->button() == Qt::MiddleButton && _activeEdge.isValid()) {
					_activeVert = _obstacleSet->insertVertex(world, _activeEdge);
					_downPos.set(world);
          _downOrigin.set( _activeVert.x(), _activeVert.y() );
					_activeEdge.clear();
					_dragging = true;
					result.set(true, true);
				}
			} else if (evt->type() == QEvent::MouseButtonRelease) {
				if (evt->button() == Qt::LeftButton || evt->button() == Qt::MiddleButton) {
					_dragging = false;
				}
			} else if (evt->type() == QEvent::MouseMove) {
				if (_dragging) {
          Vector2 newPos( _downOrigin + ( world - _downPos ) );

          // If moving vertices, allow snaping.
          if ( _activeVert.isValid() ) newPos = view->snap( newPos );
					
          if ( _activeVert.isValid() ) {
						_activeVert.set(newPos.x(), newPos.y(), _activeVert.z());
					} else if (_activeEdge.isValid()) {
						_activeEdge.set0(newPos);
						_activeEdge.set1(newPos + _edgeOffset);
					} else if (_activePoly) {
						for (size_t i = 0; i < _activePoly->_vertices.size(); ++i) {
							_activePoly->_vertices[i].set(newPos.x() + _polyVertices[i].x(),
								newPos.y() + _polyVertices[i].y(), 
								_activePoly->_vertices[i].z());
						}
					}
					result.set(true, true);
				}
				else if (_mode == VERTEX) {
					SelectVertex v = _obstacleSet->nearestVertex(world, worldDist);
					result.set(true, v != _activeVert);
					_activeVert = v;
					_activeEdge.clear();
					_activePoly = 0x0;
				}
				else if (_mode == POLY) {
					GLPolygon * poly = _obstacleSet->nearestPolygon(world, worldDist);
					result.set(true, poly != _activePoly);
					_activePoly = poly;
					_activeVert.clear();
					_activeEdge.clear();
				}
				else if (_mode == EDGE) {
					SelectEdge e = _obstacleSet->nearestEdge(world, worldDist);
					result.set(true, e != _activeEdge);
					_activeEdge = e;
					_activeVert.clear();
					_activePoly = 0x0;
				}
			}
		}
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////

Menge::SceneGraph::ContextResult EditPolygonContext::handleKeyboard(QKeyEvent * evt, GLWidget * view) {
	Menge::SceneGraph::ContextResult result = QtContext::handleKeyboard(evt, view);

	if (!result.isHandled()) {
		Qt::KeyboardModifiers mods = evt->modifiers();
		bool noMods = mods == Qt::NoModifier;
		if (evt->type() == QEvent::KeyPress) {
			if (noMods && evt->key() == Qt::Key_V) {
				result.set(true, setState(VERTEX));
				
			}
			else if (noMods && evt->key() == Qt::Key_E) {
				result.set(true, setState(EDGE));
				
			}
			else if (noMods && evt->key() == Qt::Key_P) {
				result.set(true, setState(_mode = POLY));
			}
			else if (noMods && evt->key() == Qt::Key_R && _activePoly) {
				_activePoly->reverseWinding();
				result.set(true, true);
			}
			else if (noMods && evt->key() == Qt::Key_C) {
				if (_activePoly) {
					_obstacleSet->removePolygon(_activePoly);
					_activePoly = 0x0;
					result.set(true, true);
				}
				else if (_activeVert.isValid()) {
					_obstacleSet->removeVertex(_activeVert);
					_activeVert.clear();
					result.set(true, true);
				}
				else if (_activeEdge.isValid()) {
					_obstacleSet->collapseEdge(_activeEdge);
					_activeEdge.clear();
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

QWidget * EditPolygonContext::getContextWidget() {
	return _widget;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void EditPolygonContext::activate() {

}

/////////////////////////////////////////////////////////////////////////////////////////////

void EditPolygonContext::deactivate() {

}

/////////////////////////////////////////////////////////////////////////////////////////////

bool EditPolygonContext::setState(EditMode mode) {
	if (mode != _mode) {
		_mode = mode;
		_dragging = false;
		_activePoly = 0x0;
		_activeVert.clear();
		_activeEdge.clear();
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void EditPolygonContext::draw3DGL(bool select) {
	glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	if (_activeVert.isValid()) {
		const float PT_SIZE = 6.f;
		glColor3f(0.f, 0.f, 0.f);
		glPointSize(PT_SIZE + 2.f);
		glBegin(GL_POINTS);
		glVertex3f(_activeVert.x(), _activeVert.y(), _activeVert.z());
		glEnd();

		// draw points
		glColor3f(1.f, 0.9f, 0.f);
		glPointSize(PT_SIZE);
		glBegin(GL_POINTS);
		glVertex3f(_activeVert.x(), _activeVert.y(), _activeVert.z());
		glEnd();
	}
	else if (_activePoly) {
		glColor3f(0.f, 0.f, 0.0f);
		glLineWidth(5.f);
		glBegin(GL_LINE_LOOP);
		for (const Vector3 & v : _activePoly->_vertices) {
			glVertex3f(v.x(), v.y(), v.z());
		}
		glVertex3f(_activePoly->_vertices[0].x(), _activePoly->_vertices[0].y(), _activePoly->_vertices[0].z());
		glEnd();

		glColor3f(0.9f, 0.9f, 0.0f);
		glLineWidth(3.f);
		glBegin(GL_LINE_LOOP);
		for (const Vector3 & v : _activePoly->_vertices) {
			glVertex3f(v.x(), v.y(), v.z());
		}
		glVertex3f(_activePoly->_vertices[0].x(), _activePoly->_vertices[0].y(), _activePoly->_vertices[0].z());
		glEnd();
	}
	else if (_activeEdge.isValid()) {
		glColor3f(0.f, 0.f, 0.0f);
		glLineWidth(5.f);
		glBegin(GL_LINE_LOOP);
		glVertex3f(_activeEdge._v0->x(), _activeEdge._v0->y(), _activeEdge._v0->z());
		glVertex3f(_activeEdge._v1->x(), _activeEdge._v1->y(), _activeEdge._v1->z());
		glEnd();

		glColor3f(0.9f, 0.9f, 0.0f);
		glLineWidth(3.f);
		glBegin(GL_LINE_LOOP);
		glVertex3f(_activeEdge._v0->x(), _activeEdge._v0->y(), _activeEdge._v0->z());
		glVertex3f(_activeEdge._v1->x(), _activeEdge._v1->y(), _activeEdge._v1->z());
		glEnd();
	}

	glPopAttrib();
}

/////////////////////////////////////////////////////////////////////////////////////////////

void EditPolygonContext::setEditMode(EditMode mode) {

}

/////////////////////////////////////////////////////////////////////////////////////////////

