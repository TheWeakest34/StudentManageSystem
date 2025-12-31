#include "loginview.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginView l;
    l.show();
    return a.exec();
}
