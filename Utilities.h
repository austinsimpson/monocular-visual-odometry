#ifndef UTILITIES_H
#define UTILITIES_H

#include <QVector>
#include <QVector3D>

#include <opencv2/core.hpp>
#include <iostream>

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
        result.push_back(QVector3D(mat.at<double>(0, column), mat.at<double>(1, column), mat.at<double>(2, column)) / mat.at<double>(3, column));
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

#endif // UTILITIES_H

