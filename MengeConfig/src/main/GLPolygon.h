/*!
 *	@file		Polygon.h
 *	@brief		The definition of a closed polygon.
 */

#ifndef __POLYGON_H__
#define	__POLYGON_H__

#include <vector>

#include "Math/Vector.h"
using namespace Menge::Math;

// forward declarations
class DrawPolygonContext;
class LiveObstacleSet;
class EditPolygonContext;
class GLPolygon;

/*!
*	@brief		An edge drawn from the obstacle set.
*/
class SelectEdge {
public:

	/*!
	 *	Constructor.
	 */
	SelectEdge();

	/*!
	 *	@brief		Copy constructor.
	 *
	 *	@param		sv		The select vertex to copy.
	 */
	SelectEdge(const SelectEdge & sv);

	/*!
	 *	@brief		Reports if a vertex was actually selected.
	 *
	 *	@returns	True if this contains a vertex, false if not.
	 */
	inline bool isValid() const { return _v0 != 0x0; }

	/*!
	 *	@brief		Clears the selected vertex.
	 */
	inline void clear() {
		_v0 = 0x0;
		_v1 = 0x0;
		_poly = 0x0;
	}

	/*!
	 *	@brief		Sets the x and y values of the underlying vertex.
	 *
	 *	@param		x		The vertex's x value.
	 *	@param		y		The vertex's y value.
	 *	@param		z		The vertex's y value.
	 */
	void set0(float x, float y, float z);

	/*!
	 *	@brief		Retrieves the x-component of the underlying vertex.
	 */
	inline float x0() const { return _v0->x(); }

	/*!
	 *	@brief		Retrieves the y-component of the underlying vertex.
	 */
	inline float y0() const { return _v0->y(); }

	/*!
	 *	@brief		Retrieves the z-component of the underlying vertex.
	 */
	inline float z0() const { return _v0->z(); }

	/*!
	*	@brief		Sets the x and y values of the underlying vertex.
	*
	*	@param		x		The vertex's x value.
	*	@param		y		The vertex's y value.
	*	@param		z		The vertex's y value.
	*/
	void set1(float x, float y, float z);

	/*!
	*	@brief		Retrieves the x-component of the underlying vertex.
	*/
	inline float x1() const { return _v1->x(); }

	/*!
	*	@brief		Retrieves the y-component of the underlying vertex.
	*/
	inline float y1() const { return _v1->y(); }

	/*!
	*	@brief		Retrieves the z-component of the underlying vertex.
	*/
	inline float z1() const { return _v1->z(); }

	/*!
	*	@brief		Assignment operator.
	*
	*	@param		sv		The select vertex to copy.
	*/
	SelectEdge & operator=(const SelectEdge &sv);

	/*!
	*	@brief		Reports if the two select vertices match.
	*
	*	@param		sv		The vertex to compare with this one.
	*	@returns	True if they reference the same underlying vertex.
	*				False otherwise -- even if they *contain* the same
	*				numerical values.
	*/
	bool operator==(const SelectEdge &sv);

	/*!
	*	@brief		Reports if the two select vertices are different.
	*
	*	@param		sv		The vertex to compare with this one.
	*	@returns	True if they reference the different underlying vertices.
	*/
	bool operator!=(const SelectEdge &sv);

	// Only live obstacle set can create new instances.
	friend class LiveObstacleSet;
	friend class GLPolygon;

private:

	/*!
	*	Constructor.
	*
	*	@param		v0		The first vertex referenced.
	*	@param		v1		The second vertex referenced.
	*	@param		p		The polygon to which this belongs.
	*/
	SelectEdge(Vector3 * v0, Vector3 * v1, GLPolygon * p);

	/*!
	*	The first vertex in the edge.
	*/
	Vector3 *	_v0;

	/*!
	*	The second vertex in the edge.
	*/
	Vector3 *	_v1;

	/*!
	*	The polygon the vertex belongs to.
	*/
	GLPolygon * _poly;

};

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

	/*!
	 *	@brief		Removes the given vertex from this polygon.
	 *
	 *	@param		v		A pointer to the vertex to remove.
	 *	@returns	The number of vertices remaining.
	 */
	size_t removeVertex(Vector3 * v);

	/*!
	 *	@brief		Computes the smallest distance between the query point and
	 *				the polygon (as projected on the x-y plane.
	 *
	 *	@param		v		The query point on the x-y plane.
	 *	@returns	The squared distance between v and the polygon.
	 */
	float distSquaredXY(const Vector2 & v);

	/*!
	 *	@brief		Finds the nearest edge to the query point (projected onto the x-y plane.
	 *
	 *	@param		v		The query point.
	 *	@param		edge	The edge data will be populated into this edge.
	 *	@returns	The squared distance to this edge.
	 */
	float nearestEdgeXY(const Vector2 & v, SelectEdge & edge);

	friend class DrawPolygonContext;
	friend class LiveObstacleSet;
	friend class EditPolygonContext;

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