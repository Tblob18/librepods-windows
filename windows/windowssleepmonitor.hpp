#ifndef WINDOWSSLEEPMONITOR_HPP
#define WINDOWSSLEEPMONITOR_HPP

#include <QObject>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class WindowsSleepMonitor : public QObject {
    Q_OBJECT

public:
    explicit WindowsSleepMonitor(QObject *parent = nullptr);
    ~WindowsSleepMonitor() override;

    bool initialize();

signals:
    void systemGoingToSleep();
    void systemWakingUp();

private:
#ifdef Q_OS_WIN
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    HWND m_hwnd;
    HPOWERNOTIFY m_powerNotify;
    
    void handlePowerBroadcast(WPARAM wParam, LPARAM lParam);
#endif
};

#endif // WINDOWSSLEEPMONITOR_HPP
