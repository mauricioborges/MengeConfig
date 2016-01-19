#include "EditPolygonContext.h"

#include "AppLogger.hpp"
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

EditPolygonContext::EditPolygonContext(LiveObstacleSet * polygons) : _obstacleSet(polygons), _activePoly(0x0), _activeVert(0x0), _dragging(false), _mode(VERTEX) {

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
					if (_mode == VERTEX) {
						if (_activeVert) {
							_downOrigin.set(world.x(), world.y());
							_dragging = true;
						}
						result.setHandled(true);
					}
				}
				else if (evt->button() == Qt::RightButton && _dragging) {
					if (_mode == VERTEX && _dragging) {
						_activeVert->set(_downOrigin.x(), _downOrigin.y(), _activeVert->z());
						_dragging = false;
						result.set(true, true);
					}
				}
			} 
			else if (evt->type() == QEvent::MouseButtonRelease) {
				if (evt->button() == Qt::LeftButton || evt->button() == Qt::MiddleButton) {
					_dragging = false;
				}
			}
			else if (evt->type() == QEvent::MouseMove) {
				if (_dragging) {
					if (_mode == VERTEX) {
						assert(_activeVert && "Somehow dragging in vertex mode without an active vertex");
						world = view->snap(world);
						Vector2 newPos(_downOrigin + (world - _downPos));
						_activeVert->set(newPos.x(), newPos.y(), _activeVert->z());
						result.set(true, true);
					}
				}
				else if (_mode != NO_EDIT) { 
					if (_mode == VERTEX) {
						AppLogger::logStream << AppLogger::INFO_MSG << "Querying vertices at " << world << ", max dist: " << worldDist << AppLogger::END_MSG;
						Vector3 * v = _obstacleSet->nearestVertex(world, worldDist);
						result.set(true, v != _activeVert);
						_activeVert = v;
					}
					else {
						AppLogger::logStream << AppLogger::INFO_MSG << "No support for edge or polygon yet." << AppLogger::END_MSG;
					}
				}
			}
		}
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////

Menge::SceneGraph::ContextResult EditPolygonContext::handleKeyboard(QKeyEvent * evt, GLWidget * view) {
	Menge::SceneGraph::ContextResult result(false, false);
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void EditPolygonContext::activate() {

}

/////////////////////////////////////////////////////////////////////////////////////////////

void EditPolygonContext::deactivate() {

}

/////////////////////////////////////////////////////////////////////////////////////////////

void EditPolygonContext::draw3DGL(bool select) {
	glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	if (_mode == VERTEX) {
		if (_activeVert != 0x0) {
			const float PT_SIZE = 6.f;
			glColor3f(0.f, 0.f, 0.f);
			glPointSize(PT_SIZE + 2.f);
			glBegin(GL_POINTS);
			glVertex3f(_activeVert->x(), _activeVert->y(), _activeVert->z());
			glEnd();

			// draw points
			glColor3f(1.f, 0.9f, 0.f);
			glPointSize(PT_SIZE);
			glBegin(GL_POINTS);
			glVertex3f(_activeVert->x(), _activeVert->y(), _activeVert->z());
			glEnd();
		}
	}

	glPopAttrib();
}

/////////////////////////////////////////////////////////////////////////////////////////////

void EditPolygonContext::setEditMode(EditMode mode) {

}

/////////////////////////////////////////////////////////////////////////////////////////////

