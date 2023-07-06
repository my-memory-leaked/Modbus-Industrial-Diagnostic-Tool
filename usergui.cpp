#include "usergui.hpp"
#include "./ui_usergui.h"
#include "ApplicationConstant/ApplicationConstant.hpp"

UserGUI::UserGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserGUI)
{
    ui->setupUi(this);
    setWindowTitle(ApplicationConstant::MAIN_APPLICATION_NAME);

}

UserGUI::~UserGUI()
{
    delete ui;
}

