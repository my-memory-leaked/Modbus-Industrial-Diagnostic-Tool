#include <AddModbusDeviceGUI.hpp>
#include <ui_AddModbusDeviceGUI.h>

#include <QMessageBox>

#include <ApplicationConstant.hpp>
#include <ModbusConnectionParameters.hpp>
#include <ModbusController.hpp>
#include <ModbusTCPClient.hpp>
#include <Logger.hpp>

const QRegularExpression AddModbusDeviceGUI::_ipAddressRegex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");

static auto* logger = &Singleton<Logger>::GetInstance();

AddModbusDeviceGUI::AddModbusDeviceGUI(QWidget *parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui::AddModbusDeviceGUI>())
{
    ui->setupUi(this);
    setWindowTitle(ApplicationConstant::ADD_DEVICE_GUI_NAME);

    /* Logging GUI initialization */
    logger->LogInfo(CLASS_TAG, "Initializing Modbus Device GUI...");

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
    logger->LogDebug(CLASS_TAG, QString("Changed connection interface to %1").arg(ui->ConnectionInterfaceComboBox->currentText()));
}
void AddModbusDeviceGUI::accept()
{
    bool validInputs = true;  /* A flag to track input validity */

    if (ui->ConnectionInterfaceComboBox->currentText() == "TCP")
    {
        bool validInputs = true;

        if (ui->ConnectionInterfaceComboBox->currentText() == "TCP")
        {
            /* Validate the IP address */
            if (!_ipAddressRegex.match(ui->IPAddressLineEdit->text()).hasMatch())
            {
                validInputs = false;
                logger->LogWarning(CLASS_TAG, "Invalid IP Address input.");
                QMessageBox::warning(this, "Error", "Invalid IP Address");
            }

            /* Validate the port */
            bool isOk;
            int port = ui->PortLineEdit->text().toInt(&isOk);
            if (!isOk || port < 1 || port > 65535)
            {
                validInputs = false;
                logger->LogWarning(CLASS_TAG, "Invalid Port input.");
                QMessageBox::warning(this, "Error", "Invalid Port");
            }

            if (validInputs)
            {
                logger->LogInfo(CLASS_TAG, "Adding new Modbus TCP Interface...");

                ModbusConnectionParameters mbParams;
                mbParams.SetIpAddress(ui->IPAddressLineEdit->text());
                mbParams.SetPort(static_cast<quint16>(port));

                std::unique_ptr<ModbusTCPClient> mbTCP = std::make_unique<ModbusTCPClient>();

                QString deviceName = ui->DeviceNameLineEdit->text();
                mbTCP->SetDeviceName(deviceName);
                mbTCP->SetConnectionParameters(mbParams);

                /* Get the singleton instance of the Modbus Controller and add the interface */
                auto* mbController = &Singleton<ModbusController>::GetInstance();
                mbController->AddInterface(std::move(mbTCP));

                /* Connect to device */
                mbController->ConnectInterface(deviceName);

                logger->LogInfo(CLASS_TAG, QString("Successfully added Modbus TCP Interface: %1 for IP %2 and Port %3").arg(ui->DeviceNameLineEdit->text(), ui->IPAddressLineEdit->text()).arg(port));
            }
        }

    } /* Interfaces below may be implemented in future */
    else if (ui->ConnectionInterfaceComboBox->currentText() == "RTU")
    {
      logger->LogDebug(CLASS_TAG, "Selected Modbus RTU Interface (not implemented).");
    }
    else if(ui->ConnectionInterfaceComboBox->currentText() == "ASCII")
    {
     logger->LogDebug(CLASS_TAG, "Selected Modbus ASCII Interface (not implemented).");
    }

    /* Only accept and close the dialog if the inputs were valid */
    if (validInputs)
    {
        QDialog::accept();
    }
}
