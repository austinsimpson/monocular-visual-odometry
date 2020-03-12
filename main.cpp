#include <QApplication>

#include "MainWindow.h"
#include "Frame.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
