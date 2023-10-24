#pragma once

#include <QDialog>

namespace Ui {
class AddModbusDeviceUI;
}

class AddModbusDeviceUI : public QDialog
{
    Q_OBJECT

public:
    explicit AddModbusDeviceUI(QWidget *parent = nullptr);
    ~AddModbusDeviceUI();

private:
    Ui::AddModbusDeviceUI *ui;
};
