#include "usergui.hpp"
#include "./ui_usergui.h"
#include "ApplicationConstant/ApplicationConstant.hpp"
#include "QMessageBox"

#include "QModbusRtuSerialClient"

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

void UserGUI::on__modbusConnectPushButton_clicked()
{
    // TODO Open modbus connection

    QMessageBox msgWarning;
    msgWarning.setText("Slots Working");
    msgWarning.setIcon(QMessageBox::Warning);
    msgWarning.setWindowTitle("Caution");
    msgWarning.exec();

//    ModbusConnectionManager modbusConnectionManager(new QModbusRtuSerialClient);

}

