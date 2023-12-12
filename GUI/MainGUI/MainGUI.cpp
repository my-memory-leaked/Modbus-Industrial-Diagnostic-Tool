#include <MainGUI.hpp>
#include <ui_MainGUI.h>

#include <QMessageBox>
#include <QModbusRtuSerialClient>
#include <QPixmap>

#include <ApplicationConstant.hpp>
#include <AddModbusDeviceGUI.hpp>
#include <ModbusStateMapper.hpp>
#include <ModbusTCPClient.hpp>

#include <LocalHostTest.hpp>
#include <JSONToDevicesConventer.hpp>
#include <ModbusStateMapper.hpp>
#include <ChangeModbusDeviceParameters.hpp>


MainGUI::MainGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainGUI)
{
    ui->setupUi(this);
    setWindowTitle(ApplicationConstant::MAIN_APPLICATION_NAME);
    connectSignalsAndSlots();

    _mbController = &Singleton<ModbusController>::GetInstance();
    (void)readDevicesFromFile();
    (void)loadAumaLogo();
    (void)loadDevicesStates();

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
        updateDevicesList();
    }
}

void MainGUI::handleInterfaceStateChange(const QString& deviceName, const QModbusDevice::State& newState)
{
    ModbusStrategy* interface = _mbController->GetInterfaceByName(deviceName);
    if (!interface) return; // No interface found for given device name

    if (deviceName == "Auma" && newState == QModbusDevice::State::ConnectedState)
    {
        ui->AumaConnectionState->setPixmap(GetConnectedImage());
    }
    else if(deviceName == "localhost" && newState == QModbusDevice::State::ConnectedState)
    {
        ui->LocalhostConnectionState->setPixmap(GetConnectedImage());
    }

    QString updatedInfo = createDeviceInfoString(interface);

    // Find the item in the list widget that corresponds to the changed device
    for (int i = 0; i < ui->DevicesListWidget->count(); ++i)
    {
        QListWidgetItem* item = ui->DevicesListWidget->item(i);
        if (item && item->text().contains("Device Name: " + deviceName))
        {
            item->setText(updatedInfo); // Update the text of the specific item
            break;
        }
    }
}
void MainGUI::handleAumaChangeSettingsButton()
{
    changeModbusDeviceParameters(_mbController->GetInterfaceByName("Auma"));
}

void MainGUI::handleAumaTestButton()
{
    /* TODO this! */
}

void MainGUI::handleLocalhostChangeSettingsButton()
{
    changeModbusDeviceParameters(_mbController->GetInterfaceByName("localhost"));
}

void MainGUI::handleLocalhostTestButton()
{
    /* TODO Refactor this! */
    LocalHostTest localhostTest;
    localhostTest.RunTest();
}

void MainGUI::connectSignalsAndSlots() const
{
    connect(ui->AddDevice, &QPushButton::clicked, this, &MainGUI::handleAddDeviceClick);

    connect(ui->AumaChangeSettingsButton, &QPushButton::clicked, this, &MainGUI::handleAumaChangeSettingsButton);
    connect(ui->AumaTestDeviceButton, &QPushButton::clicked, this, &MainGUI::handleAumaTestButton);

    connect(ui->LocalhostChangeSettingsButton, &QPushButton::clicked, this, &MainGUI::handleLocalhostChangeSettingsButton);
    connect(ui->LocalhostTestDeviceButton, &QPushButton::clicked, this, &MainGUI::handleLocalhostTestButton);
}

void MainGUI::readDevicesFromFile()
{
    auto& deviceConventer = JSONToDevicesConventer::GetInstance();
    auto deviceList = deviceConventer.FromJsonFile("JSON/Devices.json");
    for (auto device : deviceList)
    {
        if (device->GetDeviceName() == "Auma")
        {
            updateAumaDevice(device.get());
        }
        else if(device->GetDeviceName() == "localhost")
        {
            updateLocalhostDevice(device.get());
        }


        _mbController->AddInterface(std::move(device));
    }
    _mbController->ConnectAllInterfaces();
    updateDevicesList();
}

