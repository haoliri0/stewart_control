#include "ldata.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Ldata w;
	qDebug() << "main";
	std::cout << "main";
	CG27 cg;
	w.show();
	return a.exec();
}
