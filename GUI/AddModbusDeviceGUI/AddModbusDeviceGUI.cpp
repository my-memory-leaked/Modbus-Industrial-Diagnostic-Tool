#include "AddModbusDeviceGUI.hpp"
#include "ui_AddModbusDeviceGUI.h"

AddModbusDeviceGUI::AddModbusDeviceGUI(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddModbusDeviceGUI)
{
    ui->setupUi(this);

    connect(ui->ConnectionInterfaceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onConnectionInterfaceComboBox(int)));
}

AddModbusDeviceGUI::~AddModbusDeviceGUI()
{
    delete ui;
}


void AddModbusDeviceGUI::onConnectionInterfaceComboBox(int index)
{
    if (ui->ConnectionInterfaceComboBox->currentText() == "TCP")
    {
        ui->TCPInterfaceGroupBox->setVisible(true);
        // ui->groupBoxRTU->setVisible(false);
    }
    else if (ui->ConnectionInterfaceComboBox->currentText() == "RTU")
    {
        ui->TCPInterfaceGroupBox->setVisible(false);
        // ui->groupBoxRTU->setVisible(true);
    }
}
