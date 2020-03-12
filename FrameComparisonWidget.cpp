#include "FrameComparisonWidget.h"
#include "Frame.h"
#include "framecorrespondence.h"

#include <opencv2/core.hpp>

#include <QPainter>

const int kFrameWidth = 640;
const int kFrameHeight = 480;

FrameComparisonWidget::FrameComparisonWidget
(
	QWidget* parent
):
	QWidget(parent),
	_leftFrame(nullptr),
	_rightFrame(nullptr),
	_correspondence(nullptr)
{

}

void FrameComparisonWidget::setFrames
(
	Frame* left,
	Frame* right,
	FrameCorrespondence* correspondence
)
{
	_leftFrame = left;
	_rightFrame = right;
	_correspondence = correspondence;
	update();
}

void FrameComparisonWidget::paintEvent
(
	QPaintEvent* event
)
{
	Q_UNUSED(event)
	QPainter painter;
	QRect painterWindow(0, 0, kFrameWidth, kFrameHeight);
	painter.begin(this);
	painter.setWindow(painterWindow);
	painter.fillRect(painterWindow, QColor("cornflower blue"));

	QPen pen = painter.pen();
	pen.setColor(Qt::black);
	pen.setWidth(5);
	painter.setPen(pen);

	/*if (_leftFrame != nullptr)
	{
		for (auto point : _leftFrame->extractedFeatures())
		{
			painter.drawEllipse(point.pt.x, point.pt.y, 2, 2);
		}
	}

	pen.setColor(Qt::gray);
	painter.setPen(pen);

	if (_rightFrame != nullptr)
	{
		for (auto point : _rightFrame->extractedFeatures())
		{
			painter.drawEllipse(point.pt.x, point.pt.y, 2, 2);
		}
	}*/

	if (_correspondence != nullptr && _correspondence->isValid())
	{
		//pen.setStyle(Qt::PenStyle::DashLine);
		pen.setWidth(1);
		pen.setColor(QColor(0, 100, 0, 100));
		painter.setPen(pen);
		for (auto match : _correspondence->goodMatches())
		{
			auto leftPoint = _leftFrame->extractedFeatures()[match.queryIdx].pt;
			auto rightPoint = _rightFrame->extractedFeatures()[match.trainIdx].pt;
			painter.drawLine(leftPoint.x, leftPoint.y, rightPoint.x, rightPoint.y);
		}
	}

	painter.end();
}
