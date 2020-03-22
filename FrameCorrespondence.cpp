#include "FrameCorrespondence.h"
#include "Utilities.h"

#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>

using namespace cv;
using namespace std;

const float kLoweRatioThreshold = 0.75f;
const float kSlopeThreshold = 0.4f;
const float kMatchDistanceThreshold = 32.0;

Ptr<DescriptorMatcher> globalMatcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE_HAMMING);

FrameCorrespondence::FrameCorrespondence() :
	_firstFrame(nullptr),
	_secondFrame(nullptr)
{

}


FrameCorrespondence::FrameCorrespondence
(
	const FrameCorrespondence& other
)
{
	_essentialMatrix = other._essentialMatrix;
	_fundamentalMatrix = other._fundamentalMatrix;
	_rotation = other._rotation;
	_translation = other._translation;

	_firstFrame = other._firstFrame;
	_secondFrame = other._secondFrame;
	_goodMatches = other._goodMatches;

	_coords4d = other._coords4d;
}

FrameCorrespondence& FrameCorrespondence::operator=
(
	const FrameCorrespondence& other
)
{
	_essentialMatrix = other._essentialMatrix;
	_fundamentalMatrix = other._fundamentalMatrix;
	_rotation = other._rotation;
	_translation = other._translation;

	_firstFrame = other._firstFrame;
	_secondFrame = other._secondFrame;
	_goodMatches = other._goodMatches;

	_coords4d = other._coords4d;
	return *this;
}

bool FrameCorrespondence::isValid() const
{
	return _firstFrame != nullptr && _secondFrame != nullptr;
}

const vector<DMatch>& FrameCorrespondence::goodMatches() const
{
	return _goodMatches;
}

void FrameCorrespondence::findMatches
(
	const Frame& first,
	const Frame& second,
	int neighborCount
)
{
	_firstFrame = &first;
	_secondFrame = &second;

	vector<vector<DMatch>> allMatches;
	globalMatcher->knnMatch(first.descriptors(), second.descriptors(), allMatches, neighborCount);

	_goodMatches = findGoodMatches(allMatches);

	allMatches.clear();
}

const Frame* FrameCorrespondence::firstFrame() const
{
	return _firstFrame;
}

const Frame* FrameCorrespondence::secondFrame() const
{
	return _secondFrame;
}

vector<DMatch> FrameCorrespondence::findGoodMatches
(
	const vector<vector<DMatch>>& allMatches
)
{
	vector<DMatch> goodMatches;

	//Lowes test
	for (size_t i = 0; i < allMatches.size(); i++)
	{
		if (allMatches[i][0].distance < kLoweRatioThreshold * allMatches[i][1].distance && allMatches[i][0].distance < kMatchDistanceThreshold)
		{
			goodMatches.push_back(allMatches[i][0]);
		}
	}

	return goodMatches;
}

void FrameCorrespondence::extrapolateMatrices()
{
	vector<Point> firstPoints;
	vector<Point> secondPoints;

    for (auto match : _goodMatches)
	{
		auto firstPoint = _firstFrame->extractedFeatures()[match.queryIdx].pt;
		auto secondPoint = _secondFrame->extractedFeatures()[match.trainIdx].pt;

		firstPoints.push_back(firstPoint);
		secondPoints.push_back(secondPoint);
	}

	if (firstPoints.size() > 14)
	{
        cv::Mat inliers;
		_coords4d = Mat::zeros(4, firstPoints.size(), CV_64F);
        _fundamentalMatrix = findFundamentalMat(secondPoints, firstPoints, FM_RANSAC, 3, 0.99);
        _essentialMatrix = findEssentialMat(secondPoints, firstPoints, 1.0, Point2d(0.0, 0.0), RANSAC, 0.999, 1.0, inliers);
		cv::recoverPose(_essentialMatrix, secondPoints, firstPoints, Mat::eye(3, 3, CV_64F), _rotation, _translation, 10.0, inliers, _coords4d);

		cout << "Fundamental matrix: " << _fundamentalMatrix << endl;
		cout << "Essential Matrix: " << _essentialMatrix << endl;
	}
    else
	{
		_rotation = Mat::eye(3, 3, CV_64F);
		_translation = Mat::zeros(3, 1, CV_64F);
	}

	firstPoints.clear();
	secondPoints.clear();
}

bool FrameCorrespondence::isSlopeAppropriate
(
	const Point& p1,
	const Point& p2
)
{
	return p1.x == p2.x ? false : abs((p1.y - p2.y) / (p1.x - p2.x)) < kSlopeThreshold;
}

Mat FrameCorrespondence::rotation() const
{
	return _rotation;
}

Mat FrameCorrespondence::translation() const
{
	return _translation;
}

Mat FrameCorrespondence::worldCoords() const
{
    return _coords4d;
}
