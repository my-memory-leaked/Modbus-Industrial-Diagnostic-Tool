#include "usergui.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    UserGUI userGui;
    userGui.show();
    return application.exec();
}
