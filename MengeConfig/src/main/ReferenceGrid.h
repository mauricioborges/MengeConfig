#ifndef __REFERENCE_GRID_H__
#define __REFERENCE_GRID_H__

#include <MengeCore/Math/Vector2.h>

using namespace Menge::Math;

/*!
 *	@brief		Defines the reference grid for the simulation domain.
 *
 *	This is a 2D grid intended to lie on the x-y plane.
 */
class ReferenceGrid
{
public:
	/*!
	 *	@brief		Constructor
	 */
	ReferenceGrid();

	/*!
	 *	@brief		Destructor
	 */
	virtual ~ReferenceGrid();

	/*!
	 *	@brief		Sets the grid's origin.
	 *
	 *	@param		x		The x-position of the grid's origin.
	 *	@param		y		The y-position of the grid's origin.
	 */
	void setOrigin(float x, float y);

	/*!
	 *	@brief		Reports the grid's origin.
	 *
	 *	@returns	A 2D vector with the x- and y-values, respectively.
	 */
	Vector2 getOrigin() const {
		return Vector2(_originX, _originY);
	}

	/*!
	 *	@brief		Sets the size of the grid.
	 *
	 *	@param		w		The extent of the grid along the x-axis.
	 *	@param		h		The extent of the grid along the y-axis.
	 */
	void setSize(float w, float h);

	/*!
	 *	@brief		Reports the grid's size.
	 *
	 *	@returns	A 2D vector with the width and height values, respectively.
	 */
	Vector2 getSize() const {
		return Vector2(_width, _height);
	}

	/*!
	 *	@brief		Sets the number of minor lines between major lines.
	 *
	 *	@param		count		The number of minor lines.
	 */
	void setMinorCount(unsigned int count);

	/*!
	 *	@brief		Reports the number of minor lines between major lines.
	 *
	 *	@returns	The number of minor lines.
	 */
	unsigned int getMinorCount() const { return _minorCount; }

	/*!
	 *	@brief		Sets the distance between major lines.
	 *
	 *	@param		dist		The distance between major lines.
	 */
	void setMajorDist(float dist);

	/*!
	 *	@brief		Reports the distance between major lines.
	 *
	 *	@returns	The distance between major lines.
	 */
	float getMajorDist() const {
		return _majorDist;
	}

	/*!
	 *	@brief		Returns the nearest grid point to the input point.
	 *
	 *	@param		point		The 2D query point.
	 *	@returns	The point on the grid nearest the query point.
	 */
	Vector2 snap(const Vector2 & point);

	/*!
	 *	@brief		"Snaps" the query point to a line parallel with grid's y-axis (preserving the y-value).
	 *
	 *	@param		point		The 2D query point.
	 *	@returns	The point with the same y, but with the x-value of the nearest vertical grid line.
	 */
	Vector2 snapVertical(const Vector2 & point);

	/*!
	 *	@brief		"Snaps" the query point to a line parallel with the grid's x-axis (preserving the x-value).
	 *
	 *	@param		point		The 2D query point.
	 *	@returns	The point with the same x, but with the y-value of the nearest horizontal grid line.
	 */
	Vector2 snapHorizontal(const Vector2 & point);

	/*!
	 *	@brief		Streaming operator to write a reference grid out to a stream.
	 *
	 *	@param		out			The stream to write to.
	 *	@param		grid		The grid to write.
	 *	@returns	A reference to the stream.
	 */
	friend std::ostream & operator<<(std::ostream & out, const ReferenceGrid & grid);

protected:

	/*!
	 *	@brief	The minimum value along the x-axis spanned by this grid.
	 */
	float _originX;

	/*!
	 *	@brief	The minimum value along the y-axis spanned by this grid.
	 */
	float _originY;

	/*!
	 *	@brief	The span along the x-axis covered by the grid.
	 */
	float _width;

	/*!
	 *	@brief	The span along the y-axis covered by the grid.
	 */
	float _height;

	/*!
	 *	@brief	The number of minor lines found between major lines
	 */
	unsigned int _minorCount;

	/*!
	 *	@brief	The distance between major axis lines.  The first major line passes through
	 *			the origin.
	 */
	float _majorDist;

};

#endif	// __REFERENCE_GRID_H__