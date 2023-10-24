#pragma once

#include <QDialog>

namespace Ui {
class AddModbusDeviceGUI;
}

class AddModbusDeviceGUI : public QDialog
{
    Q_OBJECT

public:
    explicit AddModbusDeviceGUI(QWidget *parent = nullptr);
    ~AddModbusDeviceGUI();

private slots:
    void onConnectionInterfaceComboBox(int index);

private:
    Ui::AddModbusDeviceGUI *ui;
};
