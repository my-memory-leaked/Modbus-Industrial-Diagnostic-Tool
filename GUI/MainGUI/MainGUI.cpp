#include <MainGUI.hpp>
#include <ui_MainGUI.h>

#include <QMessageBox>
#include <QModbusRtuSerialClient>

#include <ApplicationConstant.hpp>
#include <AddModbusDeviceGUI.hpp>
#include <ModbusStateMapper.hpp>


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

        QString deviceInfo;
        switch(interface->GetInterfaceType())
        {
        case ModbusStrategy::ModbusInterfaceType::TCP:
            deviceInfo = QString("Device Name: %1 | IP Address: %2 | Port: %3 | State: %4")
                                .arg(interface->GetDeviceName(),
                                interface->GetConnectionParameters().GetIpAddress(),
                                QString::number(interface->GetConnectionParameters().GetPort()),
                                ModbusStateMapper::GetInstance().StateToString(interface->GetState()));
        break;
        case ModbusStrategy::ModbusInterfaceType::RTU:
            // You can set deviceInfo for RTU here if needed
            break;
        case ModbusStrategy::ModbusInterfaceType::ASCII:
            // You can set deviceInfo for ASCII here if needed
            break;
        }

        ui->DevicesListWidget->addItem(deviceInfo); // Add the information string to the list widget
    }
}
