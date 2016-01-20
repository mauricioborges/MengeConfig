/*!
 *	@fiole		LiveObstacleSet.h
 *	@brief		An approximation of an obstacle set that can be manipulated interactively.
 */

#ifndef __LIVE_OBSTACLE_SET_H__
#define	__LIVE_OBSTACLE_SET_H__

#include <vector>

#include "Math/Vector.h"
using namespace Menge::Math;

class GLPolygon;


/*!
*	@brief		A vertex drawn from the obstacle set.
*/
class SelectVertex {
public:
	
	/*!
	 *	Constructor.
	 */
	SelectVertex();

	/*!
	 *	@brief		Copy constructor.
	 *
	 *	@param		sv		The select vertex to copy.
	 */
	SelectVertex(const SelectVertex & sv);

	/*!
	 *	@brief		Reports if a vertex was actually selected.
	 *
	 *	@returns	True if this contains a vertex, false if not.
	 */
	inline bool isValid() const { return _vert != 0x0; }

	/*!
	 *	@brief		Clears the selected vertex.
	 */
	inline void clear() {
		_vert = 0x0;
		_poly = 0x0;
	}

	/*!
	 *	@brief		Sets the x and y values of the underlying vertex.
	 *
	 *	@param		x		The vertex's x value.
	 *	@param		y		The vertex's y value.
	 *	@param		z		The vertex's y value.
	 */
	void set(float x, float y, float z);

	/*!
	 *	@brief		Retrieves the x-component of the underlying vertex.
	 */
	inline float x() const { return _vert->x(); }

	/*!
	*	@brief		Retrieves the y-component of the underlying vertex.
	*/
	inline float y() const { return _vert->y(); }

	/*!
	*	@brief		Retrieves the z-component of the underlying vertex.
	*/
	inline float z() const { return _vert->z(); }

	/*!
	 *	@brief		Assignment operator.
	 *
	 *	@param		sv		The select vertex to copy.
	 */
	SelectVertex & operator=(const SelectVertex &sv);

	/*!
	 *	@brief		Reports if the two select vertices match.
	 *
	 *	@param		sv		The vertex to compare with this one.
	 *	@returns	True if they reference the same underlying vertex.
	 *				False otherwise -- even if they *contain* the same
	 *				numerical values.
	 */
	bool operator==(const SelectVertex &sv);

	/*!
	 *	@brief		Reports if the two select vertices are different.
	 *
	 *	@param		sv		The vertex to compare with this one.
	 *	@returns	True if they reference the different underlying vertices.
	 */
	bool operator!=(const SelectVertex &sv);

	// Only live obstacle set can create new instances.
	friend class LiveObstacleSet;

private:

	/*!
	 *	Constructor.
	 *
	 *	@param		The vertex referenced.
	 *	@param		The polygon to which this belongs.
	 */
	SelectVertex(Vector3 * v, GLPolygon * p);

	/*!
	 *	The vertex selected.
	 */
	Vector3 *	_vert;

	/*!
	 *	The polygon the vertex belongs to.
	 */
	GLPolygon * _poly;

};

/*!
 *	@brief		This is a representation of an obstacle set that can be edited.
 */
class LiveObstacleSet {
public:
	/*!
	 *	@brief		Constructor.
	 */
	LiveObstacleSet();

	/*!
	 *	@brief		Destructor.
	 */
	virtual ~LiveObstacleSet();

	/*!
	 *	@brief		Adds a polygon to the set.
	 *
	 *	@param		poly		The polygon to add to the set.
	 */
	void addPolygon(GLPolygon * poly);


	/*!
	 *	@brief		Removes the selected vertex from its polygon -- if the polygon
	 *				ends up with 2 vertices, the polygon in turn is deleted.
	 *
	 *	@param		vertex		The selected vertex to delete.
	 */
	void removeVertex(const SelectVertex & vertex);

	/*!
	 *	@brief		Draws the polygon set to the OpenGL context.
	 */
	void drawGL();

	/*!
	 *	@brief		Selects the nearest vertex to the given position (up to the
	 *				specified maximum distance.
	 *
	 *	@param		worldPos		The query point (in world space).
	 *	@param		maxDist			The maximum distance to consider a candidate (in
	 *								world space.)
	 *	@returns	A pointer to the nearest.  If none are sufficiently close, null.
	 */
	SelectVertex nearestVertex(const Vector2 & worldPos, float maxDist);
	
protected:

	/*!
	 *	@brief		The polygons in the obstacle set.
	 */
	std::vector<GLPolygon *>	_polygons;
};


#endif	// __LIVE_OBSTACLE_SET_H__