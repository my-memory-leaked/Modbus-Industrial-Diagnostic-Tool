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
    void SetWarningDiode(bool state);
    void SetErrorDiode(bool state);
private:
    std::unique_ptr<Ui::ActuatorResponseGUI> ui;
};

