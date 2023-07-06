#include "userui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UserUI w;
    w.show();
    return a.exec();
}
