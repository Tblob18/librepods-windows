/**
 * Example Bluetooth Connection Test
 * 
 * This is a template for adding additional tests to the test suite.
 * This particular example shows how to test basic Bluetooth connectivity.
 * 
 * To enable this test, add it to CMakeLists.txt:
 * 
 * add_executable(test_bluetooth_example
 *     test_bluetooth_example.cpp
 * )
 * target_link_libraries(test_bluetooth_example
 *     PRIVATE Qt6::Test Qt6::Bluetooth
 * )
 * add_test(NAME BluetoothExample COMMAND test_bluetooth_example)
 */

#include <QtTest>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QSignalSpy>

class BluetoothConnectionTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    // Example test cases
    void testBluetoothDeviceInfo();
    void testDeviceDiscoveryAgent();
    void testLocalDeviceCapabilities();

private:
    QBluetoothLocalDevice *m_localDevice = nullptr;
};

void BluetoothConnectionTest::initTestCase()
{
    qDebug() << "=== Bluetooth Connection Test Suite ===";
    m_localDevice = new QBluetoothLocalDevice(this);
}

void BluetoothConnectionTest::cleanupTestCase()
{
    if (m_localDevice) {
        delete m_localDevice;
        m_localDevice = nullptr;
    }
    qDebug() << "=== Test Suite Completed ===";
}

void BluetoothConnectionTest::testBluetoothDeviceInfo()
{
    qDebug() << "Testing Bluetooth device information retrieval...";
    
    QList<QBluetoothHostInfo> devices = QBluetoothLocalDevice::allDevices();
    
    QVERIFY2(!devices.isEmpty(), "At least one Bluetooth device should be present");
    
    for (const QBluetoothHostInfo &device : devices) {
        qDebug() << "  Device:" << device.name();
        qDebug() << "    Address:" << device.address().toString();
        
        // Verify device has valid name and address
        QVERIFY(!device.name().isEmpty());
        QVERIFY(!device.address().isNull());
    }
}

void BluetoothConnectionTest::testDeviceDiscoveryAgent()
{
    qDebug() << "Testing Bluetooth device discovery agent...";
    
    if (!m_localDevice->isValid()) {
        QSKIP("Local Bluetooth device is not valid");
    }
    
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    
    // Check that the agent can be created
    QVERIFY(discoveryAgent != nullptr);
    
    // Check initial state
    QCOMPARE(discoveryAgent->isActive(), false);
    
    qDebug() << "  Discovery agent created successfully";
    
    delete discoveryAgent;
}

void BluetoothConnectionTest::testLocalDeviceCapabilities()
{
    qDebug() << "Testing local Bluetooth device capabilities...";
    
    if (!m_localDevice->isValid()) {
        QSKIP("Local Bluetooth device is not valid");
    }
    
    // Get device address
    QBluetoothAddress address = m_localDevice->address();
    qDebug() << "  Local device address:" << address.toString();
    QVERIFY(!address.isNull());
    
    // Get device name
    QString name = m_localDevice->name();
    qDebug() << "  Local device name:" << name;
    QVERIFY(!name.isEmpty());
    
    // Get host mode
    QBluetoothLocalDevice::HostMode mode = m_localDevice->hostMode();
    qDebug() << "  Host mode:" << mode;
    
    // Verify we can query paired devices (this should not crash)
    QList<QBluetoothAddress> pairedDevices = m_localDevice->connectedDevices();
    qDebug() << "  Connected devices:" << pairedDevices.size();
    
    // This should always succeed if the device is valid
    QVERIFY(true);
}

// NOTE: This is an example/template file and is not built by default.
// To enable this test, uncomment the lines below and add it to CMakeLists.txt
// as described in the file header comments.

// Uncomment these two lines to enable this test:
// QTEST_MAIN(BluetoothConnectionTest)
// #include "test_bluetooth_example.moc"
