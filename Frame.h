#ifndef FRAME_H
#define FRAME_H

#include <opencv2/core.hpp>

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

    const cv::Mat& descriptors() const;

    void extractFeatures(const cv::Mat& image, KeyPointDetectionMethod method = KeyPointDetectionMethod::ORBMethod);
	void setRecordedSpeed(float recordedSpeed);
	void setObservedSpeed(float observedSpeed);

	float recordedSpeed() const;
	float observedSpeed() const;

    const std::vector<cv::KeyPoint>& extractedFeatures() const;

private:
    void useOrb(const cv::Mat& img);
    void useCvGoodFeatures(const cv::Mat& img);

    std::vector<cv::Point> _corners;

    std::vector<cv::KeyPoint> _extractedFeatures;
    cv::Mat _descriptors;

	float _recordedSpeed;
	float _observedSpeed;
};

#endif // FRAME_H
