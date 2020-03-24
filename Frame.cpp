#include "Frame.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

#include <QString>

const int kMaxFeatures = 3000;
static cv::Ptr<cv::ORB> globalOrb = cv::ORB::create(kMaxFeatures);

Frame::Frame() :
	_recordedSpeed(0),
	_width(0),
	_height(0)
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

	_width = other._width;
	_height = other._height;
}

Frame& Frame::operator=
(
	const Frame& other
)
{
	_recordedSpeed = other._recordedSpeed;
	_extractedFeatures = other._extractedFeatures;
	_descriptors = other._descriptors;
	return *this;
}

const cv::Mat& Frame::descriptors() const
{
	return _descriptors;
}

const std::vector<cv::KeyPoint>& Frame::extractedFeatures() const
{
	return _extractedFeatures;
}

void Frame::extractFeatures
(
	const cv::Mat& image,
	KeyPointDetectionMethod method
)
{
	if (image.empty())
		return;

	_width = image.cols;
	_height = image.rows;

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

void Frame::useOrb(const cv::Mat& image)
{
	globalOrb->detectAndCompute(image, cv::noArray(), _extractedFeatures, _descriptors, false);
}

void Frame::useCvGoodFeatures(const cv::Mat& image)
{
	_extractedFeatures.clear();

	cv::Ptr<cv::ORB> orb = cv::ORB::create(kMaxFeatures);
	cv::Mat intermediate = image.clone();
	cv::cvtColor(intermediate, intermediate, cv::COLOR_BGR2GRAY);

	std::vector<cv::Point> corners;
	cv::goodFeaturesToTrack(intermediate, corners, kMaxFeatures, 0.01, 7.0);

	for (cv::Point point : corners)
	{
		_extractedFeatures.push_back(cv::KeyPoint(point.x, point.y, 20));
	}

	globalOrb->compute(intermediate, _extractedFeatures, _descriptors);
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

int Frame::width() const
{
	return _width;
}

int Frame::height() const
{
	return _height;
}