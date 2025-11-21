# LibrePods Windows

![LibrePods Logo](/imgs/banner.png)

A native Windows application to control your AirPods, with support for:

- Noise Control modes (Off, Transparency, Adaptive, Noise Cancellation)
- Conversational Awareness
- Battery monitoring
- Auto play/pause on ear detection
- Hearing Aid features
- Seamless handoff between Android and Windows

## Prerequisites

### Required Software

1. **Visual Studio Build Tools** (Required for vcpkg)
   - Download from [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022)
   - During installation, select "Desktop development with C++"
   - Ensure "MSVC v143 - VS 2022 C++ x64/x86 build tools" is checked
   - Ensure "Windows 11 SDK" (or Windows 10 SDK) is checked
   - **Note**: You do NOT need the full Visual Studio IDE, just the Build Tools

2. **CMake** - Version 3.16 or later
   - Download from [cmake.org/download](https://cmake.org/download/)
   - During installation, select "Add CMake to the system PATH for all users"
   - Restart your terminal after installation

3. **Qt6** - Version 6.2 or later (installed via vcpkg or manually)
   - Qt6 Base
   - Qt6 Bluetooth
   - Qt6 Multimedia
   - Qt6 Declarative (QML)
   - Qt6 Widgets

4. **OpenSSL** - For cryptographic operations
   - Installed via vcpkg (recommended)

### Installation Options

#### Option 1: Using vcpkg (Recommended)

```powershell
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Update vcpkg to get the latest package definitions
git pull

# Install Qt6 and OpenSSL (this will take a while as it builds from source)
.\vcpkg install qt6-base:x64-windows qt6-connectivity:x64-windows qt6-multimedia:x64-windows qt6-declarative:x64-windows openssl:x64-windows

# Integrate with CMake
.\vcpkg integrate install
```

#### Option 2: Manual Qt Installation

Download and install Qt from https://www.qt.io/download-qt-installer

Make sure to install the following components:
- Qt 6.x for Windows
- Qt Bluetooth
- Qt Multimedia
- CMake integration

## Building

### Using CMake (Command Line)

#### If you installed dependencies via vcpkg:

```powershell
# Navigate to the windows directory
cd windows

# Create build directory
mkdir build
cd build

# Configure CMake with vcpkg toolchain file
# Replace the path with your actual vcpkg installation path
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Build the project
cmake --build . --config Release

# The executable will be in build/Release/librepods-windows.exe
```

#### If you installed Qt manually (not using vcpkg):

```powershell
# Navigate to the windows directory
cd windows

# Create build directory
mkdir build
cd build

# Configure CMake (adjust Qt6 path to match your installation)
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvc2019_64"

# Build the project
cmake --build . --config Release

# The executable will be in build/Release/librepods-windows.exe
```

### Using Qt Creator

1. Open Qt Creator
2. Open the `CMakeLists.txt` file in the `windows` directory
3. Configure the project with your Qt kit
4. Build and run the project

## Setup

### Bluetooth Configuration

1. Pair your AirPods with your Windows PC using Windows Bluetooth settings
2. Make sure Bluetooth is enabled and working
3. Launch LibrePods Windows application

### First Run

On first run, the application will:
- Create a system tray icon
- Start monitoring for AirPods connections
- Load saved preferences (if any)

## Features

### Noise Control

Switch between different noise control modes:
- **Off** - No noise control
- **Transparency** - Hear your surroundings
- **Adaptive Transparency** - Automatically adjusts to environment
- **Noise Cancellation** - Block out external noise

### Ear Detection

Automatically pause/resume music when you remove/insert AirPods:
- **Pause when one removed** - Pause if either AirPod is removed
- **Pause when both removed** - Pause only when both are removed
- **Disabled** - No automatic pause/resume

### Battery Monitoring

View real-time battery levels for:
- Left AirPod
- Right AirPod
- Charging case

### Conversational Awareness

Automatically lowers volume when you speak, making it easier to have conversations without removing your AirPods.

## Usage

### System Tray Icon

- **Left-click** - View battery status
- **Right-click** - Access control menu:
  - Toggle Conversational Awareness
  - Switch noise control modes
  - View battery levels
  - Access settings

### Settings

Access settings through the tray icon menu to configure:
- Auto-start with Windows
- Ear detection behavior
- Notification preferences
- Cross-device features

## Troubleshooting

### AirPods Not Connecting

1. Make sure your AirPods are paired in Windows Bluetooth settings
2. Try forgetting and re-pairing the device
3. Restart the LibrePods application
4. Check that Bluetooth service is running in Windows Services

### Audio Issues

1. Ensure AirPods are set as the default audio device in Windows Sound settings
2. Check that audio services are running
3. Try disconnecting and reconnecting the AirPods

### Battery Not Showing

1. Make sure the AirPods are fully connected (both left and right)
2. Put the AirPods in your ears to activate the connection
3. Wait a few seconds for the battery information to sync

## Hearing Aid Features

To enable hearing aid features, you need to configure Windows to present as an Apple device to the AirPods. This feature is currently in development for Windows.

## Known Limitations

- Hearing aid configuration requires additional setup (coming soon)
- Some features may require elevated privileges
- Multi-device connectivity is experimental

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues.

## License

LibrePods - AirPods liberated from Apple's ecosystem
Copyright (C) 2025 LibrePods contributors

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

See LICENSE file for more details.

## Credits

This Windows port is based on the original LibrePods Linux implementation by kavishdevar.
- Original project: https://github.com/kavishdevar/librepods
- Windows port maintainer: Tblob18

## Support

For issues specific to the Windows version, please open an issue on GitHub:
https://github.com/Tblob18/librepods-windows/issues
