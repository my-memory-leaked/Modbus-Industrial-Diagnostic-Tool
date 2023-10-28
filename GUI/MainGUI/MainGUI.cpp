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
        ui->tableWidget->setColumnCount(3);
        QStringList headers = {"Device Name", "IP Address", "Port"};
        ui->tableWidget->setHorizontalHeaderLabels(headers);

        _mbController->ListAllAvailableDevices(ui->tableWidget);
    }
}

void MainGUI::connectSignalsAndSlots() const
{
    connect(ui->AddDevice, &QPushButton::clicked, this, &MainGUI::handleAddDeviceClick);
}
