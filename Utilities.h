#ifndef UTILITIES_H
#define UTILITIES_H

#include <QVector3D>
#include <opencv2/core.hpp>

using namespace cv;

#include <iostream>

using namespace std;

QVector3D matToVector(const Mat& mat)
{
	return QVector3D(mat.at<double>(0, 0), mat.at<double>(2,0), mat.at<double>(1,0));
}

#endif // UTILITIES_H

