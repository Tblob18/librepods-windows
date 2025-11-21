# Quick Start Guide - LibrePods for Windows

Get your AirPods Pro working with Windows in minutes!

## Prerequisites

Before you start, make sure you have:
- Windows 10 (1809 or later) or Windows 11
- AirPods Pro (2nd or 3rd generation) or compatible AirPods
- Bluetooth enabled on your PC

## Installation

### Option 1: Download Pre-built Binary (Coming Soon)

1. Download the latest release from [Releases](https://github.com/Tblob18/librepods-windows/releases)
2. Extract the ZIP file to a folder (e.g., `C:\Program Files\LibrePods`)
3. Run `librepods-windows.exe`

### Option 2: Build from Source

See [BUILD_AND_TEST.md](BUILD_AND_TEST.md) for detailed build instructions.

**Quick version for vcpkg (Recommended for VS Code users):**

1. **Install Visual Studio Build Tools** (required even without Visual Studio IDE):
   - Download from [https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022)
   - Select "Desktop development with C++" during installation

2. **Install CMake**:
   - Download from [https://cmake.org/download/](https://cmake.org/download/)
   - Select "Add CMake to the system PATH" during installation
   - Restart your terminal

3. **Set up vcpkg and dependencies**:
   ```powershell
   # Clone vcpkg (outside of librepods-windows directory)
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   
   # Bootstrap and update vcpkg
   .\bootstrap-vcpkg.bat
   git pull
   
   # Install dependencies (takes 30-60 minutes)
   .\vcpkg install qt6-base:x64-windows qt6-connectivity:x64-windows qt6-multimedia:x64-windows qt6-declarative:x64-windows openssl:x64-windows
   
   # Integrate with CMake
   .\vcpkg integrate install
   ```

4. **Build LibrePods**:
   ```powershell
   # Navigate to librepods-windows\windows directory
   cd path\to\librepods-windows\windows
   
   # Create build directory
   mkdir build
   cd build
   
   # Configure and build (replace with your vcpkg path)
   cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
   cmake --build . --config Release
   
   # Deploy Qt dependencies
   cd Release
   C:\path\to\vcpkg\installed\x64-windows\tools\qt6\bin\windeployqt.exe librepods-windows.exe
   
   # Run the application
   .\librepods-windows.exe
   ```

**Quick version for manual Qt installation:**

```powershell
# Install Qt 6.x from https://www.qt.io/download
# Open PowerShell in the windows directory
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvc2019_64"
cmake --build . --config Release
cd Release
windeployqt librepods-windows.exe
.\librepods-windows.exe
```

## Initial Setup

### Step 1: Pair Your AirPods

1. Open Windows Settings (Win + I)
2. Go to Bluetooth & devices
3. Put your AirPods in pairing mode (hold button on case)
4. Click "Add device" → "Bluetooth"
5. Select your AirPods from the list
6. Wait for pairing to complete

### Step 2: Launch LibrePods

1. Run `librepods-windows.exe`
2. Look for the LibrePods icon in the system tray (bottom-right corner)
3. If you don't see it, click the arrow (^) to show hidden icons

### Step 3: Connect Your AirPods

1. Put your AirPods in your ears
2. Wait a few seconds for the connection to establish
3. The tray icon should update with battery information

## Basic Usage

### Viewing Battery Status

**Left-click** the tray icon to see:
- Left AirPod battery percentage
- Right AirPod battery percentage
- Charging case battery percentage

### Changing Noise Control Mode

**Right-click** the tray icon and select:
- **Off** - No noise control, natural sound
- **Transparency** - Hear your surroundings
- **Noise Cancellation** - Block external noise

### Enabling Conversational Awareness

1. Right-click the tray icon
2. Check "Conversational Awareness"
3. Volume will automatically lower when you speak

### Configuring Ear Detection

1. Right-click the tray icon → "Settings"
2. Under Ear Detection, choose:
   - **Pause when one removed** - Pause if either AirPod is out
   - **Pause when both removed** - Pause only when both are out
   - **Disabled** - No automatic pause/resume

### Auto-Start with Windows

1. Right-click the tray icon → "Settings"
2. Check "Start with Windows"
3. LibrePods will now launch automatically when you log in

## Troubleshooting

### Build Issues

#### vcpkg Packages Not Found
**Problem**: `qt6-base does not exist` or similar errors when installing vcpkg packages.

**Solution**:
```powershell
cd path\to\vcpkg
git pull
.\vcpkg update
.\vcpkg install qt6-base:x64-windows qt6-connectivity:x64-windows qt6-multimedia:x64-windows qt6-declarative:x64-windows openssl:x64-windows
```

#### Visual Studio Instance Not Found
**Problem**: `Unable to find a valid Visual Studio instance` when installing vcpkg packages.

**Solution**: Install Visual Studio Build Tools (not the full IDE):
1. Download from [https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022)
2. Select "Desktop development with C++"
3. Ensure MSVC compiler and Windows SDK are checked
4. Complete installation and restart terminal

#### CMake Not Found
**Problem**: `cmake: The term 'cmake' is not recognized`

**Solution**:
1. Download CMake from [https://cmake.org/download/](https://cmake.org/download/)
2. During installation, select "Add CMake to the system PATH for all users"
3. Restart your PowerShell/terminal
4. Verify: `cmake --version`

### Runtime Issues

### AirPods Not Detected

**Problem**: LibrePods doesn't show battery or allow control changes.

**Solutions**:
1. Make sure AirPods are paired in Windows Bluetooth settings
2. Put both AirPods in your ears (they need to be active)
3. Restart LibrePods
4. Try disconnecting and reconnecting in Windows Bluetooth settings

### No Sound from AirPods

**Problem**: AirPods are connected but no audio plays.

**Solutions**:
1. Open Windows Sound settings (right-click speaker icon)
2. Set AirPods as the default playback device
3. Check that volume is not muted
4. Try playing audio from a different application

### Battery Not Updating

**Problem**: Battery percentages show as "Unknown" or don't update.

**Solutions**:
1. Put both AirPods in your ears
2. Wait 10-15 seconds for the battery info to sync
3. Ensure both AirPods are fully connected (not just one)
4. Try removing and reinserting the AirPods

### Mode Changes Not Working

**Problem**: Changing noise control mode doesn't affect the AirPods.

**Solutions**:
1. Make sure both AirPods are in your ears and connected
2. Wait a few seconds after changing the mode
3. Try reconnecting the AirPods
4. Check if the mode change works via the AirPods themselves (long press)

### Application Crashes on Startup

**Problem**: LibrePods crashes immediately when launched.

**Solutions**:
1. Make sure you have all required DLLs (run `windeployqt` if you built from source)
2. Check that Bluetooth is enabled in Windows
3. Try running as Administrator (right-click → Run as administrator)
4. Check the log files in `%APPDATA%\LibrePods\` for error messages

## Advanced Features

### Cross-Device Handoff

If you also use LibrePods on Android:
1. Enable "Cross Device" in Settings
2. Your AirPods can seamlessly switch between devices
3. The device you're actively using will take control

### Notification Settings

1. Right-click tray icon → "Settings"
2. Toggle "Enable Notifications"
3. Get notified about:
   - Low battery warnings
   - Connection status changes
   - Mode changes

## Tips & Tricks

### Keyboard Shortcuts (Coming Soon)
Currently, all controls are accessed via the tray icon. Keyboard shortcuts may be added in future versions.

### Custom Profiles (Coming Soon)
Save your preferred noise control and awareness settings for different scenarios.

### Multiple AirPods
LibrePods currently supports one pair of AirPods at a time. Switch between different pairs by connecting the desired pair in Windows Bluetooth settings.

## Need Help?

- **Bug Reports**: [Open an issue](https://github.com/Tblob18/librepods-windows/issues)
- **Feature Requests**: [Open an issue](https://github.com/Tblob18/librepods-windows/issues) with the "enhancement" label
- **General Questions**: Check existing issues or open a new discussion

## What's Next?

Explore more features:
- Adjust volume via Windows system controls
- Configure auto-start behavior
- Test ear detection with music playback
- Try conversational awareness with different media

Enjoy your liberated AirPods! 🎧
