#include "winbtdevices.h"

#include <QHash>
#include <QRegularExpression>

#include <windows.h>
#include <bluetoothapis.h>
#include <cfgmgr32.h>
#include <vector>

namespace
{
    QString formatAddress(ULONGLONG addr)
    {
        QString s;
        for (int i = 5; i >= 0; --i)
        {
            s += QString("%1").arg((addr >> (i * 8)) & 0xFF, 2, 16, QChar('0')).toUpper();
            if (i) s += ':';
        }
        return s;
    }

    // Map "AABBCCDDEEFF" -> Apple product ID, from BTHENUM device instance IDs such as
    // BTHENUM\{0000110B-...}_VID&0001004C_PID&2027\8&...&0&AABBCCDDEEFF_C00000000
    QHash<QString, quint16> appleProductIdsByAddress()
    {
        QHash<QString, quint16> result;
        ULONG len = 0;
        if (CM_Get_Device_ID_List_SizeW(&len, L"BTHENUM", CM_GETIDLIST_FILTER_ENUMERATOR) != CR_SUCCESS || len == 0)
            return result;

        std::vector<wchar_t> buf(len);
        if (CM_Get_Device_ID_ListW(L"BTHENUM", buf.data(), len, CM_GETIDLIST_FILTER_ENUMERATOR) != CR_SUCCESS)
            return result;

        static const QRegularExpression re(
            R"(_VID&[0-9A-F]{4}004C_PID&([0-9A-F]{4})\\.*&([0-9A-F]{12})_)",
            QRegularExpression::CaseInsensitiveOption);

        for (const wchar_t *p = buf.data(); *p; p += wcslen(p) + 1)
        {
            const auto m = re.match(QString::fromWCharArray(p));
            if (m.hasMatch())
                result.insert(m.captured(2).toUpper(), static_cast<quint16>(m.captured(1).toUShort(nullptr, 16)));
        }
        return result;
    }
}

QList<WinBt::AppleAudioDevice> WinBt::pairedAppleDevices()
{
    QList<AppleAudioDevice> devices;
    const auto productIds = appleProductIdsByAddress();
    if (productIds.isEmpty())
        return devices;

    BLUETOOTH_DEVICE_SEARCH_PARAMS params = {};
    params.dwSize = sizeof(params);
    params.fReturnAuthenticated = TRUE;
    params.fReturnRemembered = TRUE;
    params.fReturnConnected = TRUE;
    params.fReturnUnknown = FALSE;
    params.fIssueInquiry = FALSE;

    BLUETOOTH_DEVICE_INFO info = {};
    info.dwSize = sizeof(info);

    HBLUETOOTH_DEVICE_FIND find = BluetoothFindFirstDevice(&params, &info);
    if (!find)
        return devices;

    do
    {
        const QString address = formatAddress(info.Address.ullLong);
        const QString key = QString(address).remove(':');
        if (productIds.contains(key))
        {
            AppleAudioDevice d;
            d.address = address;
            d.name = QString::fromWCharArray(info.szName);
            d.productId = productIds.value(key);
            d.connected = info.fConnected != FALSE;
            devices.append(d);
        }
        info = {};
        info.dwSize = sizeof(info);
    } while (BluetoothFindNextDevice(find, &info));

    BluetoothFindDeviceClose(find);
    return devices;
}
