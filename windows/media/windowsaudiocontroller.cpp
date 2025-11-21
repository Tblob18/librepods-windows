#include "windowsaudiocontroller.h"
#include "../logger.h"

#ifdef Q_OS_WIN
#include <comdef.h>
#include <Mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <Propvarutil.h>
#endif

WindowsAudioController::WindowsAudioController(QObject *parent)
    : QObject(parent)
#ifdef Q_OS_WIN
    , m_deviceEnumerator(nullptr)
    , m_endpointVolume(nullptr)
#endif
    , m_initialized(false)
{
}

WindowsAudioController::~WindowsAudioController()
{
#ifdef Q_OS_WIN
    if (m_endpointVolume)
    {
        m_endpointVolume->Release();
        m_endpointVolume = nullptr;
    }
    if (m_deviceEnumerator)
    {
        m_deviceEnumerator->Release();
        m_deviceEnumerator = nullptr;
    }
    CoUninitialize();
#endif
}

bool WindowsAudioController::initialize()
{
#ifdef Q_OS_WIN
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE)
    {
        LOG_ERROR("Failed to initialize COM");
        return false;
    }

    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr,
                          CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
                          (void**)&m_deviceEnumerator);
    if (FAILED(hr))
    {
        LOG_ERROR("Failed to create device enumerator");
        CoUninitialize();
        return false;
    }

    m_initialized = true;
    LOG_INFO("Windows Audio Controller initialized successfully");
    return true;
#else
    LOG_ERROR("Windows Audio Controller can only be used on Windows");
    return false;
#endif
}

QString WindowsAudioController::getDefaultSink()
{
#ifdef Q_OS_WIN
    if (!m_initialized || !m_deviceEnumerator)
        return QString();

    IMMDevice *defaultDevice = nullptr;
    HRESULT hr = m_deviceEnumerator->GetDefaultAudioEndpoint(
        eRender, eConsole, &defaultDevice);
    
    if (FAILED(hr))
    {
        LOG_ERROR("Failed to get default audio endpoint");
        return QString();
    }

    LPWSTR deviceId = nullptr;
    hr = defaultDevice->GetId(&deviceId);
    QString result;
    if (SUCCEEDED(hr))
    {
        result = QString::fromWCharArray(deviceId);
        CoTaskMemFree(deviceId);
    }

    defaultDevice->Release();
    return result;
#else
    return QString();
#endif
}

int WindowsAudioController::getSinkVolume(const QString &sinkName)
{
#ifdef Q_OS_WIN
    if (!m_initialized || !m_deviceEnumerator)
        return -1;

    IMMDevice *device = nullptr;
    std::wstring deviceIdW = sinkName.toStdWString();
    HRESULT hr = m_deviceEnumerator->GetDevice(deviceIdW.c_str(), &device);
    
    if (FAILED(hr))
    {
        LOG_ERROR("Failed to get audio device");
        return -1;
    }

    IAudioEndpointVolume *endpointVolume = nullptr;
    hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL,
                          nullptr, (void**)&endpointVolume);
    
    if (FAILED(hr))
    {
        device->Release();
        LOG_ERROR("Failed to activate endpoint volume");
        return -1;
    }

    float volumeLevel = 0.0f;
    hr = endpointVolume->GetMasterVolumeLevelScalar(&volumeLevel);
    
    int volumePercent = -1;
    if (SUCCEEDED(hr))
    {
        volumePercent = static_cast<int>(volumeLevel * 100.0f);
    }

    endpointVolume->Release();
    device->Release();
    
    return volumePercent;
#else
    Q_UNUSED(sinkName);
    return -1;
#endif
}

