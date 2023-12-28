#include "FirmwareDetailGUI.hpp"
#include "ui_FirmwareDetailGUI.h"

FirmwareDetailGUI::FirmwareDetailGUI(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FirmwareDetailGUI)
{
    ui->setupUi(this);
    setWindowTitle("Test in progress");
}

FirmwareDetailGUI::~FirmwareDetailGUI()
{
}


void FirmwareDetailGUI::SetProgressBar(int value)
{
    ui->TestProgressBar->setValue(value);
}
void FirmwareDetailGUI::SetFirmwareVersionLabel(QString string)
{
    ui->FirmwareVersionLabelInput->setText(string);
}

void FirmwareDetailGUI::SetLanguageLabel(QString string)
{
    ui->LanguageLabelInput->setText(string);
}


void FirmwareDetailGUI::SetServiceInterfaceInfo(QPair<QString, int> info)
{
    ui->DeviceIDLabelInput->setText(info.first);

    switch(info.second)
    {
    case 0:
        ui->OperatingModeLabelInput->setText("Local");
        break;
    case 1:
        ui->OperatingModeLabelInput->setText("Local + Remote");
        break;
    default:
        ui->OperatingModeLabelInput->setText("Error!");
        break;
    }
}
