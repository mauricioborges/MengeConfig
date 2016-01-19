/*!
 *	@file		Polygon.h
 *	@brief		The definition of a closed polygon.
 */

#ifndef __POLYGON_H__
#define	__POLYGON_H__

#include <vector>

#include "Math/Vector3.h"
using namespace Menge::Math;

// forward declarations
class DrawPolygonContext;
class LiveObstacleSet;

/*!
 *	@brief		A simple closed polygon -- a sequence of points.
 */
class GLPolygon {
public:
	/*!
	 *	@brief		The winding of the polygon.
	 */
	enum Winding {
		NO_WINDING,		/// Undefined winding
		CCW,			/// Points are counter-clockwise.
		CW				/// Points are clockwise.
	};

	/*!
	 *	@brief		Constructor.
	 */
	GLPolygon();

	/*!
	 *	@brief		Adds a vector to the polygon.
	 *
	 *	@param		v		The vertex to add.
	 */
	void addVertex(const Vector3 & v);

	/*!
	 *	@brief		Reverses the winding of the polygon.
	 */
	void reverseWinding();

	/*!
	 *	@brief		Pops the last vertex off the polygon.
	 *
	 *	@returns	The number of vertices remaining.
	 */
	size_t popVertex();

	/*!
	 *	@brief		Reports if the winding is counter clockwise.
	 */
	bool isCCW() { return _winding == CCW; }

	/*!
	 *	@brief		Forces the polygon to have counter-clockwise winding.
	 */
	void makeCCW();

	friend class DrawPolygonContext;
	friend class LiveObstacleSet;

protected:

	/*!
	 *	@brief		Computes the winding inherent in the vertex positions.
	 *
	 *	This assumes that there are at least three vertices.  If called with fewer, there will be an error.
	 *
	 *	@param		upDir		The normal to the plain with respect to which the winding is computed.
	 *	@returns	The winding actually represented by the vertex order.
	 */
	Winding computeWinding(const Vector3 & upDir);

	/*!
	 *	@brief		The ordered vertices in the polygon.
	 */
	std::vector<Vector3>	_vertices;

	/*!
	 *	@brief		The winding of the polygon.
	 */
	Winding		_winding;

	/*!
	 *	@brief		The normal of the plane that the polygon lies on.
	 *
	 *	This is a bit of a hack.  The polygon is defined in 3D so an arbitrary 
	 *	sequence of points would keep it from being planar.  So, this is a fragile
	 *	assumption about them lying on the x-y plane.
	 */
	static const Vector3 PLANE_NORMAL;
};

#endif	// __POLYGON_H__