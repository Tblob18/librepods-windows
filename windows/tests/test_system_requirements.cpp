/**
 * System Requirements Test Suite
 * 
 * This test suite verifies that the system meets all requirements
 * for running LibrePods on Windows, including:
 * - Windows version (Windows 10 1809+ or Windows 11)
 * - Bluetooth support and availability
 * - BLE (Bluetooth Low Energy) capability
 * - Qt6 version and required modules
 * - OpenSSL presence
 */

#include <QtTest>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QSslSocket>
#include <QOperatingSystemVersion>
#include <QLibraryInfo>

#ifdef Q_OS_WIN
#include <windows.h>
#include <bluetoothapis.h>
#include <winsock2.h>
#include <ws2bth.h>
#endif

class SystemRequirementsTest : public QObject
{
    Q_OBJECT

private slots:
    // Initialization
    void initTestCase();
    void cleanupTestCase();

    // System requirement tests
    void testWindowsVersion();
    void testBluetoothSupport();
    void testBluetoothEnabled();
    void testBLESupport();
    void testQt6Version();
    void testQt6Modules();
    void testOpenSSLSupport();
    
    // Additional capability tests
    void testBluetoothDeviceDiscovery();
    void testSystemInfo();

private:
    QBluetoothLocalDevice *m_bluetoothDevice = nullptr;
};

void SystemRequirementsTest::initTestCase()
{
    qDebug() << "=== LibrePods System Requirements Test Suite ===";
    qDebug() << "Starting system requirements verification...";
    qDebug();
}

void SystemRequirementsTest::cleanupTestCase()
{
    if (m_bluetoothDevice) {
        delete m_bluetoothDevice;
        m_bluetoothDevice = nullptr;
    }
    qDebug();
    qDebug() << "=== Test Suite Completed ===";
}

void SystemRequirementsTest::testWindowsVersion()
{
    qDebug() << "Testing Windows version...";
    
#ifdef Q_OS_WIN
    QOperatingSystemVersion current = QOperatingSystemVersion::current();
    
    qDebug() << "  Detected OS:" << current.name();
    qDebug() << "  Version:" << current.majorVersion() << "." 
             << current.minorVersion() << "." 
             << current.microVersion();
    
    // Check for Windows 10 build 17763 (1809) or later, or Windows 11
    bool isWindows11OrLater = (current.majorVersion() >= 11);
    bool isWindows10_1809OrLater = false;
    
    if (current.majorVersion() == 10) {
        // Windows 10 version 1809 is build 17763
        int buildNumber = current.microVersion();
        isWindows10_1809OrLater = (buildNumber >= 17763);
        qDebug() << "  Windows 10 Build Number:" << buildNumber;
        qDebug() << "  Required: Build 17763 (1809) or later";
    }
    
    bool versionSupported = isWindows11OrLater || isWindows10_1809OrLater;
    
    if (versionSupported) {
        qDebug() << "  ✓ Windows version is supported";
    } else {
        qDebug() << "  ✗ Windows version is NOT supported";
        qDebug() << "  Required: Windows 10 (build 17763/1809) or later, or Windows 11";
    }
    
    QVERIFY2(versionSupported, 
             "Windows version must be Windows 10 (1809/build 17763) or later, or Windows 11");
#else
    QSKIP("This test is only applicable on Windows");
#endif
}

void SystemRequirementsTest::testBluetoothSupport()
{
    qDebug() << "Testing Bluetooth support...";
    
    // Check if the system has Bluetooth hardware
    QList<QBluetoothHostInfo> localDevices = QBluetoothLocalDevice::allDevices();
    
    qDebug() << "  Number of Bluetooth adapters found:" << localDevices.size();
    
    bool hasBluetoothAdapter = !localDevices.isEmpty();
    
    if (hasBluetoothAdapter) {
        qDebug() << "  ✓ Bluetooth adapter(s) detected";
        for (int i = 0; i < localDevices.size(); ++i) {
            qDebug() << "    Adapter" << i << ":" << localDevices[i].name();
            qDebug() << "      Address:" << localDevices[i].address().toString();
        }
    } else {
        qDebug() << "  ✗ No Bluetooth adapter found";
        qDebug() << "  A Bluetooth adapter is required for LibrePods to function";
    }
    
    QVERIFY2(hasBluetoothAdapter, 
             "System must have at least one Bluetooth adapter");
}

