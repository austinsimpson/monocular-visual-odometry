#ifndef FRAMECORRESPONDENCE_H
#define FRAMECORRESPONDENCE_H

#include <opencv2/core.hpp>

#include "Frame.h"

using namespace cv;

class FrameCorrespondence
{
public:
	FrameCorrespondence();
	FrameCorrespondence(const FrameCorrespondence& other);
	FrameCorrespondence& operator=(const FrameCorrespondence& other);

	bool isValid() const;
	const vector<DMatch>& goodMatches() const;

	Mat rotation() const;
	Mat translation() const;

	void findMatches(const Frame& first, const Frame& second, int neighborCount = 2);
	void extrapolateMatrices();

private:
	const Frame* _firstFrame;
	const Frame* _secondFrame;

	vector<DMatch> findGoodMatches(const vector<vector<DMatch>>& allMatches);
	vector<DMatch> _goodMatches;

	bool isSlopeAppropriate(const Point& p1, const Point& p2);

	Mat _fundamentalMatrix;
	Mat _essentialMatrix;
	Mat _rotation;
	Mat _translation;
};

#endif // FRAMECORRESPONDENCE_H
