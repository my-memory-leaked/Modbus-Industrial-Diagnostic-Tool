#include "MainGUI.hpp"
#include "ui_MainGUI.h"

#include "ApplicationConstant.hpp"
#include "QMessageBox"

#include "QModbusRtuSerialClient"

#include <AddModbusDeviceGUI.hpp>

ModbusTCPClient client;

// Singleton<ModbusController> UserGUI::_mbController = Singleton<ModbusController>::GetInstance();

MainGUI::MainGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainGUI)
{
    ui->setupUi(this);
    connectSignalsAndSlots();

    setWindowTitle(ApplicationConstant::MAIN_APPLICATION_NAME);
    _mbParams.SetDefaultValues();

}

MainGUI::~MainGUI()
{
    delete ui;
}

// void MainGUI::on__modbusConnectPushButton_clicked()
// {
//     SystemResult status = SystemResult::SYSTEM_ERROR;

//     QMessageBox msgWarning;
//     msgWarning.setText("Slots Working");
//     msgWarning.setIcon(QMessageBox::Warning);
//     msgWarning.setWindowTitle("Caution");
//     msgWarning.exec();

//     // _mbController
//     (void) client.SetConnectionParameters(_mbParams);
//     status = client.Connect();
//     //if(SystemResult::SYSTEM_OK != result)
//     // callback do ikonki połącznia zajeb debila przynajmniej jest spokój ale odklejka

// }


void MainGUI::handleAddDeviceClick()
{
    AddModbusDeviceGUI modbusDialog(this);
    if(modbusDialog.exec() == QDialog::Accepted)
    {
        // Use getters to get data from the dialog
        // QString parameter1 = modbusDialog.getParameter1();
        // Handle the rest of the parameters...

        // Now you can process or use the Modbus parameters as needed
    }
}

void MainGUI::connectSignalsAndSlots() const
{
    connect(ui->AddDevice, &QPushButton::clicked, this, &MainGUI::handleAddDeviceClick);
}
