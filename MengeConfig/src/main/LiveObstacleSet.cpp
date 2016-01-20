#include "LiveObstacleSet.h"
#include "GLPolygon.h"
#include "glwidget.hpp"

#include <gl/GL.h>


///////////////////////////////////////////////////////////////////////////////
//                    Implementation of LiveObstacleSet
///////////////////////////////////////////////////////////////////////////////

LiveObstacleSet::LiveObstacleSet() : _polygons() {

}

///////////////////////////////////////////////////////////////////////////////

LiveObstacleSet::~LiveObstacleSet() {
	for (size_t i = 0; i < _polygons.size(); ++i) {
		delete _polygons[i];
	}
}

///////////////////////////////////////////////////////////////////////////////


void LiveObstacleSet::addPolygon(GLPolygon * poly) {
	_polygons.push_back(poly);
}

///////////////////////////////////////////////////////////////////////////////

void LiveObstacleSet::drawGL() {
	glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glLineWidth(3.f);
	for (GLPolygon * poly : _polygons) {
		if (poly->isCCW()) {
			glColor3f(0.2f, 0.2f, 0.6f);
		}
		else {
			glColor3f(0.6f, 0.2f, 0.2f);
		}

		glBegin(GL_LINE_STRIP);
		for (Vector3 v : poly->_vertices) {
			glVertex3f(v.x(), v.y(), v.z());
		}
		const Vector3 & v0 = poly->_vertices[0];
		glVertex3f(v0.x(), v0.y(), v0.z());
		glEnd();
	}

	glPopAttrib();
}

///////////////////////////////////////////////////////////////////////////////

SelectVertex LiveObstacleSet::nearestVertex(const Vector2 & worldPos, float maxDist) {
	SelectVertex sv;
	float d2 = maxDist * maxDist;
	float bestDistSq = 1e6f;
	for (GLPolygon * p : _polygons) {
		for (size_t i = 0; i < p->_vertices.size(); ++i) {
			Vector3 & v = p->_vertices[i];
			float dx = worldPos._x - v._x;
			float dy = worldPos._y - v._y;
			// omitting z because I'm assuming everything is on the ground plane.
			float distSq = dx *dx + dy * dy;
			if (distSq < bestDistSq && distSq < d2) {
				bestDistSq = distSq;
				sv._vert = &p->_vertices[i];
				sv._poly = p;
			}
		}
	}

	return sv;
}

///////////////////////////////////////////////////////////////////////////////


GLPolygon * LiveObstacleSet::nearestPolygon(const Vector2 & worldPos, float maxDist) {
	float d2 = maxDist * maxDist;
	GLPolygon * nearest = 0x0;
	float bestDistSq = 1e6f;
	for (GLPolygon * p : _polygons) {
		float distSq = p->distSquaredXY(worldPos);
		if (distSq < bestDistSq && distSq < d2) {
			nearest = p;
			bestDistSq = distSq;
		}
	}
	return nearest;
}

///////////////////////////////////////////////////////////////////////////////

void LiveObstacleSet::removePolygon(GLPolygon * poly) {
	// TODO: Implement this
}

///////////////////////////////////////////////////////////////////////////////

void LiveObstacleSet::removeVertex(const SelectVertex & vertex) {
	size_t vCount = vertex._poly->removeVertex(vertex._vert);
	if (vCount < 3) {
		std::vector<GLPolygon *>::iterator itr = _polygons.begin();
		for (; itr != _polygons.end(); ++itr) {
			if (*itr == vertex._poly) {
				_polygons.erase(itr);
				return;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//                    Implementation of SelectVertex
///////////////////////////////////////////////////////////////////////////////

SelectVertex::SelectVertex(const SelectVertex & sv) : _vert(sv._vert), _poly(sv._poly) {
}

///////////////////////////////////////////////////////////////////////////////

SelectVertex::SelectVertex() : _vert(0x0), _poly(0x0) {
}

///////////////////////////////////////////////////////////////////////////////

SelectVertex::SelectVertex(Vector3 * v, GLPolygon * p) : _vert(v), _poly(p) {
}

///////////////////////////////////////////////////////////////////////////////

void SelectVertex::set(float x, float y, float z) {
	_vert->set(x, y, z);
}

///////////////////////////////////////////////////////////////////////////////

bool SelectVertex::operator==(const SelectVertex &sv) {
	return _vert == sv._vert;
}

///////////////////////////////////////////////////////////////////////////////

SelectVertex & SelectVertex::operator=(const SelectVertex &sv) {
	_vert = sv._vert;
	_poly = sv._poly;
	return (*this);
}

///////////////////////////////////////////////////////////////////////////////

bool SelectVertex::operator!=(const SelectVertex &sv) {
	return _vert != sv._vert;
}
