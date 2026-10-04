#pragma once

#include <QString>
#include <QList>

// Detection of classic (BR/EDR) Bluetooth AirPods connections on Windows.
// Qt Bluetooth on Windows cannot open the L2CAP AAP control channel, and
// QBluetoothLocalDevice::connectedDevices() returns no names or service UUIDs,
// so we query the Win32 Bluetooth and Configuration Manager APIs directly.
namespace WinBt
{
    struct AppleAudioDevice
    {
        QString address;      // "AA:BB:CC:DD:EE:FF"
        QString name;         // Friendly name as shown in Windows
        quint16 productId = 0; // Apple Bluetooth product ID, e.g. 0x2027
        bool connected = false;
    };

    // All paired Bluetooth devices with Apple's vendor ID (0x004C).
    QList<AppleAudioDevice> pairedAppleDevices();
}
