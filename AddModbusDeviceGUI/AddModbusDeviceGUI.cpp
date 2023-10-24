#include "AddModbusDeviceGUI.hpp"
#include "ui_AddModbusDeviceGUI.h"

AddModbusDeviceGUI::AddModbusDeviceGUI(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddModbusDeviceGUI)
{
    ui->setupUi(this);
}

AddModbusDeviceGUI::~AddModbusDeviceGUI()
{
    delete ui;
}
