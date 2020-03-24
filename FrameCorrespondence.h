#ifndef FRAMECORRESPONDENCE_H
#define FRAMECORRESPONDENCE_H

#include <opencv2/core.hpp>

#include "Frame.h"

class FrameCorrespondence
{
public:
	FrameCorrespondence();
	FrameCorrespondence(const FrameCorrespondence& other);
	FrameCorrespondence& operator=(const FrameCorrespondence& other);

	bool isValid() const;
    const std::vector<cv::DMatch>& goodMatches() const;

    cv::Mat rotation() const;
    cv::Mat translation() const;
    cv::Mat worldCoords() const;

    const Frame* firstFrame() const;
    const Frame* secondFrame() const;

	void findMatches(const Frame& first, const Frame& second, int neighborCount = 2);
	void extrapolateMatrices();


private:
	const Frame* _firstFrame;
	const Frame* _secondFrame;

    std::vector<cv::DMatch> findGoodMatches(const std::vector<std::vector<cv::DMatch>>& allMatches);
    std::vector<cv::DMatch> _goodMatches;

    bool isSlopeAppropriate(const cv::Point& p1, const cv::Point& p2);

    cv::Mat _coords4d;

    cv::Mat _fundamentalMatrix;
    cv::Mat _essentialMatrix;
    cv::Mat _rotation;
    cv::Mat _translation;
};

#endif // FRAMECORRESPONDENCE_H
