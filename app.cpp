#include <QApplication>
#include <QFont>
#include <QDebug>
#include "viewer.h"

int main(int argc, char** argv)
{
	QApplication app(argc,argv);

	Viewer v;
	v.show();
	return app.exec();
}
