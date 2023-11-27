#include "myminecraft.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyGLWidget w;
	w.setWindowTitle("mineCraft");
    w.show();
    return a.exec();
}


