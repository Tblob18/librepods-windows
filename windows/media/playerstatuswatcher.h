#pragma once

#include <QObject>

// Windows stub implementation - DBus is not available on Windows
// This provides a no-op implementation that compiles but doesn't monitor media players
class PlayerStatusWatcher : public QObject {
    Q_OBJECT
public:
    explicit PlayerStatusWatcher(const QString &playerService, QObject *parent = nullptr);
    static QString getCurrentPlaybackStatus(const QString &playerService);

signals:
    void playbackStatusChanged(const QString &status);

private:
    QString m_playerService;
};