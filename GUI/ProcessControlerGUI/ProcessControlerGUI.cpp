#include "ProcessControlerGUI.hpp"
#include "ui_ProcessControlerGUI.h"

ProcessControlerGUI::ProcessControlerGUI(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProcessControlerGUI)
{
    ui->setupUi(this);
    setWindowTitle("Process controller diagnose");

}

ProcessControlerGUI::~ProcessControlerGUI()
{
}


void ProcessControlerGUI::SetActualPosition(QString val)
{
    ui->ActualPosLabelInput->setText(val);
}

void ProcessControlerGUI::SetActualTorque(QString val)
{
    ui->ActualTorqLabelInput->setText(val);
}

void ProcessControlerGUI::SetProgressBar(int val)
{
    ui->TestProgressBar->setValue(val);
}

void ProcessControlerGUI::SetActuatorRunningDiode(bool state)
{
    (state)?
        ui->ActuatorRunning->setText("Running") :
        ui->ActuatorRunning->setText("Idle");
}

void ProcessControlerGUI::SetWarningInfo(int value)
{
    ui->WarningLabel->setText(QString::number(value));
}

void ProcessControlerGUI::SetErrorInfo(int value)
{
    ui->ErrorLabel->setText(QString::number(value));
}

void ProcessControlerGUI::AddLog(QString log)
{
    ui->ErrorWarningList->addItem(log);
}
