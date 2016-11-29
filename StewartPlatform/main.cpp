#include "MainWindow.h"
#include <QDebug>
#include <QApplication>

extern double lr_angle;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	//CG27 cg;
	MainWindow w;
	//CG27 cg;
	w.show();
    //w.show();   
    return a.exec();
}
