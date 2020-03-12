#ifndef UTILITIES_H
#define UTILITIES_H

#include <QVector>
#include <QVector3D>

#include <opencv2/core.hpp>
#include <iostream>

QVector3D matToVector(const cv::Mat& mat)
{
	return QVector3D(mat.at<double>(0, 0), mat.at<double>(2,0), mat.at<double>(1,0));
}

QVector<QVector3D> matToVectorList(const cv::Mat& mat)
{
    QVector<QVector3D> result;
    int column = 0;
    while (column < mat.cols)
    {
        ++column;
    }
    return result;
}

#endif // UTILITIES_H

