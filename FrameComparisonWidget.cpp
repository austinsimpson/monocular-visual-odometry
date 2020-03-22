#include "FrameComparisonWidget.h"
#include "Frame.h"
#include "FrameCorrespondence.h"

#include "Utilities.h"

#include <opencv2/core.hpp>

#include <QPainter>

const int kFrameWidth = 640;
const int kFrameHeight = 480;

FrameComparisonWidget::FrameComparisonWidget
(
	QWidget* parent
):
	QWidget(parent),
	_correspondence(nullptr)
{

}

void FrameComparisonWidget::setFrames
(
	FrameCorrespondence* correspondence
)
{
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

	if (_correspondence != nullptr && _correspondence->isValid())
	{
		for (auto match : _correspondence->goodMatches())
		{
			auto firstPoint = _correspondence->firstFrame()->extractedFeatures()[match.queryIdx];
			auto secondPoint = _correspondence->secondFrame()->extractedFeatures()[match.trainIdx];

			QColor color = colorForPoint(firstPoint.pt, _correspondence->firstFrame()->width(), _correspondence->firstFrame()->height());
			pen.setColor(color);
			painter.setPen(pen);

			painter.drawPoint(firstPoint.pt.x, firstPoint.pt.y);
			painter.drawPoint(secondPoint.pt.x, secondPoint.pt.y);

			painter.drawLine(firstPoint.pt.x, firstPoint.pt.y, secondPoint.pt.x, secondPoint.pt.y);
		}
	}

	painter.end();
}
