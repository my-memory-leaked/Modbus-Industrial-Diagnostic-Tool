#include <MainGUI.hpp>

#include <QApplication>
#include <Logger.hpp>

static auto* logger = &Singleton<Logger>::GetInstance();

int main(int argc, char *argv[])
{
    logger->LogInfo("[Main]", "Application starting!");
    QApplication application(argc, argv);
    MainGUI mainGUI;
    mainGUI.show();
    return application.exec();
}
