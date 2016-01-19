/*!
 *	@fiole		LiveObstacleSet.h
 *	@brief		An approximation of an obstacle set that can be manipulated interactively.
 */

#ifndef __LIVE_OBSTACLE_SET_H__
#define	__LIVE_OBSTACLE_SET_H__

#include <vector>

#include "Math/Vector3.h"
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
	
protected:

	/*!
	 *	@brief		The polygons in the obstacle set.
	 */
	std::vector<GLPolygon *>	_polygons;
};

#endif	// __LIVE_OBSTACLE_SET_H__