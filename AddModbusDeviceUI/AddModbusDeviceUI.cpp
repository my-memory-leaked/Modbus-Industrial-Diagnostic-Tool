#include "AddModbusDeviceUI.hpp"
#include "ui_AddModbusDeviceUI.h"

AddModbusDeviceUI::AddModbusDeviceUI(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddModbusDeviceUI)
{
    ui->setupUi(this);
}

AddModbusDeviceUI::~AddModbusDeviceUI()
{
    delete ui;
}
