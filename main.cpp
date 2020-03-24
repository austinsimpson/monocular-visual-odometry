#include <QApplication>

#include "MainWindow.h"
#include "Frame.h"

#include <QVector>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	qRegisterMetaType<QVector<QPair<int, QPoint>>>();
	return a.exec();
}
