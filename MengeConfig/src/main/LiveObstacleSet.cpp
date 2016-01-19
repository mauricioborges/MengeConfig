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