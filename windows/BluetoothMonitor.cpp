#include "BluetoothMonitor.h"
#include "logger.h"

// Windows stub implementation - DBus and BlueZ are not available on Windows
// This provides a no-op implementation that compiles but doesn't monitor Bluetooth devices
// Windows Bluetooth monitoring would require Windows Bluetooth API (like WinRT Bluetooth APIs)

BluetoothMonitor::BluetoothMonitor(QObject *parent)
    : QObject(parent)
{
    // Stub implementation for Windows - DBus and BlueZ not available
    LOG_DEBUG("BluetoothMonitor: DBus/BlueZ not available on Windows, using Qt Bluetooth APIs instead");
}

BluetoothMonitor::~BluetoothMonitor()
{
    // Stub implementation for Windows - no cleanup needed
}

bool BluetoothMonitor::checkAlreadyConnectedDevices()
{
    // Stub implementation for Windows - DBus and BlueZ not available
    // The main.cpp uses Qt's QBluetoothLocalDevice for device detection on Windows
    LOG_DEBUG("BluetoothMonitor: checkAlreadyConnectedDevices not implemented on Windows");
    return false;
}