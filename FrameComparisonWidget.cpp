#include "FrameComparisonWidget.h"
#include "Frame.h"
#include "FrameCorrespondence.h"

#include "Utilities.h"

#include <opencv2/core.hpp>

#include <QPainter>

const int kFrameWidth = 640;
const int kFrameHeight = 480;

const float kFocalLength = 500.0;

FrameComparisonWidget::FrameComparisonWidget
(
	QWidget* parent
) :
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

void FrameComparisonWidget::setLocationsWithIndices
(
	const QVector<QPair<int, QPoint>> locationsWithIndices
)
{
	_locationsWithIndices = locationsWithIndices;
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


	for (auto locationWithIndex : _locationsWithIndices)
	{
		int pointIndex = locationWithIndex.first;
		QPoint point = locationWithIndex.second;

		QColor color = colorForPoint(point, 640, 480);
		pen.setColor(color);
		painter.setPen(pen);

		painter.drawPoint(point);
		painter.drawText(point, QString::number(pointIndex));
	}

	painter.end();
}
