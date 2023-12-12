#ifndef CHANGEMODBUSDEVICEPARAMETERS_H
#define CHANGEMODBUSDEVICEPARAMETERS_H

#include <QDialog>
#include <ModbusStrategy.hpp>

namespace Ui {
class ChangeModbusDeviceParameters;
}

class ChangeModbusDeviceParameters : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeModbusDeviceParameters(QWidget *parent = nullptr);
    explicit ChangeModbusDeviceParameters(QWidget *parent = nullptr, ModbusStrategy *interface = nullptr);
    ~ChangeModbusDeviceParameters();

private slots:
    void onConnectionInterfaceComboBox(int index);
    void accept() override;

private:
    std::unique_ptr<Ui::ChangeModbusDeviceParameters> ui;
    static constexpr const char CLASS_TAG[] {"[ChangeModbusDeviceParameters]"};
    static const QRegularExpression _ipAddressRegex;
    ModbusStrategy *_interface;
};

#endif // CHANGEMODBUSDEVICEPARAMETERS_H
