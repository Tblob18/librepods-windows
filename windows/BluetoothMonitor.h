#ifndef BLUETOOTHMONITOR_H
#define BLUETOOTHMONITOR_H

#include <QObject>

// Windows stub implementation - DBus and BlueZ are not available on Windows
// This provides a no-op implementation that compiles but doesn't monitor Bluetooth devices
// Windows Bluetooth monitoring would require Windows Bluetooth API instead
class BluetoothMonitor : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothMonitor(QObject *parent = nullptr);
    ~BluetoothMonitor();

    bool checkAlreadyConnectedDevices();

signals:
    void deviceConnected(const QString &macAddress, const QString &deviceName);
    void deviceDisconnected(const QString &macAddress, const QString &deviceName);

private:
    // No implementation needed for Windows stub
};

#endif // BLUETOOTHMONITOR_H