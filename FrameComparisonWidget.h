#ifndef VIDEOFRAMEWIDGET_H
#define VIDEOFRAMEWIDGET_H

#include <QWidget>
#include <QPoint>

class Frame;
class FrameCorrespondence;

class FrameComparisonWidget : public QWidget
{
	Q_OBJECT
public:
	FrameComparisonWidget(QWidget* parent = nullptr);

	virtual void paintEvent(QPaintEvent* event) override;

	void setFrames(FrameCorrespondence* correspondence);

public slots:
	void setLocationsWithIndices(const QVector<QPair<int, QPoint>> locationsWithIndices);

private:
	FrameCorrespondence* _correspondence;
	QVector<QPair<int, QPoint>> _locationsWithIndices;
};

#endif // VIDEOFRAMEWIDGET_H
