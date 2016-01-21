#include "GLPolygon.h"

#include <cassert>

///////////////////////////////////////////////////////////////////////////////
//                    Implementation of Helper method
///////////////////////////////////////////////////////////////////////////////

/*!
 *	@brief		Computes the distance to the edge defined by v0 and v1 to the
 *				query point q projected on the x-y plane.
 *
 *	@param		v0		The first point of the edge.
 *	@param		v2		The second point of the edge.
 *	@param		q		The query point.
 */
float distSqXY(const Vector3 & v0, const Vector3 & v1, const Vector2 & q) {
	Vector2 u0(v0.x(), v0.y());
	Vector2 u1(v1.x(), v1.y());
	Vector2 dir(u1 - u0);
	Vector2 p(q - u0);
	float len = dir.Length();
	if (len > 0.0001f) {
		dir /= len;	// normalize
		float dp = p * dir;
		if (dp < 0) {
			return p * p;
		}
		else if (dp > len) {
			p.set(q - u1);
			return p * p;
		}
		else {
			float dist = det(dir, p);
			return dist * dist;
		}
	}
	else {
		return p * p;
	}
}

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

///////////////////////////////////////////////////////////////////////////////

size_t GLPolygon::removeVertex(Vector3 * v) {
	std::vector<Vector3>::iterator itr = _vertices.begin();
	for (; itr != _vertices.end(); ++itr) {
		if (&(*itr) == v) {
			_vertices.erase(itr);
			if (_vertices.size() >= 3) _winding = computeWinding(PLANE_NORMAL);
			break;
		}
	}
	return _vertices.size();
}

///////////////////////////////////////////////////////////////////////////////

float GLPolygon::distSquaredXY(const Vector2 & v) {
	SelectEdge nearest;
	return nearestEdgeXY(v, nearest);
}

///////////////////////////////////////////////////////////////////////////////

float GLPolygon::nearestEdgeXY(const Vector2 & v, SelectEdge & edge) {
	int j = _vertices.size() - 1;
	float bestDistSq = distSqXY(_vertices[j], _vertices[0], v);
	edge = SelectEdge(&(_vertices[j]), &(_vertices[0]), this);
	SelectEdge temp;
	for (size_t i = 0; i < _vertices.size() - 1; ++i) {
		const Vector3 & v0 = _vertices[i];
		const Vector3 & v1 = _vertices[i + 1];
		float distSq = distSqXY(v0, v1, v);
		if (distSq < bestDistSq) {
			bestDistSq = distSq;
			edge = SelectEdge(&(_vertices[i]), &(_vertices[i+1]), this);
		}
	}
	return bestDistSq;
}

///////////////////////////////////////////////////////////////////////////////

Vector3 * GLPolygon::insertPoint(const Vector3 * v0, const Vector2 & groundPos) {
	Vector3 * start = &_vertices[0];
	size_t delta = v0 - start;
	if (delta == _vertices.size() - 1) {
		// new vertex gets appended on the end
		_vertices.push_back(Vector3(groundPos.x(), groundPos.y(), 0.f));
	}
	else {
		// insert in the middle
		std::vector<Vector3>::iterator itr = _vertices.begin();
		itr += delta + 1;
		_vertices.insert(itr, Vector3(groundPos.x(), groundPos.y(), 0.f));
	}
	// I can't just do v0 + 1, because the vector may end up putting
	//	the data in some alternative location.
	return &_vertices[delta + 1];
}

///////////////////////////////////////////////////////////////////////////////
//                    Implementation of SelectEdge
///////////////////////////////////////////////////////////////////////////////

SelectEdge::SelectEdge(const SelectEdge & se) : _v0(se._v0), _v1(se._v1), _poly(se._poly) {
}

///////////////////////////////////////////////////////////////////////////////

SelectEdge::SelectEdge() : _v0(0x0), _v1(0x0), _poly(0x0) {
}

///////////////////////////////////////////////////////////////////////////////

SelectEdge::SelectEdge(Vector3 * v0, Vector3 * v1, GLPolygon * p) : _v0(v0), _v1(v1), _poly(p) {
}

///////////////////////////////////////////////////////////////////////////////

void SelectEdge::set0(const Vector2 & v) {
	_v0->set(v.x(), v.y(), _v0->z());
}

///////////////////////////////////////////////////////////////////////////////

void SelectEdge::set1(const Vector2 & v) {
	_v1->set(v.x(), v.y(), _v1->z());
}

///////////////////////////////////////////////////////////////////////////////

bool SelectEdge::operator==(const SelectEdge &sv) {
	// this is sufficent because it should serve as a unique identifier.
	return _v0 == sv._v0;
}

///////////////////////////////////////////////////////////////////////////////

SelectEdge & SelectEdge::operator=(const SelectEdge &se) {
	_v0 = se._v0;
	_v1 = se._v1;
	_poly = se._poly;
	return (*this);
}

///////////////////////////////////////////////////////////////////////////////

bool SelectEdge::operator!=(const SelectEdge &sv) {
	return _v0 != sv._v1;
}
