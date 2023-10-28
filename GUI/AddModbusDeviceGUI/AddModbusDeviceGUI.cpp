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

    /* IP Address Validator */
    QRegularExpressionValidator* ipAddressValidator = new QRegularExpressionValidator(
        QRegularExpression("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"),
        this
        );
    ui->IPAddressLineEdit->setValidator(ipAddressValidator);

    /* Port Validator */
    QIntValidator* portValidator = new QIntValidator(1, 65535, this);
    ui->PortLineEdit->setValidator(portValidator);

    connect(ui->ConnectionInterfaceComboBox, &QComboBox::currentIndexChanged, this, &AddModbusDeviceGUI::onConnectionInterfaceComboBox);
}

AddModbusDeviceGUI::~AddModbusDeviceGUI()
{
}


void AddModbusDeviceGUI::onConnectionInterfaceComboBox(int index)
{
    if (ui->ConnectionInterfaceComboBox->currentText() == "TCP")
    {
        ui->TCPInterfaceGroupBox->setVisible(true);
    }
    else if (ui->ConnectionInterfaceComboBox->currentText() == "RTU")
    {
        ui->TCPInterfaceGroupBox->setVisible(false);
    }
    else if (ui->ConnectionInterfaceComboBox->currentText() == "ASCII")
    {
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

    } /* Interfaces below may be implemented in future */
    else if (ui->ConnectionInterfaceComboBox->currentText() == "RTU")
    {
    }
    else if(ui->ConnectionInterfaceComboBox->currentText() == "ASCII")
    {
    }

    /* Close the ui */
    QDialog::accept();
}
