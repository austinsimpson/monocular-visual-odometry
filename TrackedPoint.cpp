#include "TrackedPoint.h"

TrackedPoint::TrackedPoint() : 
	_startFrame(-1), 
	_endFrame(-1)
{

}

TrackedPoint::TrackedPoint(const TrackedPoint& other)
{
	copy(other);
}

TrackedPoint& TrackedPoint::operator=
(
	const TrackedPoint& other	
)
{
	copy(other);
	return *this;
}

void TrackedPoint::copy
(
	const TrackedPoint& other
)
{
	_startFrame = other._startFrame;
	_endFrame = other._endFrame;
	_indices = other._indices;
	_locations = other._locations;
}

void TrackedPoint::addLocation
(
	const QPoint& point
)
{
	_locations.append(point);
}

void TrackedPoint::setStartFrame
(
	int frame
)
{
	_startFrame = frame;
}

int TrackedPoint::startFrame() const
{
	return _startFrame;
}

void TrackedPoint::setEndFrame
(
	int frame
)
{
	_endFrame = frame;
}

int TrackedPoint::endFrame() const
{
	return _endFrame;
}