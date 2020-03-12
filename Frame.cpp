#include "Frame.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

#include <QString>

using namespace cv;
using namespace std;

const int kMaxFeatures = 3000;
static Ptr<ORB> globalOrb = ORB::create(kMaxFeatures);

Frame::Frame() :
	_recordedSpeed(0)
{
}

Frame::Frame
(
	const Frame& other
)
{
	_recordedSpeed = other._recordedSpeed;
	_extractedFeatures = other._extractedFeatures;
	_descriptors = other._descriptors;
	_corners = other._corners;
}

Frame& Frame::operator=
(
	const Frame& other
)
{
	_recordedSpeed = other._recordedSpeed;
	_extractedFeatures = other._extractedFeatures;
	_descriptors = other._descriptors;
	_corners = other._corners;
	return *this;
}

const Mat& Frame::descriptors() const
{
	return _descriptors;
}

const vector<KeyPoint>& Frame::extractedFeatures() const
{
	return _extractedFeatures;
}

void Frame::extractFeatures
(
	const Mat& image,
	KeyPointDetectionMethod method
)
{
	if (image.empty())
		return;

	switch (method)
	{
	case ORBMethod:
		useOrb(image);
		break;
	case CvGoodPointsMethod:
		useCvGoodFeatures(image);
		break;
	default:
		break;
	}
}

void Frame::useOrb(const Mat& image)
{
	globalOrb->detectAndCompute(image, noArray(), _extractedFeatures, _descriptors, false);
}

void Frame::useCvGoodFeatures(const Mat& image)
{
	_extractedFeatures.clear();

	Ptr<ORB> orb = ORB::create(kMaxFeatures);
	Mat intermediate = image.clone();
	cv::cvtColor(intermediate, intermediate, COLOR_BGR2GRAY);
	cv::goodFeaturesToTrack(intermediate, _corners, kMaxFeatures, 0.01, 7.0);

	for (Point point : _corners)
	{
		_extractedFeatures.push_back(KeyPoint(point.x, point.y, 20));
	}

	globalOrb->compute(image, _extractedFeatures, _descriptors);
}

void Frame::setRecordedSpeed
(
	float recordedSpeed
)
{
	_recordedSpeed = recordedSpeed;
}

float Frame::recordedSpeed() const
{
	return _recordedSpeed;
}

void Frame::setObservedSpeed
(
	float observedSpeed
)
{
	_observedSpeed = observedSpeed;
}

float Frame::observedSpeed() const
{
	return _observedSpeed;

}