bool WindowsAudioController::setSinkVolume(const QString &sinkName, int volumePercent)
{
#ifdef Q_OS_WIN
    if (!m_initialized || !m_deviceEnumerator)
        return false;

    if (volumePercent < 0 || volumePercent > 100)
        return false;

    IMMDevice *device = nullptr;
    std::wstring deviceIdW = sinkName.toStdWString();
    HRESULT hr = m_deviceEnumerator->GetDevice(deviceIdW.c_str(), &device);
    
    if (FAILED(hr))
    {
        LOG_ERROR("Failed to get audio device");
        return false;
    }

    IAudioEndpointVolume *endpointVolume = nullptr;
    hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL,
                          nullptr, (void**)&endpointVolume);
    
    if (FAILED(hr))
    {
        device->Release();
        LOG_ERROR("Failed to activate endpoint volume");
        return false;
    }

    float volumeLevel = static_cast<float>(volumePercent) / 100.0f;
    hr = endpointVolume->SetMasterVolumeLevelScalar(volumeLevel, nullptr);
    
    bool success = SUCCEEDED(hr);
    if (!success)
    {
        LOG_ERROR("Failed to set volume");
    }

    endpointVolume->Release();
    device->Release();
    
    return success;
#else
    Q_UNUSED(sinkName);
    Q_UNUSED(volumePercent);
    return false;
#endif
}

bool WindowsAudioController::setCardProfile(const QString &cardName, const QString &profileName)
{
    // Windows handles audio profiles differently than Linux
    // This is a placeholder for potential future implementation
    Q_UNUSED(cardName);
    Q_UNUSED(profileName);
    LOG_DEBUG("setCardProfile not implemented on Windows");
    return true;
}

QString WindowsAudioController::getCardNameForDevice(const QString &macAddress)
{
#ifdef Q_OS_WIN
    if (!m_initialized || !m_deviceEnumerator)
        return QString();

    IMMDeviceCollection *deviceCollection = nullptr;
    HRESULT hr = m_deviceEnumerator->EnumAudioEndpoints(
        eRender, DEVICE_STATE_ACTIVE, &deviceCollection);
    
    if (FAILED(hr))
    {
        LOG_ERROR("Failed to enumerate audio endpoints");
        return QString();
    }

    UINT count = 0;
    deviceCollection->GetCount(&count);

    QString result;
    for (UINT i = 0; i < count; i++)
    {
        IMMDevice *device = nullptr;
        hr = deviceCollection->Item(i, &device);
        if (FAILED(hr))
            continue;

        QString friendlyName = getDeviceFriendlyName(device);
        
        // Check if device name contains MAC address or AirPods identifier
        if (friendlyName.contains(macAddress, Qt::CaseInsensitive) ||
            friendlyName.contains("AirPods", Qt::CaseInsensitive))
        {
            LPWSTR deviceId = nullptr;
            if (SUCCEEDED(device->GetId(&deviceId)))
            {
                result = QString::fromWCharArray(deviceId);
                CoTaskMemFree(deviceId);
                device->Release();
                break;
            }
        }
        
        device->Release();
    }

    deviceCollection->Release();
    return result;
#else
    Q_UNUSED(macAddress);
    return QString();
#endif
}

bool WindowsAudioController::isProfileAvailable(const QString &cardName, const QString &profileName)
{
    // Windows doesn't use profiles in the same way as Linux PulseAudio
    Q_UNUSED(cardName);
    Q_UNUSED(profileName);
    return true;
}

QString WindowsAudioController::getDeviceFriendlyName(void *devicePtr)
{
#ifdef Q_OS_WIN
    IMMDevice *device = static_cast<IMMDevice*>(devicePtr);
    if (!device)
        return QString();

    IPropertyStore *propertyStore = nullptr;
    HRESULT hr = device->OpenPropertyStore(STGM_READ, &propertyStore);
    if (FAILED(hr))
        return QString();

    PROPVARIANT friendlyName;
    PropVariantInit(&friendlyName);
    
    hr = propertyStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
    QString name;
    if (SUCCEEDED(hr) && friendlyName.vt == VT_LPWSTR)
    {
        name = QString::fromWCharArray(friendlyName.pwszVal);
    }

    PropVariantClear(&friendlyName);
    propertyStore->Release();
    
    return name;
#else
    Q_UNUSED(devicePtr);
    return QString();
#endif
}

bool WindowsAudioController::setDefaultAudioDevice(const QString &deviceId)
{
    // Setting default audio device on Windows requires using PolicyConfig COM interface
    // which is undocumented. For now, this is a placeholder.
    Q_UNUSED(deviceId);
    LOG_WARN("setDefaultAudioDevice not implemented on Windows");
    return false;
}
