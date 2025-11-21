#ifndef SYSTEMSLEEPMONITOR_HPP
#define SYSTEMSLEEPMONITOR_HPP

#include <QObject>
#include <QDebug>

// Windows stub - DBus not available on Windows
// TODO: Implement Windows Power Management APIs for sleep/wake monitoring
class SystemSleepMonitor : public QObject {
    Q_OBJECT

public:
    explicit SystemSleepMonitor(QObject *parent = nullptr) : QObject(parent) {
        qWarning() << "SystemSleepMonitor: DBus-based monitoring not available on Windows";
        // Future implementation would use Windows Power Management APIs
    }

    ~SystemSleepMonitor() override = default;

signals:
    void systemGoingToSleep();
    void systemWakingUp();
};

#endif // SYSTEMSLEEPMONITOR_HPP