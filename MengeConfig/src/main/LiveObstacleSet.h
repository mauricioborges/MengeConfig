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
	Vector3 * nearestVertex(const Vector2 & worldPos, float maxDist);
	
protected:

	/*!
	 *	@brief		The polygons in the obstacle set.
	 */
	std::vector<GLPolygon *>	_polygons;
};

#endif	// __LIVE_OBSTACLE_SET_H__