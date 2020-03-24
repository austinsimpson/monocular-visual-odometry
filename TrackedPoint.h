#pragma once

#include <QVector>
#include <QPoint>

class TrackedPoint
{	
public:
	TrackedPoint();
	TrackedPoint(const TrackedPoint& other);

	TrackedPoint& operator= (const TrackedPoint& other);

	void addLocation(const QPoint& point);

	void setStartFrame(int frame);
	int startFrame() const;

	void setEndFrame(int frame);
	int endFrame() const;

private:
	void copy(const TrackedPoint& other);

	int _startFrame;
	int _endFrame;
	QVector<QPoint> _locations;
	QVector<int> _indices;
};

