#include "ReferenceGrid.h"

#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of ReferenceGrid
/////////////////////////////////////////////////////////////////////////////////////////////

ReferenceGrid::ReferenceGrid()
{
	_originX = _originY = 0.f;
	_width = _height = 1.f;
	_minorCount = 0;
	_majorDist = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////

ReferenceGrid::~ReferenceGrid()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////

void ReferenceGrid::setOrigin(float x, float y) {
	_originX = x;
	_originY = y;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void ReferenceGrid::setSize(float w, float h) {
	_width = w;
	_height = h;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void ReferenceGrid::setMinorCount(unsigned int count) {
	_minorCount = count;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void ReferenceGrid::setMajorDist(float dist) {
	_majorDist = dist;
}

/////////////////////////////////////////////////////////////////////////////////////////////

Vector2 ReferenceGrid::snap(const Vector2 & point) {
	Vector2 O(_originX, _originY);
	Vector2 offset = point - O;
	float cellSize = _majorDist / (_minorCount + 1);
	Vector2 mapped = offset / cellSize;
	float x = round(mapped.x()) * cellSize + _originX;
	float y = round(mapped.y()) * cellSize + _originY;
	return Vector2(x, y);
}

/////////////////////////////////////////////////////////////////////////////////////////////

Vector2 ReferenceGrid::snapVertical(const Vector2 & point) {
	float offset = point.x() - _originX;
	float cellSize = _majorDist / (_minorCount + 1);
	float mapped = offset / cellSize;
	float x = round(mapped) * cellSize + _originX;
	return Vector2(x, point.y());
}

/////////////////////////////////////////////////////////////////////////////////////////////

Vector2 ReferenceGrid::snapHorizontal(const Vector2 & point) {
	float offset = point.y() - _originY;
	float cellSize = _majorDist / (_minorCount + 1);
	float mapped = offset / cellSize;
	float y = round(mapped) * cellSize + _originY;
	return Vector2(point.x(), y);
}

/////////////////////////////////////////////////////////////////////////////////////////////

std::ostream & operator<<(std::ostream & out, const ReferenceGrid & grid) {
	out << "X origin:         " << grid._originX << "\n";
	out << "Y origin:         " << grid._originY << "\n";
	out << "Width:            " << grid._width << "\n";
	out << "Height:           " << grid._height << "\n";
	out << "Major distance:   " << grid._majorDist << "\n";
	out << "Minor line count: " << grid._minorCount;
	return out;
}