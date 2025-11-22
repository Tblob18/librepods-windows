# LibrePods Windows Test Suite Guide

## Overview

The LibrePods Windows test suite is designed to help users and developers verify that their system meets all requirements for running LibrePods. This guide provides comprehensive information about the test suite.

## Test Suite Structure

```
windows/tests/
├── CMakeLists.txt                    # CMake configuration for tests
├── README.md                         # Basic test documentation
├── TESTING_GUIDE.md                  # This file
├── .gitignore                        # Ignore build artifacts
├── test_system_requirements.cpp      # Main system requirements test
└── test_bluetooth_example.cpp        # Example test (not built by default)
```

## Quick Start

### For Users: Check System Requirements

1. **Quick check without building anything:**
   ```powershell
   cd windows
   .\run_tests.ps1 -VcpkgPath "C:\path\to\vcpkg"
   ```

2. **If you already built the project:**
   ```powershell
   cd windows/build
   ctest -C Release --verbose
   ```

### For Developers: Run Tests During Development

1. **Configure with tests enabled:**
   ```powershell
   cmake .. -DBUILD_TESTS=ON -DCMAKE_TOOLCHAIN_FILE="path/to/vcpkg.cmake"
   ```

2. **Build and run:**
   ```powershell
   cmake --build . --config Release
   ctest -C Release
   ```

## Test Categories

### System Requirements Tests

**File:** `test_system_requirements.cpp`

These tests verify the fundamental requirements for LibrePods:

#### 1. Windows Version Test
- **Purpose:** Ensure Windows version is compatible
- **Pass Criteria:** Windows 10 build 17763+ (version 1809) or Windows 11
- **Why:** Earlier versions lack required Bluetooth features

#### 2. Bluetooth Support Test
- **Purpose:** Verify Bluetooth hardware is present
- **Pass Criteria:** At least one Bluetooth adapter detected
- **Why:** LibrePods requires Bluetooth to communicate with AirPods

#### 3. Bluetooth Enabled Test
- **Purpose:** Check if Bluetooth is currently enabled
- **Pass Criteria:** Bluetooth is powered on
- **Note:** This is a warning, not a failure - users can enable it

#### 4. BLE Support Test
- **Purpose:** Verify Bluetooth Low Energy support
- **Pass Criteria:** System supports BLE protocol
- **Why:** AirPods use BLE for communication

#### 5. Qt6 Version Test
- **Purpose:** Verify Qt version meets minimum requirements
- **Pass Criteria:** Qt 6.2 or later installed
- **Why:** Earlier Qt versions lack required features

#### 6. Qt6 Modules Test
- **Purpose:** Verify all required Qt modules are present
- **Pass Criteria:** Qt6 Quick, Widgets, Bluetooth, and Multimedia available
- **Why:** Each module provides essential functionality

#### 7. OpenSSL Support Test
- **Purpose:** Verify SSL/TLS support for encryption
- **Pass Criteria:** OpenSSL libraries available and functional
- **Why:** Required for encrypted communication with AirPods

#### 8. Bluetooth Device Discovery Test
- **Purpose:** Verify device scanning capability (informational)
- **Pass Criteria:** Can create device discovery agent
- **Note:** This is informational, not a hard requirement

#### 9. System Information Test
- **Purpose:** Display comprehensive system information
- **Pass Criteria:** Always passes (informational only)
- **Output:** OS version, Qt version, OpenSSL version, Bluetooth adapters

## Understanding Test Output

### Success Output
```
=== LibrePods System Requirements Test Suite ===
Starting system requirements verification...

Testing Windows version...
  Detected OS: Windows 11
  Version: 11.0.22000
  ✓ Windows version is supported

Testing Bluetooth support...
  Number of Bluetooth adapters found: 1
  ✓ Bluetooth adapter(s) detected
    Adapter 0: Intel Bluetooth Adapter
      Address: 12:34:56:78:9A:BC

...

=== Test Suite Completed ===
Totals: 9 passed, 0 failed, 0 skipped
```

### Failure Output
```
Testing Windows version...
  Detected OS: Windows 10
  Version: 10.0.17134
  Windows 10 Build Number: 17134
  Required: Build 17763 (1809) or later
  ✗ Windows version is NOT supported
FAIL!  : SystemRequirementsTest::testWindowsVersion() 
   Windows version must be Windows 10 (1809/build 17763) or later
```

## Common Issues and Solutions

### Issue: "Qt6Test not found"

**Cause:** Qt Test module not installed

**Solution:**
```powershell
# With vcpkg
cd path\to\vcpkg
.\vcpkg install qt6-base:x64-windows  # Includes Qt Test

# Or install manually from Qt installer
# Ensure "Qt Test" component is selected
```

### Issue: "No Bluetooth adapter found"

**Cause:** System lacks Bluetooth hardware

**Solutions:**
1. Check Device Manager → Bluetooth
2. Install Bluetooth drivers if present but not working
3. Use USB Bluetooth adapter if motherboard lacks built-in Bluetooth
4. Verify Bluetooth is enabled in BIOS/UEFI

### Issue: "OpenSSL support is NOT available"

**Cause:** OpenSSL libraries missing or not in PATH

**Solution:**
```powershell
# With vcpkg (recommended)
cd path\to\vcpkg
.\vcpkg install openssl:x64-windows

# Or download from: https://slproweb.com/products/Win32OpenSSL.html
# Install and add to PATH
```

