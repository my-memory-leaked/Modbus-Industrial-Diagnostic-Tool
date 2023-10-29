#pragma once

#include <QMainWindow>

#include <SystemResult.hpp>
#include <ModbusConnectionParameters.hpp>
#include <ModbusController.hpp>
#include <ModbusTCPClient.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainGUI; }
QT_END_NAMESPACE

class MainGUI : public QMainWindow
{
    Q_OBJECT

public:
    MainGUI(QWidget *parent = nullptr);
    ~MainGUI();

private slots:

    void handleAddDeviceClick();
    void handleTestButtonClick();

private:
    Ui::MainGUI *ui;

    ModbusController *_mbController;


    void connectSignalsAndSlots() const;
};
