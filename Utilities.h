#ifndef UTILITIES_H
#define UTILITIES_H

#include <QColor>

#include <QVector>
#include <QVector3D>

#include <opencv2/core.hpp>
#include <iostream>

#include <QtCore/qmath.h>

inline QVector3D matToVector(const cv::Mat& mat)
{
	return QVector3D(mat.at<double>(0, 0), mat.at<double>(2,0), mat.at<double>(1,0));
}

inline QVector<QVector3D> matToVectorList(const cv::Mat& mat)
{
    QVector<QVector3D> result;
    int column = 0;
    while (column < mat.cols)
    {
        result.push_back(QVector3D(mat.at<double>(0, column), -mat.at<double>(1, column), mat.at<double>(2, column)));
        ++column;
    }
    return result;
}


template <typename T> inline void maskVector(std::vector<T>& vector, const cv::Mat& mask)
{
    auto iter = vector.begin();
    int index = 0;
    while (iter != vector.end())
    {
        if (mask.at<bool>(index, 0) == false)
        {
            vector.erase(iter);
        }
        index++;
        iter++;
    }
}

inline cv::Mat pointVectorToMat(const std::vector<cv::Point>& points)
{
    int columnIndex = 0;
    cv::Mat result(2, points.size(), CV_64F);
    for (auto point : points)
    {
        result.at<double>(0, columnIndex) = point.x;
        result.at<double>(1, columnIndex) = point.y;
        columnIndex++;
    }
    return result;
}

inline QColor colorForPoint(const cv::Point& point, int width = 0, int height = 0)
{
    //We're going to do the complex analysis trick where we find the angle of the point and map it to an hsv value;
    
    //First we translate to points relative to the center of the image. If we don't do this, all of the points will be in quadrant 1 (x > 0 && y > 0) and thus only have 1/4 of the color spectrum
    double adjustedX = (point.x - (width / 2));
    double adjustedY = (point.y - (height / 2));

    //We do atan2 because it's sensitive to signage.
    double angle = atan2(adjustedY, adjustedX);
    int hue = ((angle + M_PI) / (2 * M_PI)) * 360;
    
    return QColor::fromHsl(hue, 255, 126, 255);
}

#endif // UTILITIES_H

