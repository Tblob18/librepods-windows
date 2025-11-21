# Windows Build and Testing Guide

This document provides detailed instructions for building and testing LibrePods on Windows.

## Build System Requirements

### Required Tools
- **CMake** 3.16 or later
- **Qt 6.2+** with the following modules:
  - Qt6Base
  - Qt6Bluetooth
  - Qt6Multimedia
  - Qt6Declarative (QML)
  - Qt6Widgets
- **OpenSSL** 1.1.1 or 3.x
- **C++ Compiler** with C++17 support:
  - MSVC 2019/2022 (recommended)
  - MinGW-w64 GCC 8.1+

### Optional Tools
- **Qt Creator** - For IDE-based development
- **Visual Studio** - For debugging and advanced development

## Building from Source

### Method 1: Command Line (CMake)

1. Open PowerShell or Command Prompt
2. Navigate to the windows directory:
   ```powershell
   cd path\to\librepods-windows\windows
   ```

3. Create and enter build directory:
   ```powershell
   mkdir build
   cd build
   ```

4. Configure CMake (adjust Qt path as needed):
   ```powershell
   cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvc2019_64" -DCMAKE_BUILD_TYPE=Release
   ```

5. Build the project:
   ```powershell
   cmake --build . --config Release
   ```

6. The executable will be located at:
   ```
   build\Release\librepods-windows.exe
   ```

### Method 2: Qt Creator

1. Launch Qt Creator
2. File → Open File or Project
3. Select `windows/CMakeLists.txt`
4. Choose your Qt kit (ensure it includes Qt 6.x with Bluetooth support)
5. Click Configure Project
6. Build → Build Project "librepods-windows"
7. Run → Run to launch the application

## Deployment

### Creating a Deployable Package

1. After building, use Qt's deployment tool:
   ```powershell
   cd build\Release
   windeployqt librepods-windows.exe
   ```

2. This will copy all necessary Qt DLLs and plugins to the Release directory

3. Create an installer or zip the Release directory for distribution

## Testing

### Manual Testing Checklist

#### 1. Initial Launch
- [ ] Application starts without errors
- [ ] System tray icon appears
- [ ] No crash dialogs or error messages

#### 2. Bluetooth Connection
- [ ] Pair AirPods with Windows Bluetooth settings
- [ ] Launch LibrePods
- [ ] Application detects paired AirPods
- [ ] Battery levels display correctly (may take a few seconds)

#### 3. Noise Control
- [ ] Right-click tray icon
- [ ] Try switching between noise control modes:
  - [ ] Off
  - [ ] Transparency
  - [ ] Adaptive (if supported by your AirPods)
  - [ ] Noise Cancellation
- [ ] Verify mode changes take effect on AirPods

#### 4. Ear Detection
- [ ] Open Settings (from tray menu)
- [ ] Enable ear detection
- [ ] Play music
- [ ] Remove one AirPod - music should pause
- [ ] Put it back - music should resume
- [ ] Test "Pause when both removed" mode

#### 5. Conversational Awareness
- [ ] Enable from tray menu
- [ ] Play music
- [ ] Start speaking - volume should lower
- [ ] Stop speaking - volume should return to normal

#### 6. Power Management
- [ ] Put computer to sleep
- [ ] Wake computer
- [ ] Verify AirPods reconnect automatically
- [ ] Check that battery status updates

#### 7. Auto-Start
- [ ] Enable auto-start in settings
- [ ] Restart computer
- [ ] Verify LibrePods starts automatically
- [ ] Check that it starts minimized to tray

### Known Limitations

1. **Hearing Aid Features**: Not yet fully implemented on Windows
2. **Multi-device Connectivity**: Experimental support
3. **Some Windows versions**: May require running as administrator for full functionality

### Troubleshooting Build Issues

#### Qt Not Found
```
CMake Error: Could not find Qt6
```
**Solution**: Specify Qt path explicitly:
```powershell
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvc2019_64"
```

#### OpenSSL Not Found
```
CMake Error: Could not find OpenSSL
```
**Solution**: Install OpenSSL via vcpkg or manually:
```powershell
# Using vcpkg
vcpkg install openssl:x64-windows
# Then add to cmake:
cmake .. -DCMAKE_TOOLCHAIN_FILE="path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

#### Missing DLLs at Runtime
```
The code execution cannot proceed because Qt6Core.dll was not found
```
**Solution**: Run windeployqt or add Qt bin directory to PATH:
```powershell
$env:PATH += ";C:\Qt\6.x.x\msvc2019_64\bin"
```

#### Bluetooth Not Working
**Solution**: Ensure:
1. Bluetooth is enabled in Windows
2. AirPods are paired in Windows Bluetooth settings
3. Windows Bluetooth service is running
4. Application has necessary permissions

### Performance Testing

#### Memory Usage
- Expected idle memory: ~50-100 MB
- Expected with AirPods connected: ~100-150 MB
- Report if consistently exceeds 200 MB

#### CPU Usage
- Expected idle: <1%
- Expected during audio playback: 1-3%
- Report if consistently exceeds 10%

### Logging and Debugging

#### Enable Debug Logging
Launch with debug flag:
```powershell
.\librepods-windows.exe --debug
```

#### Log File Location
Logs are typically saved to:
```
%APPDATA%\AirPodsTrayApp\logs\
```

#### Viewing Logs
Use any text editor or PowerShell:
```powershell
Get-Content "$env:APPDATA\AirPodsTrayApp\logs\latest.log" -Tail 50 -Wait
```

## Contributing Test Results

When reporting issues, please include:
1. Windows version (e.g., Windows 10 21H2, Windows 11 23H2)
2. Qt version used for building
3. AirPods model
4. Build configuration (Release/Debug)
5. Relevant log excerpts
6. Steps to reproduce the issue

## Continuous Integration

Currently, CI/CD is not set up for Windows builds. Future plans include:
- Automated Windows builds via GitHub Actions
- Automated testing with virtual Bluetooth devices
- MSI installer generation
- Automatic deployment of releases

## Next Steps After Successful Build

1. Test with your AirPods Pro
2. Report any bugs or issues on GitHub
3. Suggest features or improvements
4. Contribute code improvements or bug fixes
5. Help with documentation

## Support

For build or testing issues:
- Open an issue: https://github.com/Tblob18/librepods-windows/issues
- Tag issues with `windows` label
- Provide full build output and system information
