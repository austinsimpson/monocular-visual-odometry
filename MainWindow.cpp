#include "MainWindow.h"

#include "Utilities.h"

#include "Frame.h"
#include "FrameCorrespondence.h"

#include <QDebug>

using namespace cv;
using namespace std;

MainWindow::MainWindow
(
	QWidget* parent
):
	QMainWindow(parent),
	_trainSpeedsFile(":/data/train.txt"),
    _trainToObservedFile("trainandobservations.csv"),
	_mapper(new VideoMapper)
{
	setupUi(this);

    QString videoPath = QCoreApplication::applicationDirPath() + "/train.mp4";
    
	_mapper->load(videoPath);
	_mapper->moveToThread(&_mapperThread);

	/*connect(_mapper, &VideoMapper::frameProcessed, [](int frameNumber) { 
		qDebug() << QString("Frame: %1\n").arg(frameNumber);
	});*/

	connect(_mapper, &VideoMapper::trackedPointsUpdated, _frameComparisonWidget, &FrameComparisonWidget::setLocationsWithIndices);

	_mapperThread.start();

	QMetaObject::invokeMethod(_mapper, &VideoMapper::startProcessing);
}

MainWindow::~MainWindow()
{
	if (_mapperThread.isRunning())
	{
		_mapperThread.quit();
		_mapperThread.wait();
	}
}