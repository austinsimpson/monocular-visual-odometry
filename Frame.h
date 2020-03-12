#ifndef FRAME_H
#define FRAME_H


#include <opencv2/core.hpp>

using namespace cv;
using namespace std;

class Frame
{
public:
	enum KeyPointDetectionMethod
	{
		CvGoodPointsMethod,
		ORBMethod
	};

	Frame();
	Frame(const Frame& other);
	Frame& operator=(const Frame& other);

	const Mat& descriptors() const;

	void extractFeatures(const Mat& image, KeyPointDetectionMethod method = KeyPointDetectionMethod::ORBMethod);
	void setRecordedSpeed(float recordedSpeed);
	void setObservedSpeed(float observedSpeed);

	float recordedSpeed() const;
	float observedSpeed() const;

	const vector<KeyPoint>& extractedFeatures() const;

private:
	void useOrb(const Mat& img);
	void useCvGoodFeatures(const Mat& img);

	vector<Point> _corners;

	vector<KeyPoint> _extractedFeatures;
	Mat _descriptors;

	float _recordedSpeed;
	float _observedSpeed;
};

#endif // FRAME_H
