# LibrePods Windows Test Suite

This directory contains the test suite for LibrePods Windows, focusing on system requirements verification.

## Overview

The test suite verifies that your system meets all requirements necessary to run LibrePods on Windows, including:

- **Windows Version**: Windows 10 (build 17763/version 1809) or later, or Windows 11
- **Bluetooth Support**: Presence of Bluetooth hardware adapter
- **Bluetooth Status**: Whether Bluetooth is enabled
- **BLE Support**: Bluetooth Low Energy capability
- **Qt6 Version**: Qt 6.2 or later
- **Qt6 Modules**: Required modules (Quick, Widgets, Bluetooth, Multimedia)
- **OpenSSL**: SSL/TLS support for encrypted communication

## Building the Tests

### Prerequisites

The test suite requires the same dependencies as the main application:
- Visual Studio Build Tools (or Visual Studio)
- CMake 3.16 or later
- Qt6 (6.2 or later) with Bluetooth and Network modules
- OpenSSL

### Build Instructions

#### Option 1: Build tests separately

```powershell
# Navigate to the tests directory
cd windows/tests

# Create build directory
mkdir build
cd build

# Configure with vcpkg (replace with your vcpkg path)
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Or configure with manual Qt installation
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvc2019_64"

# Build
cmake --build . --config Release
```

#### Option 2: Build with main project

The tests can be optionally built alongside the main application by adding the tests subdirectory to the main CMakeLists.txt:

```cmake
# Add this to windows/CMakeLists.txt
option(BUILD_TESTS "Build test suite" OFF)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
```

Then build with:
```powershell
cmake .. -DBUILD_TESTS=ON -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build . --config Release
```

## Running the Tests

### Run all tests

```powershell
# From the build directory
ctest

# Or with verbose output
ctest --verbose

# Or in Release configuration on Windows
ctest -C Release
```

### Run specific test executable directly

```powershell
# From the build/Release directory (or build/Debug for debug builds)
cd Release
.\test_system_requirements.exe
```

### Understanding Test Output

The test suite provides detailed output for each check:

✓ = Requirement met (test passed)
✗ = Requirement not met (test failed)
⚠ = Warning (non-critical issue)

Example output:
```
=== LibrePods System Requirements Test Suite ===
Starting system requirements verification...

Testing Windows version...
  Detected OS: Windows 10
  Version: 10.0.19045
  Windows 10 Build Number: 19045
  Required: Build 17763 (1809) or later
  ✓ Windows version is supported

Testing Bluetooth support...
  Number of Bluetooth adapters found: 1
  ✓ Bluetooth adapter(s) detected
    Adapter 0: Intel Bluetooth Adapter
      Address: 12:34:56:78:9A:BC

Testing if Bluetooth is enabled...
  Bluetooth Host Mode: HostConnectable
  ✓ Bluetooth is enabled
    Mode: Connectable

...
```

## Test Cases

### test_system_requirements.cpp

This test file includes the following test cases:

1. **testWindowsVersion**: Verifies Windows version meets minimum requirements
2. **testBluetoothSupport**: Checks for Bluetooth hardware adapter
3. **testBluetoothEnabled**: Verifies Bluetooth is enabled
4. **testBLESupport**: Confirms BLE (Bluetooth Low Energy) capability
5. **testQt6Version**: Checks Qt version is 6.2 or later
6. **testQt6Modules**: Verifies all required Qt modules are present
7. **testOpenSSLSupport**: Confirms OpenSSL is available for encryption
8. **testBluetoothDeviceDiscovery**: Tests device discovery capability (informational)
9. **testSystemInfo**: Displays comprehensive system information summary

## Troubleshooting

### Tests fail to build

**Error**: `Qt6Test not found`
**Solution**: Install Qt6 Test module via vcpkg or Qt installer

**Error**: `Cannot open include file: 'bluetoothapis.h'`
**Solution**: Ensure Windows SDK is installed with Visual Studio Build Tools

### Tests fail to run

**Error**: Test executable crashes on startup
**Solution**: Ensure Qt DLLs are in PATH or run `windeployqt` on test executable

**Error**: "Bluetooth adapter not found"
**Solution**: This is expected if your PC doesn't have Bluetooth hardware. LibrePods requires Bluetooth to function.

### Bluetooth tests fail

**Warning**: "Bluetooth is currently disabled"
**Solution**: Enable Bluetooth in Windows Settings → Bluetooth & devices

**Error**: "No Bluetooth adapter found"
**Solution**: Ensure your PC has Bluetooth hardware. Check Device Manager → Bluetooth

### OpenSSL test fails

**Error**: "OpenSSL support is NOT available"
**Solution**: 
1. Install OpenSSL via vcpkg: `.\vcpkg install openssl:x64-windows`
2. Or manually install from [slproweb.com/products/Win32OpenSSL.html](https://slproweb.com/products/Win32OpenSSL.html)
3. Ensure OpenSSL DLLs are in PATH

## Adding New Tests

To add new test cases:

1. Create a new test file in the `tests/` directory (e.g., `test_bluetooth_connection.cpp`)
2. Add the test executable to `tests/CMakeLists.txt`:
   ```cmake
   add_executable(test_bluetooth_connection test_bluetooth_connection.cpp)
   target_link_libraries(test_bluetooth_connection PRIVATE Qt6::Test Qt6::Bluetooth)
   add_test(NAME BluetoothConnection COMMAND test_bluetooth_connection)
   ```
3. Follow Qt Test framework conventions (inherit from QObject, use private slots for tests)
4. Use `QVERIFY`, `QCOMPARE`, and other Qt Test macros for assertions

## Continuous Integration

These tests are designed to be run in CI/CD pipelines. Example GitHub Actions workflow:

```yaml
- name: Run Tests
  run: |
    cd windows/tests/build
    ctest --output-on-failure -C Release
```

## Contributing

When contributing new features to LibrePods Windows, please:
1. Add appropriate test cases for new system requirements
2. Update this README if new test categories are added
3. Ensure all tests pass before submitting pull requests

## License

This test suite is part of LibrePods and is licensed under the GNU General Public License v3.0.
See the LICENSE file in the repository root for details.
