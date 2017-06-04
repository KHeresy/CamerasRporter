#include "CamerasRporter.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CamerasRporter w;
	w.show();
	return a.exec();
}
