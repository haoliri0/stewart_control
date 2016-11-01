#include <QApplication>
#include <QWidget>
#include "controller.h"

int main(int argc,char ** argv)
{
    QApplication app(argc,argv);

    Controller m;
    m.show();

    return app.exec();
}
