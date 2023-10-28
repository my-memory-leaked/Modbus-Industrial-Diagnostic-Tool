#include <AddModbusDeviceGUI.hpp>
#include <ui_AddModbusDeviceGUI.h>

#include <QMessageBox>

#include <ApplicationConstant.hpp>
#include <ModbusConnectionParameters.hpp>
#include <ModbusController.hpp>
#include <ModbusTCPClient.hpp>

const QRegularExpression AddModbusDeviceGUI::_ipAddressRegex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");

AddModbusDeviceGUI::AddModbusDeviceGUI(QWidget *parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui::AddModbusDeviceGUI>())
{
    ui->setupUi(this);
    setWindowTitle(ApplicationConstant::ADD_DEVICE_GUI_NAME);

    /* There won't be memory leak because we pass this object as parent */
    /* IP Address Validator */
    QRegularExpressionValidator* ipAddressValidator = new QRegularExpressionValidator(
        _ipAddressRegex,
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
    bool validInputs = true;  /* A flag to track input validity */

    if (ui->ConnectionInterfaceComboBox->currentText() == "TCP")
    {
        /* Validate the IP address */
        if (!_ipAddressRegex.match(ui->IPAddressLineEdit->text()).hasMatch())
        {
            validInputs = false;
            QMessageBox::warning(this, "Error", "Invalid IP Address");
        }

        /* Validate the port */
        bool isOk;
        int port = ui->PortLineEdit->text().toInt(&isOk);
        if (!isOk || port < 1 || port > 65535)
        {
            validInputs = false;
            QMessageBox::warning(this, "Error", "Invalid Port");
        }

        if (validInputs)
        {
            ModbusConnectionParameters mbParams;
            mbParams.SetIpAddress(ui->IPAddressLineEdit->text());
            mbParams.SetPort(static_cast<quint16>(port));

            std::unique_ptr<ModbusTCPClient> mbTCP = std::make_unique<ModbusTCPClient>();
            mbTCP->SetDeviceName(ui->DeviceNameLineEdit->text());
            mbTCP->SetConnectionParameters(mbParams);

            /* Get the singleton instance of the Modbus Controller and add the interface */
            auto* mbController = &Singleton<ModbusController>::GetInstance();
            mbController->AddInterface(std::move(mbTCP));
        }

    } /* Interfaces below may be implemented in future */
    else if (ui->ConnectionInterfaceComboBox->currentText() == "RTU")
    {
    }
    else if(ui->ConnectionInterfaceComboBox->currentText() == "ASCII")
    {
    }

    /* Only accept and close the dialog if the inputs were valid */
    if (validInputs)
    {
        QDialog::accept();
    }
}
