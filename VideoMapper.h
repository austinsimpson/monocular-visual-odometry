#pragma once

#include <QObject>

#include <QMap>
#include <QString>
#include <QVector>
#include <QVector3D>

#include <opencv2/videoio.hpp>

#include "FrameCorrespondence.h"
#include "TrackedPoint.h"

class VideoMapper : public QObject
{
	Q_OBJECT
public:
	VideoMapper(QObject* parent = nullptr);
	
	bool load(const QString& filePath);
	
	int totalFrameCount() const;
	int width() const;
	int height() const;


	//QVector<std::tuple<int, QPoint>> pointsForFrame(int frameNumber);

public slots:
	void startProcessing();

signals:
	void frameProcessed(int frameIndex);
	void trackedPointsUpdated(QVector<QPair<int, QPoint>> locationsWithIndices);
	void processingFinished();

private:
	void processNextFrame();
	void updateTrackedPoints();

	cv::VideoCapture _videoCapture;

	QVector<FrameCorrespondence> _correspondences;
	QVector<Frame> _frames;
	QVector<TrackedPoint> _trackedPoints;

	QMap<int, int> _trackedIndices;

	int _totalFrameCount;
	int _currentFrameIndex;
	int _width;
	int _height;
};

