#include <MainGUI.hpp>
#include <ui_MainGUI.h>

#include <QMessageBox>
#include <QModbusRtuSerialClient>

#include <ApplicationConstant.hpp>
#include <AddModbusDeviceGUI.hpp>

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
        _mbController->UpdateDeviceList(ui->DevicesListWidget);
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
