#include "myminecraft.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyGLWidget w;
	w.setWindowTitle("Homework 1");
    w.show();
    return a.exec();
}


