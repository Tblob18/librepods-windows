# Windows Build and Testing Guide

This document provides detailed instructions for building and testing LibrePods on Windows.

## Build System Requirements

### Required Tools
- **Visual Studio Build Tools 2019/2022** (or full Visual Studio)
  - Download from [Visual Studio Downloads](https://visualstudio.microsoft.com/downloads/)
  - Select "Desktop development with C++" workload
  - Ensure "MSVC v143 - VS 2022 C++ x64/x86 build tools" is checked
  - Ensure "Windows 11 SDK" (or Windows 10 SDK) is checked
  - **Important**: Even if using only VS Code, you need these Build Tools for vcpkg to work
  
- **CMake** 3.16 or later
  - Download from [cmake.org/download](https://cmake.org/download/)
  - Select "Add CMake to the system PATH for all users" during installation
  - Verify installation: `cmake --version`
  
- **Qt 6.2+** with the following modules:
  - Qt6Base
  - Qt6Bluetooth
  - Qt6Multimedia
  - Qt6Declarative (QML)
  - Qt6Widgets
  
- **OpenSSL** 1.1.1 or 3.x
  
- **C++ Compiler** with C++17 support:
  - MSVC 2019/2022 (recommended, required for vcpkg)
  - MinGW-w64 GCC 8.1+ (alternative, requires different vcpkg triplet)

### Optional Tools
- **Qt Creator** - For IDE-based development
- **Visual Studio** - For debugging and advanced development

## Building from Source

### Method 1: Command Line (CMake with vcpkg)

**Recommended for users without Visual Studio IDE**

1. Install Visual Studio Build Tools:
   - Download from [https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022)
   - Run installer and select "Desktop development with C++"
   - Ensure MSVC compiler and Windows SDK are selected

2. Install CMake:
   - Download from [https://cmake.org/download/](https://cmake.org/download/)
   - During installation, select "Add CMake to the system PATH for all users"
   - Restart your terminal after installation

3. Set up vcpkg and install dependencies:
   ```powershell
   # Clone vcpkg (do this outside the librepods-windows directory)
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   
   # Bootstrap vcpkg
   .\bootstrap-vcpkg.bat
   
   # Update vcpkg to ensure you have the latest package definitions
   git pull
   
   # Install required libraries (this will take 30-60 minutes)
   .\vcpkg install qt6-base:x64-windows qt6-connectivity:x64-windows qt6-multimedia:x64-windows qt6-declarative:x64-windows openssl:x64-windows
   
   # Integrate vcpkg with CMake
   .\vcpkg integrate install
   ```
   
   **Note**: The vcpkg integrate command will output a CMake toolchain file path. Save this for the next step.

4. Build LibrePods:
   ```powershell
   # Navigate to librepods-windows/windows directory
   cd path\to\librepods-windows\windows
   
   # Create and enter build directory
   mkdir build
   cd build
   
   # Configure CMake (replace with your actual vcpkg path)
   cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
   
   # Build the project
   cmake --build . --config Release
   ```

5. The executable will be located at:
   ```
   build\Release\librepods-windows.exe
   ```

### Method 1b: Command Line (CMake with manual Qt installation)

**For users who prefer to install Qt manually**

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

#### vcpkg: Package Not Found (qt6-base, qt6-connectivity, etc.)
```
error: while looking for qt6-base:x64-windows:
C:\path\to\vcpkg\ports\qt6-base: error: qt6-base does not exist
```
**Solution**: Update vcpkg to get the latest package definitions:
```powershell
cd path\to\vcpkg
git pull
.\vcpkg update
# Then retry the install command
.\vcpkg install qt6-base:x64-windows qt6-connectivity:x64-windows qt6-multimedia:x64-windows qt6-declarative:x64-windows openssl:x64-windows
```

#### vcpkg: Unable to Find Visual Studio Instance
```
error: in triplet x64-windows: Unable to find a valid Visual Studio instance
Could not locate a complete Visual Studio instance
```
**Solution**: Install Visual Studio Build Tools:
1. Download from [https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022)
2. Run the installer
3. Select "Desktop development with C++"
4. Ensure "MSVC v143 - VS 2022 C++ x64/x86 build tools" is checked on the right panel
5. Ensure "Windows 11 SDK" (or Windows 10 SDK) is checked
6. Complete installation and restart your terminal

**Alternative**: If you want to avoid Microsoft compilers, install MinGW-w64 and use:
```powershell
.\vcpkg install qt6-base:x64-mingw-dynamic qt6-connectivity:x64-mingw-dynamic ...
```
Note: This may require additional project configuration changes.

#### CMake Not Recognized
```
cmake: The term 'cmake' is not recognized as a name of a cmdlet, function, script file, or executable program.
```
**Solution**: 
1. Install CMake from [https://cmake.org/download/](https://cmake.org/download/)
2. During installation, select "Add CMake to the system PATH for all users"
3. Restart PowerShell/Command Prompt
4. Verify: `cmake --version`

**Note**: vcpkg downloads its own copy of CMake internally, but this is NOT added to your PATH for manual use.

#### Qt Not Found
```
CMake Error: Could not find Qt6
```
**Solution 1 (if using vcpkg)**: Use the vcpkg toolchain file:
```powershell
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

**Solution 2 (if installed Qt manually)**: Specify Qt path explicitly:
```powershell
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvc2019_64"
```

#### OpenSSL Not Found
```
CMake Error: Could not find OpenSSL
```
**Solution 1 (recommended)**: Install OpenSSL via vcpkg:
```powershell
# Navigate to vcpkg directory
cd path\to\vcpkg
# Install OpenSSL
.\vcpkg install openssl:x64-windows
# Then configure cmake with toolchain file:
cmake .. -DCMAKE_TOOLCHAIN_FILE="path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

**Solution 2**: Install OpenSSL manually from [https://slproweb.com/products/Win32OpenSSL.html](https://slproweb.com/products/Win32OpenSSL.html) and add to PATH.

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
