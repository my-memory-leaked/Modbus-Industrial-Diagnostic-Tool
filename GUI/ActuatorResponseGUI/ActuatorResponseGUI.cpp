#include <ActuatorResponseGUI.hpp>
#include <ui_ActuatorResponseGUI.h>

ActuatorResponseGUI::ActuatorResponseGUI(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ActuatorResponseGUI)
{
    ui->setupUi(this);
    setWindowTitle("Actuator response diagnose");

}

ActuatorResponseGUI::~ActuatorResponseGUI()
{
}


void ActuatorResponseGUI::SetActualPosition(QString val)
{
    ui->ActualPosLabelInput->setText(val);
}

void ActuatorResponseGUI::SetActualTorque(QString val)
{
    ui->ActualTorqLabelInput->setText(val);
}

void ActuatorResponseGUI::SetProgressBar(int val)
{
    ui->TestProgressBar->setValue(val);
}

void ActuatorResponseGUI::SetActuatorRunningDiode(bool state)
{
    (state)?
    ui->ActuatorRunning->setText("Running") :
    ui->ActuatorRunning->setText("Idle");
}

void ActuatorResponseGUI::SetWarningInfo(int value)
{
    ui->WarningLabel->setText(QString::number(value));
}

void ActuatorResponseGUI::SetErrorInfo(int value)
{
    ui->ErrorLabel->setText(QString::number(value));
}

void ActuatorResponseGUI::AddLog(QString log)
{
    ui->ErrorWarningList->addItem(log);
}
