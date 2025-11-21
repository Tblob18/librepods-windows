#ifndef WINDOWSAUDIOCONTROLLER_H
#define WINDOWSAUDIOCONTROLLER_H

#include <QString>
#include <QObject>

#ifdef Q_OS_WIN
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <audioclient.h>
#include <functiondiscoverykeys_devpkey.h>
#endif

class WindowsAudioController : public QObject
{
    Q_OBJECT

public:
    explicit WindowsAudioController(QObject *parent = nullptr);
    ~WindowsAudioController();

    bool initialize();
    QString getDefaultSink();
    int getSinkVolume(const QString &sinkName);
    bool setSinkVolume(const QString &sinkName, int volumePercent);
    bool setCardProfile(const QString &cardName, const QString &profileName);
    QString getCardNameForDevice(const QString &macAddress);
    bool isProfileAvailable(const QString &cardName, const QString &profileName);

private:
#ifdef Q_OS_WIN
    IMMDeviceEnumerator *m_deviceEnumerator;
    IAudioEndpointVolume *m_endpointVolume;
#endif
    bool m_initialized;

    QString getDeviceFriendlyName(void *device);
    bool setDefaultAudioDevice(const QString &deviceId);
};

#endif // WINDOWSAUDIOCONTROLLER_H
