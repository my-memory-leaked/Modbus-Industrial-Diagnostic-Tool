#include <MainGUI.hpp>
#include <ui_MainGUI.h>

#include <QMessageBox>
#include <QModbusRtuSerialClient>

#include <ApplicationConstant.hpp>
#include <AddModbusDeviceGUI.hpp>
#include <ModbusStateMapper.hpp>
#include <ModbusTCPClient.hpp>

#include <LocalHostTest.hpp>


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
        updateDevicesList();
    }
}

void MainGUI::handleTestButtonClick()
{
    ModbusStrategy* interface = _mbController->GetInterfaceByName("localhost");
    if(!interface)
        return;

    int startAddress = 0;
    int numberOfRegisters = 1;

    // QModbusDataUnit query(QModbusDataUnit::HoldingRegisters, startAddress, numberOfRegisters);
    // QModbusReply* result = interface->ReadData(query);

    // if(result)
    // {
    //     connect(result, &QModbusReply::finished, this, &MainGUI::handleModbusReply);
    // }
    LocalHostTest localhostTest;

    localhostTest.RunTest();
}

void MainGUI::handleModbusReply()
{
    QModbusReply* reply = qobject_cast<QModbusReply*>(sender());
    if(!reply)
        return;

    if(reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        for (uint i = 0; i < unit.valueCount(); i++)
        {
            int value = unit.value(i);
            qDebug() << "Register address:" << unit.startAddress() + i << "Value:" << value;
        }
    }
    else
    {
        qDebug() << "Error reading modbus registers:" << reply->errorString();
    }

    reply->deleteLater();  // Cleanup the reply object
}

void MainGUI::handleInterfaceStateChange(const QString& deviceName, const QModbusDevice::State& newState)
{
    ModbusStrategy* interface = _mbController->GetInterfaceByName(deviceName);
    if (!interface) return; // No interface found for given device name

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


void MainGUI::connectSignalsAndSlots() const
{
    connect(ui->AddDevice, &QPushButton::clicked, this, &MainGUI::handleAddDeviceClick);
    connect(ui->TestButton, &QPushButton::clicked, this, &MainGUI::handleTestButtonClick);
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
