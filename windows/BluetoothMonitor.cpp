#include "BluetoothMonitor.h"
#include "logger.h"

#include <QDebug>

// Windows stub implementation - DBus not available on Windows
// Bluetooth monitoring on Windows would use Windows Bluetooth APIs

BluetoothMonitor::BluetoothMonitor(QObject *parent)
    : QObject(parent)
{
    LOG_WARN("BluetoothMonitor: DBus-based monitoring not available on Windows");
    // Future implementation would use Windows Bluetooth APIs
}

BluetoothMonitor::~BluetoothMonitor()
{
    // Empty destructor
}

void BluetoothMonitor::registerDBusService()
{
    // Stub - no DBus on Windows
}

bool BluetoothMonitor::isAirPodsDevice(const QString &devicePath)
{
    Q_UNUSED(devicePath);
    // Stub - no DBus on Windows
    return false;
}

QString BluetoothMonitor::getDeviceName(const QString &devicePath)
{
    Q_UNUSED(devicePath);
    // Stub - no DBus on Windows
    return "Unknown";
}

bool BluetoothMonitor::checkAlreadyConnectedDevices()
{
    // Stub - no DBus on Windows
    // Future implementation would use Windows Bluetooth APIs
    LOG_DEBUG("BluetoothMonitor::checkAlreadyConnectedDevices not implemented on Windows");
    return false;
}