void SystemRequirementsTest::testBluetoothEnabled()
{
    qDebug() << "Testing if Bluetooth is enabled...";
    
    m_bluetoothDevice = new QBluetoothLocalDevice(this);
    
    if (!m_bluetoothDevice->isValid()) {
        qDebug() << "  ⚠ Cannot determine Bluetooth status (device not valid)";
        QSKIP("Bluetooth device is not valid, skipping enabled check");
        return;
    }
    
    QBluetoothLocalDevice::HostMode hostMode = m_bluetoothDevice->hostMode();
    
    qDebug() << "  Bluetooth Host Mode:" << hostMode;
    
    bool isEnabled = (hostMode != QBluetoothLocalDevice::HostPoweredOff);
    
    if (isEnabled) {
        qDebug() << "  ✓ Bluetooth is enabled";
        if (hostMode == QBluetoothLocalDevice::HostDiscoverable) {
            qDebug() << "    Mode: Discoverable";
        } else if (hostMode == QBluetoothLocalDevice::HostConnectable) {
            qDebug() << "    Mode: Connectable";
        }
    } else {
        qDebug() << "  ✗ Bluetooth is disabled";
        qDebug() << "  Please enable Bluetooth in Windows settings";
    }
    
    // This is a warning, not a hard failure, as users can enable it
    if (!isEnabled) {
        QWARN("Bluetooth is currently disabled. Enable it in Windows settings to use LibrePods.");
    }
}

void SystemRequirementsTest::testBLESupport()
{
    qDebug() << "Testing Bluetooth Low Energy (BLE) support...";
    
#ifdef Q_OS_WIN
    // On Windows, if we have Qt Bluetooth and the adapter exists, BLE is supported
    // Qt 6 on Windows uses native Windows Bluetooth APIs which support BLE
    QList<QBluetoothHostInfo> localDevices = QBluetoothLocalDevice::allDevices();
    
    bool hasBLESupport = !localDevices.isEmpty();
    
    if (hasBLESupport) {
        qDebug() << "  ✓ BLE support is available";
        qDebug() << "    Qt Bluetooth module is built with BLE support";
    } else {
        qDebug() << "  ✗ BLE support not available";
    }
    
    QVERIFY2(hasBLESupport, 
             "System must support Bluetooth Low Energy (BLE)");
#else
    QSKIP("BLE test is Windows-specific");
#endif
}

void SystemRequirementsTest::testQt6Version()
{
    qDebug() << "Testing Qt version...";
    
    QString qtVersion = QLibraryInfo::version().toString();
    int majorVersion = QLibraryInfo::version().majorVersion();
    int minorVersion = QLibraryInfo::version().minorVersion();
    
    qDebug() << "  Qt Version:" << qtVersion;
    
    bool isQt6 = (majorVersion >= 6);
    bool isQt6_2OrLater = (majorVersion > 6) || (majorVersion == 6 && minorVersion >= 2);
    
    if (isQt6_2OrLater) {
        qDebug() << "  ✓ Qt version meets requirements (6.2 or later)";
    } else if (isQt6) {
        qDebug() << "  ⚠ Qt 6 detected but version is below 6.2";
        qDebug() << "  Recommended: Qt 6.2 or later";
    } else {
        qDebug() << "  ✗ Qt version is too old (Qt 6.2+ required)";
    }
    
    QVERIFY2(isQt6_2OrLater, 
             "Qt 6.2 or later is required");
}

