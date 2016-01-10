#include "GridNode.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of GridNode
/////////////////////////////////////////////////////////////////////////////////////////////

GridNode::GridNode(Menge::SceneGraph::GLDagNode * parent) : Menge::SceneGraph::GLNode(), ReferenceGrid() {

}

/////////////////////////////////////////////////////////////////////////////////////////////

GridNode::~GridNode() {

}

/////////////////////////////////////////////////////////////////////////////////////////////

void GridNode::drawGL(bool select) {
	if (!select && _visible) {
		glPushAttrib(GL_LIGHTING_BIT);

		// boundary
		glLineWidth(3.f);
		glColor3f(0.5f, 0.5f, 0.5f);
		glBegin(GL_LINE_STRIP);
		float X = _originX + _width;
		float Y = _originY + _height;
		glVertex3f(_originX, _originY, 0.f);
		glVertex3f(X, _originY, 0.f);
		glVertex3f(X, Y, 0.f);
		glVertex3f(_originX, Y, 0.f);
		glVertex3f(_originX, _originY, 0.f);
		glEnd();

		// minor lines
		glColor3f(0.f, 0.f, 0.f);
		glLineWidth(1.f);
		float delta = _majorDist / (_minorCount + 1);

		glBegin(GL_LINES);
		// parallel to x-axis
		float y = _originY;
		int i = 0;
		while (y < Y) {
			y = _originY + i * delta;
			glVertex3f(_originX, y, 0.f);
			glVertex3f(X, y, 0.f);
			++i;
		}

		//parallel to y-axis
		float x = _originX;
		i = 0;
		while (x < X) {
			x = _originX + i * delta;
			glVertex3f(x, _originY, 0.f);
			glVertex3f(x, Y, 0.f);
			++i;
		}
		glEnd();

		// Major lines
		glLineWidth(2.f);
		glBegin(GL_LINES);
		// parallel to x-axis
		y = _originY;
		i = 0;
		while (y < Y) {
			y = _originY + i * _majorDist;
			glVertex3f(_originX, y, 0.f);
			glVertex3f(X, y, 0.f);
			++i;
		}

		//parallel to y-axis
		x = _originX;
		i = 0;
		while (x < X) {
			x = _originX + i * _majorDist;
			glVertex3f(x, _originY, 0.f);
			glVertex3f(x, Y, 0.f);
			++i;
		}

		glEnd();

		glPopAttrib();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GridNode::newContext() {

}

/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////