### Issue: "Bluetooth is currently disabled"

**Cause:** Bluetooth adapter is powered off

**Solution:**
1. Open Settings → Bluetooth & devices
2. Toggle Bluetooth to "On"
3. Re-run tests

### Issue: Test crashes immediately

**Cause:** Missing Qt DLLs

**Solution:**
```powershell
# From build directory
cd tests\Release
C:\path\to\vcpkg\installed\x64-windows\tools\qt6\bin\windeployqt.exe test_system_requirements.exe

# Or add Qt to PATH
$env:PATH += ";C:\Qt\6.x.x\msvc2019_64\bin"
```

## Advanced Usage

### Running Specific Tests

Qt Test supports filtering:

```powershell
# Run only Windows version test
.\test_system_requirements.exe testWindowsVersion

# Run tests matching pattern
.\test_system_requirements.exe test*Bluetooth*
```

### Verbose Output

```powershell
# Show all qDebug output
.\test_system_requirements.exe -v2

# Show even more detail
.\test_system_requirements.exe -v3
```

### Output to File

```powershell
# Save results to file
.\test_system_requirements.exe -o results.txt

# XML format for CI
.\test_system_requirements.exe -o results.xml -xunitxml
```

### Integration with CTest

```powershell
# Run with CTest
ctest -C Release --verbose

# Run specific test
ctest -R SystemRequirements -C Release

# Generate JUnit XML for CI
ctest -C Release --output-junit results.xml
```

## Adding New Tests

### Step 1: Create Test File

Create a new `.cpp` file in `tests/` directory:

```cpp
#include <QtTest>

class MyNewTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testSomething();
};

void MyNewTest::initTestCase()
{
    qDebug() << "Setup";
}

void MyNewTest::cleanupTestCase()
{
    qDebug() << "Cleanup";
}

void MyNewTest::testSomething()
{
    QVERIFY2(true, "Test passed");
}

QTEST_MAIN(MyNewTest)
#include "my_new_test.moc"
```

### Step 2: Update CMakeLists.txt

Add your test to `tests/CMakeLists.txt`:

```cmake
add_executable(my_new_test
    my_new_test.cpp
)

target_link_libraries(my_new_test
    PRIVATE
    Qt6::Test
    Qt6::Bluetooth  # Add other Qt modules as needed
)

add_test(NAME MyNewTest COMMAND my_new_test)
```

### Step 3: Build and Run

```powershell
cd build
cmake ..
cmake --build . --config Release
ctest -R MyNewTest -C Release --verbose
```

## Continuous Integration

### GitHub Actions Example

```yaml
name: Test

on: [push, pull_request]

jobs:
  test:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install Qt
        uses: jurplel/install-qt-action@v3
        with:
          version: '6.5.0'
          
      - name: Configure
        run: |
          cd windows/tests
          mkdir build
          cd build
          cmake .. -DCMAKE_PREFIX_PATH=${{ env.Qt6_DIR }}
          
      - name: Build
        run: |
          cd windows/tests/build
          cmake --build . --config Release
          
      - name: Test
        run: |
          cd windows/tests/build
          ctest -C Release --output-on-failure
```

## Best Practices

### For Users

1. **Run tests before reporting issues** - Helps diagnose environment problems
2. **Include test output in bug reports** - Provides valuable system information
3. **Re-run after system updates** - Verify compatibility after Windows/driver updates

### For Developers

1. **Run tests before committing** - Catch issues early
2. **Add tests for new features** - Especially if they have system requirements
3. **Keep tests fast** - System requirement tests should complete in seconds
4. **Make tests informative** - Good error messages help users fix problems

## Troubleshooting Test Development

### MOC Errors

If you get errors about missing `#include "filename.moc"`:

1. Ensure you have `QTEST_MAIN(YourTestClass)` at the end of your file
2. Add `#include "yourfile.moc"` after QTEST_MAIN
3. Your test class must have the `Q_OBJECT` macro

### Linking Errors

If you get linker errors:

1. Verify all required Qt modules are listed in `target_link_libraries`
2. On Windows, ensure Windows-specific libraries are included in the `if(WIN32)` block
3. Check that Qt was built with the same compiler (MSVC vs MinGW)

### Runtime Crashes

If tests crash at runtime:

1. Run with debugger: `windbg test_system_requirements.exe`
2. Check Qt DLLs are present: `dumpbin /dependents test_system_requirements.exe`
3. Verify all DLLs are same architecture (all x64 or all x86)

## Resources

- [Qt Test Documentation](https://doc.qt.io/qt-6/qtest-overview.html)
- [CMake Test Documentation](https://cmake.org/cmake/help/latest/command/add_test.html)
- [Windows Bluetooth API](https://docs.microsoft.com/en-us/windows/win32/bluetooth/bluetooth-start-page)
- [Qt Bluetooth Documentation](https://doc.qt.io/qt-6/qtbluetooth-index.html)

## Contributing

When contributing tests:

1. Follow the existing code style
2. Add clear documentation for what the test checks
3. Include both positive and negative test cases when applicable
4. Update this guide if adding new test categories

## Support

If you encounter issues with the test suite:

1. Check this guide first
2. Review [tests/README.md](README.md) for basic information
3. Open an issue on GitHub with:
   - Test output (use `--verbose` flag)
   - System information from System Info test
   - Steps to reproduce the issue

## License

This test suite is part of LibrePods and is licensed under GPL v3.0.
See the LICENSE file in the repository root for details.
