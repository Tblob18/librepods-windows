#ifndef MEDIACONTROLLER_H
#define MEDIACONTROLLER_H

#include <QObject>

#ifdef Q_OS_LINUX
#include "pulseaudiocontroller.h"
#elif defined(Q_OS_WIN)
#include "windowsaudiocontroller.h"
#endif

class QProcess;
class EarDetection;
class PlayerStatusWatcher;

#ifndef Q_OS_WIN
class QDBusInterface;
#endif

class MediaController : public QObject
{
  Q_OBJECT
public:
  enum MediaState
  {
    Playing,
    Paused,
    Stopped
  };
  Q_ENUM(MediaState)
  enum EarDetectionBehavior
  {
    PauseWhenOneRemoved,
    PauseWhenBothRemoved,
    Disabled
  };
  Q_ENUM(EarDetectionBehavior)

  explicit MediaController(QObject *parent = nullptr);
  ~MediaController();

  void handleEarDetection(EarDetection*);
  void followMediaChanges();
  bool isActiveOutputDeviceAirPods();
  void handleConversationalAwareness(const QByteArray &data);
  void activateA2dpProfile();
  void removeAudioOutputDevice();
  void setConnectedDeviceMacAddress(const QString &macAddress);
  bool isA2dpProfileAvailable();
  QString getPreferredA2dpProfile();
  bool restartWirePlumber();

  void setEarDetectionBehavior(EarDetectionBehavior behavior);
  inline EarDetectionBehavior getEarDetectionBehavior() const { return earDetectionBehavior; }

  void play();
  void pause();
  MediaState getCurrentMediaState() const;

Q_SIGNALS:
  void mediaStateChanged(MediaState state);

private:
  MediaState mediaStateFromPlayerctlOutput(const QString &output) const;
  QString getAudioDeviceName();
  QStringList getPlayingMediaPlayers();
  QString getDefaultSink();
  int getSinkVolume(const QString &sinkName);
  bool setSinkVolume(const QString &sinkName, int volumePercent);
  QString getCardNameForDevice(const QString &macAddress);
  bool setCardProfile(const QString &cardName, const QString &profileName);
  bool isProfileAvailable(const QString &cardName, const QString &profileName);

  QStringList pausedByAppServices;
  int initialVolume = -1;
  QString connectedDeviceMacAddress;
  EarDetectionBehavior earDetectionBehavior = PauseWhenOneRemoved;
  QString m_deviceOutputName;
  PlayerStatusWatcher *playerStatusWatcher = nullptr;
  
#ifdef Q_OS_LINUX
  PulseAudioController *m_pulseAudio = nullptr;
#elif defined(Q_OS_WIN)
  WindowsAudioController *m_windowsAudio = nullptr;
#endif
  
  QString m_cachedA2dpProfile;
};

#endif // MEDIACONTROLLER_H