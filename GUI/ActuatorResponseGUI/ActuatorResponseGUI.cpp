#include <ActuatorResponseGUI.hpp>
#include <ui_ActuatorResponseGUI.h>

ActuatorResponseGUI::ActuatorResponseGUI(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ActuatorResponseGUI)
{
    ui->setupUi(this);
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
    // (state)?
    // ui->ActuatorRunning->setPixmap() :
    // ui->ActuatorRunning->setPixmap();
}

void ActuatorResponseGUI::SetWarningDiode(bool state)
{
    // (state)?
    // ui->WarningLabel->setPixmap() :
    // ui->WarningLabel->setPixmap();
}

void ActuatorResponseGUI::SetErrorDiode(bool state)
{
    // (state)?
    // ui->ErrorLabel->setPixmap() :
    // ui->ErrorLabel->setPixmap();
}
