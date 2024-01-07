#pragma once

#include <QDialog>
#include <memory>

namespace Ui {
class ActuatorResponseGUI;
}

class ActuatorResponseGUI : public QDialog
{
    Q_OBJECT

public:
    explicit ActuatorResponseGUI(QWidget *parent = nullptr);
    ~ActuatorResponseGUI();

    void SetActualPosition(QString val);
    void SetActualTorque(QString val);
    void SetProgressBar(int val);

    void SetActuatorRunningDiode(bool state);
    void SetWarningInfo(int value);
    void SetErrorInfo(int value);

    void AddLog(QString log);
private:
    std::unique_ptr<Ui::ActuatorResponseGUI> ui;
};

