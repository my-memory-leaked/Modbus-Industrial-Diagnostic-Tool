#pragma once

#include <QDialog>

#include <memory>

namespace Ui {
class ProcessControlerGUI;
}

class ProcessControlerGUI : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessControlerGUI(QWidget *parent = nullptr);
    ~ProcessControlerGUI();

    void SetActualPosition(QString val);
    void SetActualTorque(QString val);
    void SetProgressBar(int val);

    void SetActuatorRunningDiode(bool state);
    void SetWarningInfo(int value);
    void SetErrorInfo(int value);

    void AddLog(QString log);

private:
    std::unique_ptr<Ui::ProcessControlerGUI> ui;
};
