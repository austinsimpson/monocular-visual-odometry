#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

#include "Frame.h"
#include "FrameCorrespondence.h"

#include <opencv2/videoio.hpp>

#include <QFile>
#include <QTimer>

#include <QVector3D>
#include <Qt3DWindow>
#include <QEntity>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);

private:
	void build3dScene();
	void addTrackedCameraPoint(const QVector3D& point);
    void addTrackedWorldPoint(const QVector3D& point);

	Qt3DExtras::Qt3DWindow* _3dWindow;
	Qt3DCore::QEntity* _rootEntity;

	void loadVideo(const QString& name);
	void processNextFrame();
	void finalize();

	//Responsible for extracting key points out of image data
	QVector<Frame> _frames;

	//Correspondences store important data that is intrinsic to a change in frames, such as a list of matches between features,
	//essential, rotation, and translation matrices.
	QList<FrameCorrespondence> _correspondences;
	QTimer _timer;

	QVector<QVector3D> _translationByFrame;

	int _currentFrameIndex;
	int _numberOfFrames;

    cv::VideoCapture _videoCapture;
	QFile _trainSpeedsFile;
	QFile _trainToObservedFile;
};

#endif // MAINWINDOW_H
