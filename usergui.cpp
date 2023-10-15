#include "usergui.hpp"
#include "./ui_usergui.h"
#include "ApplicationConstant/ApplicationConstant.hpp"
#include "QMessageBox"

#include "QModbusRtuSerialClient"

#include <ModbusTCPClient.hpp>
#include <ModbusConnectionParameters.hpp>

ModbusConnectionParameters params;
ModbusTCPClient client;


UserGUI::UserGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserGUI)
{
    ui->setupUi(this);
    setWindowTitle(ApplicationConstant::MAIN_APPLICATION_NAME);

    // ui->actionConnection->trigger(); // Get the connection to slot here
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

    params.SetIpAddress("127.0.0.1");
    params.SetPort(502);
    client.SetConnectionParameters(params);



    client.Connect();
//    ModbusConnectionManager modbusConnectionManager(new QModbusRtuSerialClient);

}

