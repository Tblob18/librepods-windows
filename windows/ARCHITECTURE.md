# Windows Port Architecture

This document explains the technical architecture of the LibrePods Windows port and how it differs from the Linux implementation.

## Overview

The Windows port maintains the same core functionality as the Linux version while replacing platform-specific components with Windows equivalents. The application uses Qt6 for cross-platform Bluetooth and UI functionality, with Windows-specific implementations for audio control and system integration.

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                    LibrePods Windows                          │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  ┌─────────────────┐         ┌──────────────────┐          │
│  │   QML UI Layer  │◄────────┤  Main Application │          │
│  │  (System Tray)  │         │  (AirPodsTrayApp) │          │
│  └─────────────────┘         └──────────────────┘          │
│           │                           │                      │
│           │                           │                      │
│  ┌────────▼───────────────────────────▼────────────┐       │
│  │         TrayIconManager                          │       │
│  │  (Battery Status, Menu, Notifications)          │       │
│  └─────────────────────────────────────────────────┘       │
│                                                              │
│  ┌──────────────────────────────────────────────┐          │
│  │           DeviceInfo Manager                  │          │
│  │  (Battery, Noise Control, Device State)      │          │
│  └──────────────────────────────────────────────┘          │
│                          │                                   │
│           ┌──────────────┼──────────────┐                  │
│           │              │               │                  │
│  ┌────────▼──────┐ ┌────▼────────┐ ┌───▼──────────┐      │
│  │ BLE Manager   │ │ Media       │ │ Auto-start   │      │
│  │ (Qt Bluetooth)│ │ Controller  │ │ Manager      │      │
│  └───────────────┘ └─────────────┘ └──────────────┘      │
│                          │                                   │
│         ┌────────────────┼────────────────┐                │
│         │                │                 │                │
│  ┌──────▼─────────┐ ┌───▼──────────┐ ┌───▼────────────┐  │
│  │ Windows Audio  │ │ Windows Sleep│ │ Registry       │  │
│  │ Controller     │ │ Monitor      │ │ (Auto-start)   │  │
│  │ (WASAPI)       │ │ (PowerMgmt)  │ │                │  │
│  └────────────────┘ └──────────────┘ └────────────────┘  │
│         │                    │                              │
└─────────┼────────────────────┼──────────────────────────────┘
          │                    │
          │                    │
┌─────────▼────────────────────▼──────────────────────────────┐
│                    Windows APIs                              │
├──────────────────────────────────────────────────────────────┤
│  • Core Audio (WASAPI)                                       │
│  • IMMDeviceEnumerator (Audio Device Discovery)              │
│  • IAudioEndpointVolume (Volume Control)                     │
│  • Power Management (Suspend/Resume)                         │
│  • Registry (Startup Configuration)                          │
│  • Window Messages (Power Notifications)                     │
└──────────────────────────────────────────────────────────────┘
```

## Platform-Specific Components

### 1. Windows Audio Controller (`windowsaudiocontroller.cpp/h`)

**Replaces**: Linux PulseAudio Controller

**Purpose**: Manages audio device enumeration and volume control using Windows Core Audio APIs.

**Key APIs Used**:
- `IMMDeviceEnumerator` - Enumerate audio devices
- `IAudioEndpointVolume` - Control volume levels
- `IPropertyStore` - Get device properties (friendly names)

**Features**:
- Get default audio sink (playback device)
- Set/get volume for specific devices
- Enumerate available audio devices
- Detect AirPods by device name or MAC address

**Implementation Notes**:
- COM initialization in constructor
- Proper COM cleanup in destructor
- Safe string conversion from QString to Windows LPCWSTR
- Error handling with HRESULT codes

### 2. Windows Sleep Monitor (`windowssleepmonitor.cpp/hpp`)

**Replaces**: Linux D-Bus/logind system integration

**Purpose**: Monitor system power state changes to handle AirPods reconnection after sleep/wake.

**Key APIs Used**:
- `RegisterPowerSettingNotification` - Register for power events
- `WM_POWERBROADCAST` - Window message for power state changes
- `PBT_APMSUSPEND` - System going to sleep
- `PBT_APMRESUMEAUTOMATIC/PBT_APMRESUMESUSPEND` - System waking up

**Features**:
- Detect system suspend
- Detect system resume
- Handle display state changes (optional)

**Implementation Notes**:
- Uses message-only window for receiving power notifications
- Static window class registration to prevent multiple registrations
- Pointer validation in window procedure for safety
- Signals Qt objects when power events occur

### 3. Auto-Start Manager (`autostartmanager.hpp`)

**Replaces**: Linux desktop file in `~/.config/autostart/`

**Purpose**: Enable/disable automatic startup with Windows.

**Key APIs Used**:
- Windows Registry via QSettings
- `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`

**Features**:
- Enable auto-start by adding registry entry
- Disable auto-start by removing registry entry
- Check current auto-start status

**Implementation Notes**:
- Uses Qt's QSettings with NativeFormat for registry access
- Properly quotes application path if it contains spaces
- Adds `--hide` flag to start minimized to tray

### 4. Media Controller Platform Abstraction

**Purpose**: Provide unified interface for both Linux (PulseAudio/MPRIS) and Windows (WASAPI) audio systems.

**Key Changes**:
- Wrapper methods for audio operations
- Platform guards (`#ifdef Q_OS_WIN` / `#ifdef Q_OS_LINUX`)
- Conditional compilation for D-Bus code (Linux only)
- Runtime platform detection

