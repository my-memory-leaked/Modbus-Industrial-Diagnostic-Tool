#pragma once

#include <QDialog>
#include <memory>

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
    void accept() override;

private:
    std::unique_ptr<Ui::AddModbusDeviceGUI> ui;

    static const QRegularExpression _ipAddressRegex;
};
