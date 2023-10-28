#include <AddModbusDeviceGUI.hpp>
#include <ui_AddModbusDeviceGUI.h>

#include <ModbusConnectionParameters.hpp>
#include <ModbusController.hpp>
#include <ModbusTCPClient.hpp>

AddModbusDeviceGUI::AddModbusDeviceGUI(QWidget *parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui::AddModbusDeviceGUI>())
{
    ui->setupUi(this);

    connect(ui->ConnectionInterfaceComboBox, &QComboBox::currentIndexChanged, this, &AddModbusDeviceGUI::onConnectionInterfaceComboBox);
    // connect(ui->ButtonBox, &QDialogButtonBox::rejected, this , &QDialog::reject);
}

AddModbusDeviceGUI::~AddModbusDeviceGUI()
{
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
void AddModbusDeviceGUI::accept()
{
    if (ui->ConnectionInterfaceComboBox->currentText() == "TCP")
    {
        ModbusConnectionParameters mbParams;
        mbParams.SetIpAddress(ui->IPAddressLineEdit->text());
        mbParams.SetPort(static_cast<quint16>(ui->PortLineEdit->text().toInt()));

        std::unique_ptr<ModbusTCPClient> mbTCP = std::make_unique<ModbusTCPClient>();
        mbTCP->SetDeviceName(ui->DeviceNameLineEdit->text());
        mbTCP->SetConnectionParameters(mbParams);

        auto* mbController = &Singleton<ModbusController>::GetInstance();

        mbController->AddInterface(std::move(mbTCP));

    }
    else if (ui->ConnectionInterfaceComboBox->currentText() == "RTU")
    {
        // ui->TCPInterfaceGroupBox->setVisible(false);
        // ui->groupBoxRTU->setVisible(true);
    }

    // mbController->AddInterface()

}
