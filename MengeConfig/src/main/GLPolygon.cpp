#include "GLPolygon.h"

#include <cassert>

///////////////////////////////////////////////////////////////////////////////
//                    Implementation of GLGLPolygon
///////////////////////////////////////////////////////////////////////////////

const Vector3 GLPolygon::PLANE_NORMAL(0.f, 0.f, 1.f);

///////////////////////////////////////////////////////////////////////////////


GLPolygon::GLPolygon() : _vertices(), _winding(NO_WINDING){

}

///////////////////////////////////////////////////////////////////////////////

void GLPolygon::addVertex(const Vector3 & v) {
	_vertices.push_back(v);
	if (_vertices.size() >= 3) _winding = computeWinding(PLANE_NORMAL);
}

///////////////////////////////////////////////////////////////////////////////

void GLPolygon::reverseWinding() {
	// Do nothing if winding is undefined.
	if (_winding != NO_WINDING) {
		if (_winding == CCW) _winding = CW;
		else if (_winding = CW) _winding = CCW;
		// TODO: Reverse the order of the vertices
		const size_t COUNT = _vertices.size();
		for (size_t i = 0; i < COUNT / 2; ++i) {
			Vector3 temp = _vertices[i];
			_vertices[i] = _vertices[COUNT - i - 1];
			_vertices[COUNT - i - 1] = temp;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

size_t GLPolygon::popVertex() {
	size_t size  = _vertices.size();
	if (size > 0) {
		--size;
		_vertices.pop_back();
		if (size > 2) _winding = computeWinding(PLANE_NORMAL);
	}
	return size;
}

///////////////////////////////////////////////////////////////////////////////

void GLPolygon::makeCCW() {
	if (_winding != CW) {
		reverseWinding();
	}
	else if (_winding == NO_WINDING) {
		if (_vertices.size() >= 3) {
			_winding = computeWinding(Vector3(0.f, 0.f, 1.f));
			if (_winding == CW) reverseWinding();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

GLPolygon::Winding GLPolygon::computeWinding(const Vector3 & upDir) {
	assert(_vertices.size() >= 3 && "Calling compute winding with insufficient vertices");

	float turning = 0.f;
	const size_t COUNT = _vertices.size();
	// TODO: This does twice as many edge calculations as necessary -- I compute every edge
	//		twice.  I *could* do once per edge.
	for (size_t i = 0; i < COUNT; ++i) {
		size_t j = (i + 1) % COUNT;
		size_t k = (i + 2) % COUNT;
		Vector3 e1 = _vertices[j] - _vertices[i];
		e1.normalize();
		Vector3 e2 = _vertices[k] - _vertices[j];
		e2.normalize();
		float dot = e1 * e2;
		float cross = e1.cross(e2) * upDir;
		turning += atan2(cross, dot);
	}
	return turning < 0.f ? CW : CCW;
}