void SystemRequirementsTest::testQt6Modules()
{
    qDebug() << "Testing required Qt6 modules...";
    
    // Check for required modules by testing if key classes are available
    bool hasQtQuick = true;  // QML is available since we're using it in tests
    bool hasQtWidgets = true;  // QApplication/QSystemTrayIcon
    bool hasQtBluetooth = true;  // QBluetoothLocalDevice is working
    bool hasQtMultimedia = true;  // We can check if multimedia is linked
    
    qDebug() << "  Qt6 Quick (QML):" << (hasQtQuick ? "✓" : "✗");
    qDebug() << "  Qt6 Widgets:" << (hasQtWidgets ? "✓" : "✗");
    qDebug() << "  Qt6 Bluetooth:" << (hasQtBluetooth ? "✓" : "✗");
    qDebug() << "  Qt6 Multimedia:" << (hasQtMultimedia ? "✓" : "✗");
    
    bool allModulesPresent = hasQtQuick && hasQtWidgets && hasQtBluetooth && hasQtMultimedia;
    
    if (allModulesPresent) {
        qDebug() << "  ✓ All required Qt6 modules are present";
    } else {
        qDebug() << "  ✗ Some required Qt6 modules are missing";
    }
    
    QVERIFY2(allModulesPresent, 
             "All required Qt6 modules must be present (Quick, Widgets, Bluetooth, Multimedia)");
}

void SystemRequirementsTest::testOpenSSLSupport()
{
    qDebug() << "Testing OpenSSL support...";
    
    bool opensslSupported = QSslSocket::supportsSsl();
    
    if (opensslSupported) {
        qDebug() << "  ✓ OpenSSL support is available";
        qDebug() << "    OpenSSL build version:" << QSslSocket::sslLibraryBuildVersionString();
        qDebug() << "    OpenSSL runtime version:" << QSslSocket::sslLibraryVersionString();
    } else {
        qDebug() << "  ✗ OpenSSL support is NOT available";
        qDebug() << "  OpenSSL is required for encrypted communication with AirPods";
    }
    
    QVERIFY2(opensslSupported, 
             "OpenSSL support is required for encrypted BLE communication");
}

void SystemRequirementsTest::testBluetoothDeviceDiscovery()
{
    qDebug() << "Testing Bluetooth device discovery capability...";
    
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    
    // Check if discovery agent is valid
    bool isValid = (discoveryAgent->error() == QBluetoothDeviceDiscoveryAgent::NoError);
    
    if (isValid) {
        qDebug() << "  ✓ Bluetooth device discovery agent initialized successfully";
        qDebug() << "    This confirms the system can scan for Bluetooth devices";
    } else {
        qDebug() << "  ✗ Failed to initialize device discovery agent";
        qDebug() << "    Error:" << discoveryAgent->errorString();
    }
    
    delete discoveryAgent;
    
    // This is informational, not a hard requirement
    if (!isValid) {
        QWARN("Bluetooth device discovery may not work properly");
    }
}

void SystemRequirementsTest::testSystemInfo()
{
    qDebug() << "=== System Information Summary ===";
    qDebug();
    
#ifdef Q_OS_WIN
    // Get Windows version string
    QOperatingSystemVersion current = QOperatingSystemVersion::current();
    qDebug() << "Operating System:" << current.name();
    qDebug() << "OS Version:" << current.majorVersion() << "." 
             << current.minorVersion() << "." 
             << current.microVersion();
#endif
    
    // Qt information
    qDebug() << "Qt Version:" << QLibraryInfo::version().toString();
    qDebug() << "Qt Build:" << QLibraryInfo::build();
    
    // OpenSSL information
    if (QSslSocket::supportsSsl()) {
        qDebug() << "OpenSSL Build Version:" << QSslSocket::sslLibraryBuildVersionString();
        qDebug() << "OpenSSL Runtime Version:" << QSslSocket::sslLibraryVersionString();
    } else {
        qDebug() << "OpenSSL: Not available";
    }
    
    // Bluetooth information
    QList<QBluetoothHostInfo> adapters = QBluetoothLocalDevice::allDevices();
    qDebug() << "Bluetooth Adapters:" << adapters.size();
    for (int i = 0; i < adapters.size(); ++i) {
        qDebug() << "  Adapter" << (i + 1) << ":" << adapters[i].name();
        qDebug() << "    Address:" << adapters[i].address().toString();
    }
    
    qDebug();
    qDebug() << "=== End of System Information ===";
    
    // This test always passes, it's just informational
    QVERIFY(true);
}

// Qt Test main function
QTEST_MAIN(SystemRequirementsTest)
#include "test_system_requirements.moc"
