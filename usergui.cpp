#include "usergui.hpp"
#include "./ui_usergui.h"
#include "ApplicationConstant/ApplicationConstant.hpp"
#include "QMessageBox"

#include "QModbusRtuSerialClient"

#include <AddModbusDeviceUI.hpp>

ModbusTCPClient client;

// Singleton<ModbusController> UserGUI::_mbController = Singleton<ModbusController>::GetInstance();

UserGUI::UserGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserGUI)
{
    ui->setupUi(this);
    setWindowTitle(ApplicationConstant::MAIN_APPLICATION_NAME);
    _mbParams.SetDefaultValues();
}

UserGUI::~UserGUI()
{
    delete ui;
}

void UserGUI::on__modbusConnectPushButton_clicked()
{
    SystemResult status = SystemResult::SYSTEM_ERROR;

    QMessageBox msgWarning;
    msgWarning.setText("Slots Working");
    msgWarning.setIcon(QMessageBox::Warning);
    msgWarning.setWindowTitle("Caution");
    msgWarning.exec();

    // _mbController
    (void) client.SetConnectionParameters(_mbParams);
    status = client.Connect();
    //if(SystemResult::SYSTEM_OK != result)
    // callback do ikonki połącznia zajeb debila przynajmniej jest spokój ale odklejka



}

void UserGUI::on__addDeviceButton_clicked()
{

    AddModbusDeviceUI modbusDialog(this);
    if(modbusDialog.exec() == QDialog::Accepted)
    {
        // Use getters to get data from the dialog
        // QString parameter1 = modbusDialog.getParameter1();
        // Handle the rest of the parameters...

        // Now you can process or use the Modbus parameters as needed
    }

}

