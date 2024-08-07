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

    void handleInterfaceStateChange(const QString& deviceName, const QModbusDevice::State& newState);

    void handleAumaChangeSettingsButton();
    void handleAumaTestButton();

    void handleLocalhostChangeSettingsButton();
    void handleLocalhostTestButton();

private:
    Ui::MainGUI *ui;

    ModbusController *_mbController;

    void connectSignalsAndSlots() const;
    void readDevicesFromFile();

    QString createDeviceInfoString(const ModbusStrategy* cInterface) const;

    void updateDevicesList();
    void changeModbusDeviceParameters(ModbusStrategy* interface);
    void updateAumaDevice(const ModbusStrategy* cInterface);
    void updateLocalhostDevice(const ModbusStrategy* cInterface);

    void loadAumaLogo();
    void loadDevicesStates();

    QPixmap GetConnectedImage();
    QPixmap GetDisconnectedImage();



    void hideDebugFeatures();
};
