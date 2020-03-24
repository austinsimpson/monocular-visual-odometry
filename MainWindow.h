#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

#include "VideoMapper.h"

#include <QFile>
#include <QTimer>
#include <QThread>

class MainWindow : public QMainWindow, 
				   private Ui::MainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
private:
	QFile _trainSpeedsFile;
	QFile _trainToObservedFile;
	VideoMapper* _mapper;
	QThread _mapperThread;
};

#endif // MAINWINDOW_H