**Methods with Platform Abstraction**:
- `getDefaultSink()` - Get current audio output device
- `getSinkVolume()` - Get volume level
- `setSinkVolume()` - Set volume level
- `getCardNameForDevice()` - Find audio device by MAC
- `setCardProfile()` - Switch audio profiles (stub on Windows)
- `isProfileAvailable()` - Check profile availability (stub on Windows)

## Cross-Platform Components

These components work identically on both platforms:

### 1. BLE Manager (Qt6::Bluetooth)
- Handles Bluetooth Low Energy communication
- Discovers AirPods
- Manages encrypted communication
- Platform-independent through Qt

### 2. Device Info Manager
- Parses AirPods protocol packets
- Manages battery status
- Tracks noise control mode
- Handles conversational awareness

### 3. Tray Icon Manager (Qt6::Widgets)
- System tray integration
- Context menu
- Notifications
- Platform-independent through Qt

### 4. QML UI Components
- Battery indicators
- Settings dialogs
- Control interfaces

## Build System

### CMakeLists.txt Differences

**Windows-Specific**:
```cmake
find_package(Qt6 REQUIRED COMPONENTS Quick Widgets Bluetooth Multimedia)
# Note: No DBus on Windows

target_link_libraries(librepods-windows
    PRIVATE 
    Qt6::Quick 
    Qt6::Widgets 
    Qt6::Bluetooth 
    Qt6::Multimedia
    OpenSSL::SSL 
    OpenSSL::Crypto
    Ole32      # COM support
    User32     # Windows messages
    PowrProf   # Power management
)
```

**Linux-Specific**:
```cmake
find_package(Qt6 REQUIRED COMPONENTS Quick Widgets Bluetooth DBus)
pkg_check_modules(PULSEAUDIO REQUIRED libpulse)

target_link_libraries(librepods
    PRIVATE 
    Qt6::Quick 
    Qt6::Widgets 
    Qt6::Bluetooth 
    Qt6::DBus
    OpenSSL::SSL 
    OpenSSL::Crypto 
    ${PULSEAUDIO_LIBRARIES}
)
```

## Communication Flow

### AirPods Connection
```
1. User pairs AirPods with Windows Bluetooth
2. Windows establishes basic Bluetooth connection
3. Qt6::Bluetooth detects the device
4. BleManager initiates encrypted handshake
5. DeviceInfo parses battery and status packets
6. TrayIconManager updates UI
```

### Noise Control Mode Change
```
1. User clicks mode in tray menu
2. TrayIconManager emits signal
3. DeviceInfo sends control packet to AirPods
4. AirPods acknowledge mode change
5. TrayIconManager updates menu state
```

### Volume Control (Conversational Awareness)
```
1. AirPods detect user speaking
2. Packet sent to PC indicating CA trigger
3. MediaController receives notification
4. WindowsAudioController lowers system volume
5. When speaking stops, volume restored
```

## Known Limitations

### Not Yet Implemented on Windows
1. **MPRIS Media Control** - Play/pause/skip from AirPods
   - Requires Windows Media Session API integration
   - Currently returns placeholder responses

2. **Hearing Aid Setup** - Full audiogram configuration
   - Planned for future release
   - Basic hearing aid toggle works

3. **Device ID Spoofing** - Acting as Apple device
   - Windows doesn't have equivalent to Linux `/etc/bluetooth/main.conf`
   - May require custom Bluetooth stack modifications

### Platform Differences

| Feature | Linux | Windows |
|---------|-------|---------|
| Audio System | PulseAudio | Core Audio (WASAPI) |
| Sleep Detection | D-Bus/logind | Power Management API |
| Auto-start | Desktop file | Registry |
| Media Control | MPRIS (D-Bus) | Not yet implemented |
| Profile Switching | PulseAudio profiles | N/A (handled by Windows) |

## Security Considerations

1. **COM Object Lifetime** - Proper initialization and cleanup
2. **Window Procedure Safety** - Pointer validation before use
3. **Registry Access** - User-level keys only (no admin required)
4. **String Conversions** - Proper null termination for Windows APIs
5. **Bluetooth Encryption** - Same protocol as Linux (secure)

## Future Improvements

1. **Windows Media Session API Integration**
   - Detect current media player
   - Send play/pause/skip commands
   - Query playback status

2. **Windows Toast Notifications**
   - Better native notification support
   - Action buttons in notifications

3. **MSI Installer**
   - Automated installation
   - Start menu shortcuts
   - Uninstaller

4. **Hearing Aid Full Support**
   - Audiogram configuration UI
   - Hearing test integration
   - Per-frequency adjustment

5. **Multiple Language Support**
   - Internationalization (i18n)
   - Localized strings

## Development Guidelines

### Adding New Features

When adding features that may be platform-specific:

1. **Check for platform differences** in the underlying APIs
2. **Use conditional compilation** (`#ifdef Q_OS_WIN`, `#ifdef Q_OS_LINUX`)
3. **Create wrapper methods** in platform-independent classes
4. **Document platform limitations** in code comments
5. **Update this architecture document**

### Testing

Since the main development is done on Linux/macOS:

1. Use conditional compilation guards
2. Test compilation on Windows regularly
3. Use Windows VMs for runtime testing
4. Document Windows-specific behaviors

### Code Style

- Use Qt naming conventions
- Platform-specific code in platform-specific files
- Keep cross-platform code truly cross-platform
- Add comments explaining Windows API usage

## Conclusion

The Windows port maintains feature parity with the Linux version while adapting to Windows-specific APIs and conventions. The modular design allows for platform-specific implementations while sharing the majority of the codebase, making maintenance and future enhancements easier.
