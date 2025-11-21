#include "playerstatuswatcher.h"
#include "logger.h"

// Windows stub implementation - DBus is not available on Windows
// This provides a no-op implementation that compiles but doesn't monitor media players
// Media player monitoring on Windows would require the Windows Media Session API instead

PlayerStatusWatcher::PlayerStatusWatcher(const QString &playerService, QObject *parent)
    : QObject(parent),
      m_playerService(playerService)
{
    // Stub implementation for Windows - DBus not available
    LOG_DEBUG("PlayerStatusWatcher: DBus not available on Windows, media monitoring disabled");
}

QString PlayerStatusWatcher::getCurrentPlaybackStatus(const QString &playerService)
{
    Q_UNUSED(playerService);
    // Stub implementation for Windows - DBus not available
    return QString();
}