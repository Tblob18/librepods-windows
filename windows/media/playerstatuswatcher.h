#pragma once

#include <QObject>

// Windows stub - DBus not available on Windows
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