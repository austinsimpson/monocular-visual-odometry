#include "VideoMapper.h"

#include "Utilities.h"

VideoMapper::VideoMapper
(
	QObject* parent
): 
	QObject(parent)
{

}

bool VideoMapper::load
(
	const QString& fileName
)
{
	bool result = false;
	_currentFrameIndex = 0;
	if (_videoCapture.open(fileName.toStdString(), cv::CAP_ANY))
	{
		_totalFrameCount = static_cast<int>(_videoCapture.get(cv::CAP_PROP_FRAME_COUNT));
		_width = static_cast<int>(_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH));
		_height = static_cast<int>(_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT));

		auto focus = _videoCapture.get(cv::CAP_PROP_FOCUS);

		_frames.reserve(_totalFrameCount);
		_correspondences.reserve(_totalFrameCount - 1);

		result = true;
	}
	return result;
}

int VideoMapper::width() const
{
	return _width;
}

int VideoMapper::height() const
{
	return _height;
}

int VideoMapper::totalFrameCount() const
{
	return _totalFrameCount;
}

void VideoMapper::startProcessing()
{
	if (_videoCapture.isOpened())
	{
		while (_currentFrameIndex < _totalFrameCount)
		{
			processNextFrame();
			_currentFrameIndex++;
		}
		_videoCapture.release();
	}
}

void VideoMapper::processNextFrame()
{
	cv::Mat currentVideoFrame;

	_videoCapture >> currentVideoFrame;

	if (currentVideoFrame.empty() == false)
	{
		Frame frame;
		frame.extractFeatures(currentVideoFrame);
		_frames.append(frame);

		if (_frames.count() > 1)
		{
			FrameCorrespondence correspondence;
			correspondence.findMatches(_frames[_frames.count() - 2], _frames[_frames.count() - 1]);
			correspondence.extrapolateMatrices();
			_correspondences.append(correspondence);
		}

		if (_correspondences.count() > 1)
		{
			updateTrackedPoints();
		}

		emit frameProcessed(_currentFrameIndex);
	}
}

void VideoMapper::updateTrackedPoints()
{
	if (_correspondences.count() > 1)
	{
		auto firstMatches = (_correspondences.end() - 2)->goodMatches();
		auto secondMatches = (_correspondences.end() - 1)->goodMatches();

		auto firstFrame = (_frames.end() - 3);
		auto middleFrame = (_frames.end() - 2);
		auto lastFrame = (_frames.end() - 1);

		//We have two sets of matches. One set is from the matches in the first and second frame, and the second set is the matches from the second and third frame.
		//The goal is to find out if points are getting rematched in the second set of matches. Recall that a dmatch is a data structure containing point indices.  
		std::sort(firstMatches.begin(), firstMatches.end(), [](const cv::DMatch& firstMatch, const cv::DMatch& secondMatch)
		{
				return firstMatch.trainIdx < secondMatch.trainIdx;
		});

		std::sort(secondMatches.begin(), secondMatches.end(), [](const cv::DMatch& firstMatch, const cv::DMatch& secondMatch)
		{
				return firstMatch.queryIdx < secondMatch.queryIdx;
		});

		QMap<int, int> newIndicesMap;
		QVector<QPair<int, QPoint>> locationsWithIndices;

		for (auto secondMatch : secondMatches)
		{
			auto firstMatchIter = std::find_if(firstMatches.begin(), firstMatches.end(), [secondMatch](const cv::DMatch& firstMatch)
			{
				return firstMatch.trainIdx == secondMatch.queryIdx;
			});

			//If it's not the last iterator, it means that the point was matched again between the second and third frame.
			if (firstMatchIter != firstMatches.end())
			{
				auto firstMatch = *firstMatchIter;

				auto firstLocation = firstFrame->extractedFeatures()[firstMatch.queryIdx];
				auto secondLocation = middleFrame->extractedFeatures()[secondMatch.queryIdx];
				auto thirdLocation = lastFrame->extractedFeatures()[secondMatch.trainIdx];

				if (_trackedIndices.contains(firstMatch.trainIdx) == false)
				{
					TrackedPoint trackedPoint;
					trackedPoint.setStartFrame(_currentFrameIndex);
					trackedPoint.addLocation(QPoint(firstLocation.pt.x, firstLocation.pt.y));
					trackedPoint.addLocation(QPoint(secondLocation.pt.x, secondLocation.pt.y));

					_trackedPoints.append(trackedPoint);
					_trackedIndices[firstMatch.trainIdx] = _trackedPoints.count() - 1;
				}

				QPoint locationToAdd(thirdLocation.pt.x, thirdLocation.pt.y);

				int trackedPointIndex = _trackedIndices[firstMatch.trainIdx];

				_trackedPoints[trackedPointIndex].addLocation(locationToAdd);
				newIndicesMap[secondMatch.trainIdx] = trackedPointIndex;

				auto locationWithIndex = qMakePair(trackedPointIndex, locationToAdd);
				locationsWithIndices.push_back(locationWithIndex);
			}
		}

		_trackedIndices = newIndicesMap;
		emit trackedPointsUpdated(locationsWithIndices);
	}
}

