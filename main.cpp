#include <QApplication>

#include "MainWindow.h"
#include "Frame.h"

#include <QVector>

int main(int argc, char *argv[])
{
	qRegisterMetaType<QVector<QPair<int, QPoint>>>();
	qRegisterMetaType<QVector<QVector3D>>();
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