QString MainGUI::createDeviceInfoString(const ModbusStrategy* cInterface) const
{
    QString retVal;
    switch (cInterface->GetInterfaceType())
    {
    case ModbusStrategy::ModbusInterfaceType::TCP:
        retVal = QString("Device Name: %1 | IP Address: %2 | Port: %3 | State: %4")
                     .arg(cInterface->GetDeviceName(),
                          cInterface->GetConnectionParameters().GetIpAddress(),
                          QString::number(cInterface->GetConnectionParameters().GetPort()),
                          ModbusStateMapper::GetInstance().StateToString(cInterface->GetState()));
        break;


    case ModbusStrategy::ModbusInterfaceType::RTU:
        // Return RTU-specific info string
        break;

    case ModbusStrategy::ModbusInterfaceType::ASCII:
        // Return ASCII-specific info string
        break;
    }
    return retVal;
}

void MainGUI::updateDevicesList()
{
    ModbusStrategy* interface = nullptr;

    ui->DevicesListWidget->clear();
    for (const auto& entry : _mbController->GetInterfacesMap())
    {
        interface = entry.second.get();

        QString deviceInfo = createDeviceInfoString(interface);

        if(interface)
            connect(interface, &ModbusTCPClient::ModbusStateUpdated, this, &MainGUI::handleInterfaceStateChange);

        ui->DevicesListWidget->addItem(deviceInfo);
    }
}

void MainGUI::changeModbusDeviceParameters(ModbusStrategy* interface)
{
    ChangeModbusDeviceParameters modbusDialog(this, interface);
    if(modbusDialog.exec() == QDialog::Accepted)
    {   
        if (interface->GetDeviceName() == "Auma")
        {
            updateAumaDevice(interface);
        }
        else if(interface->GetDeviceName() == "localhost")
        {
            updateLocalhostDevice(interface);
        }
    }
}

void MainGUI::updateAumaDevice(const ModbusStrategy* cInterface)
{
    if (!cInterface->GetDeviceName().isEmpty())
        ui->AumaGroupBox->setTitle(cInterface->GetDeviceName());

    ui->AumaTypeInputLabel->setText("TCP");
    ui->AumaIPInputLabel->setText(cInterface->GetConnectionParameters().GetIpAddress());
    ui->AumaPortInputLabel->setText(QString::number(cInterface->GetConnectionParameters().GetPort()));
}

void MainGUI::updateLocalhostDevice(const ModbusStrategy* cInterface)
{
    if (!cInterface->GetDeviceName().isEmpty())
        ui->LocalhostGroupBox->setTitle(cInterface->GetDeviceName());

    ui->LocalhostTypeInputLabel->setText("TCP");
    ui->LocalhostIPInputLabel->setText(cInterface->GetConnectionParameters().GetIpAddress());
    ui->LocalhostPortInputLabel->setText(QString::number(cInterface->GetConnectionParameters().GetPort()));
}


void MainGUI::loadAumaLogo()
{
    QString appDirPath = QCoreApplication::applicationDirPath() + "/Images/LogoAuma.png";
    QPixmap aumaLogoPixmap(appDirPath);
    if (!aumaLogoPixmap.isNull())
    {
        /* Scale logo */
        QPixmap scaledPixmap = aumaLogoPixmap.scaled(ui->AumaLogoLabel->size(),
                                                     Qt::KeepAspectRatio, Qt::SmoothTransformation);

        ui->AumaLogoLabel->setPixmap(scaledPixmap);
    }
}

void MainGUI::loadDevicesStates()
{
    QPixmap disconnectedPixmap = GetDisconnectedImage();
    ui->AumaConnectionState->setPixmap(disconnectedPixmap);
    ui->LocalhostConnectionState->setPixmap(disconnectedPixmap);
}

QPixmap MainGUI::GetConnectedImage()
{
    QString appDirPath = QCoreApplication::applicationDirPath() + "/Images/ConnectedState.png";
    QPixmap connectedPixmap(appDirPath);
    /* Scale logo */
    QPixmap scaledPixmap = connectedPixmap.scaled(ui->AumaLogoLabel->size(),
                                                 Qt::KeepAspectRatio, Qt::SmoothTransformation);
    return scaledPixmap;
}

QPixmap MainGUI::GetDisconnectedImage()
{
    QString appDirPath = QCoreApplication::applicationDirPath() + "/Images/DisconnectedState.png";
    QPixmap disconnectedPixmap(appDirPath);
    /* Scale logo */
    QPixmap scaledPixmap = disconnectedPixmap.scaled(ui->AumaLogoLabel->size(),
                                                  Qt::KeepAspectRatio, Qt::SmoothTransformation);
    return scaledPixmap;
}
