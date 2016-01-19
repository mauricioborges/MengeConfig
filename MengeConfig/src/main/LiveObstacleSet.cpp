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

Vector3 * LiveObstacleSet::nearestVertex(const Vector2 & worldPos, float maxDist) {
	float d2 = maxDist * maxDist;
	Vector3 * nearest = 0x0;
	float bestDistSq = 1e6f;
	for (GLPolygon * p : _polygons) {
		for (size_t i = 0; i < p->_vertices.size(); ++i) {
			Vector3 & v = p->_vertices[i];
		//for (Vector3 & v : p->_vertices) {
			float dx = worldPos._x - v._x;
			float dy = worldPos._y - v._y;
			// omitting z because I'm assuming everything is on the ground plane.
			float distSq = dx *dx + dy * dy;
			if (distSq < bestDistSq) {
				bestDistSq = distSq;
				nearest = &p->_vertices[i];
			}
		}
	}

	return bestDistSq <= d2 ? nearest : 0x0;
}

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
