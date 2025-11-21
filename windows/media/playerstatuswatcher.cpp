#include "playerstatuswatcher.h"

// Windows stub implementation - DBus not available on Windows
// Media player control on Windows would require Windows Media Session API

PlayerStatusWatcher::PlayerStatusWatcher(const QString &playerService, QObject *parent)
    : QObject(parent),
      m_playerService(playerService)
{
    // Empty constructor - no DBus monitoring on Windows
}

QString PlayerStatusWatcher::getCurrentPlaybackStatus(const QString &playerService)
{
    Q_UNUSED(playerService);
    // Return empty string - no DBus on Windows
    // Future implementation would use Windows Media Session API
    return QString();
}