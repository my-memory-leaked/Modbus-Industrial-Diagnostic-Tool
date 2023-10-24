#pragma once

#include <QMainWindow>
#include <QList>
#include <memory.h>

#include <SystemResult.hpp>
#include <ModbusConnectionParameters.hpp>
#include <ModbusController.hpp>
#include <ModbusTCPClient.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class UserGUI; }
QT_END_NAMESPACE

class UserGUI : public QMainWindow
{
    Q_OBJECT

public:
    UserGUI(QWidget *parent = nullptr);
    ~UserGUI();

private slots:
    void on__modbusConnectPushButton_clicked();

    void on__addDeviceButton_clicked();

private:
    Ui::UserGUI *ui;

    ModbusConnectionParameters _mbParams;
    static Singleton<ModbusController> _mbController;
};
