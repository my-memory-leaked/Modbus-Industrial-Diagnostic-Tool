#include "MainGUI.hpp"
#include "ui_MainGUI.h"

#include "ApplicationConstant.hpp"
#include "QMessageBox"

#include "QModbusRtuSerialClient"

#include <AddModbusDeviceGUI.hpp>

MainGUI::MainGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainGUI)
{
    ui->setupUi(this);
    setWindowTitle(ApplicationConstant::MAIN_APPLICATION_NAME);
    connectSignalsAndSlots();

    _mbController = &Singleton<ModbusController>::GetInstance();

}

MainGUI::~MainGUI()
{
    delete ui;
}

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
