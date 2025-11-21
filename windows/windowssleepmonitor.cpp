#include "windowssleepmonitor.hpp"
#include "logger.h"

#ifdef Q_OS_WIN
#include <PowrProf.h>
#pragma comment(lib, "PowrProf.lib")
#endif

WindowsSleepMonitor::WindowsSleepMonitor(QObject *parent)
    : QObject(parent)
#ifdef Q_OS_WIN
    , m_hwnd(nullptr)
    , m_powerNotify(nullptr)
#endif
{
}

WindowsSleepMonitor::~WindowsSleepMonitor()
{
#ifdef Q_OS_WIN
    if (m_powerNotify)
    {
        UnregisterPowerSettingNotification(m_powerNotify);
        m_powerNotify = nullptr;
    }
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
#endif
}

bool WindowsSleepMonitor::initialize()
{
#ifdef Q_OS_WIN
    // Create a message-only window for receiving power notifications
    // Use a static flag to register the window class only once
    static bool classRegistered = false;
    
    if (!classRegistered)
    {
        WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
        wc.lpfnWndProc = windowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = L"LibrePodsWindowsSleepMonitor";
        
        if (!RegisterClassEx(&wc))
        {
            DWORD error = GetLastError();
            if (error != ERROR_CLASS_ALREADY_EXISTS)
            {
                LOG_ERROR("Failed to register window class, error: " << error);
                return false;
            }
        }
        classRegistered = true;
    }

    m_hwnd = CreateWindowEx(
        0, L"LibrePodsWindowsSleepMonitor", L"",
        0, 0, 0, 0, 0,
        HWND_MESSAGE, nullptr, GetModuleHandle(nullptr), this
    );

    if (!m_hwnd)
    {
        LOG_ERROR("Failed to create message-only window");
        return false;
    }

    // Store 'this' pointer in window user data for callback access
    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    // Register for power notifications
    m_powerNotify = RegisterPowerSettingNotification(
        m_hwnd,
        &GUID_CONSOLE_DISPLAY_STATE,
        DEVICE_NOTIFY_WINDOW_HANDLE
    );

    if (!m_powerNotify)
    {
        LOG_ERROR("Failed to register for power notifications");
        return false;
    }

    LOG_INFO("Windows Sleep Monitor initialized successfully");
    return true;
#else
    LOG_ERROR("Windows Sleep Monitor can only be used on Windows");
    return false;
#endif
}

#ifdef Q_OS_WIN
LRESULT CALLBACK WindowsSleepMonitor::windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_POWERBROADCAST)
    {
        WindowsSleepMonitor *monitor = reinterpret_cast<WindowsSleepMonitor*>(
            GetWindowLongPtr(hwnd, GWLP_USERDATA)
        );
        
        // Validate monitor pointer before use
        if (monitor && !IsBadReadPtr(monitor, sizeof(WindowsSleepMonitor)))
        {
            monitor->handlePowerBroadcast(wParam, lParam);
        }
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void WindowsSleepMonitor::handlePowerBroadcast(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case PBT_APMSUSPEND:
        LOG_DEBUG("System is going to sleep");
        emit systemGoingToSleep();
        break;
        
    case PBT_APMRESUMEAUTOMATIC:
    case PBT_APMRESUMESUSPEND:
        LOG_DEBUG("System is waking up");
        emit systemWakingUp();
        break;
        
    case PBT_POWERSETTINGCHANGE:
        {
            POWERBROADCAST_SETTING *setting = reinterpret_cast<POWERBROADCAST_SETTING*>(lParam);
            if (setting && IsEqualGUID(setting->PowerSetting, GUID_CONSOLE_DISPLAY_STATE))
            {
                DWORD displayState = *reinterpret_cast<DWORD*>(&setting->Data);
                if (displayState == 0)
                {
                    LOG_DEBUG("Display turned off");
                    // Optionally handle display off as a sleep-like event
                }
                else if (displayState == 1)
                {
                    LOG_DEBUG("Display turned on");
                    // Optionally handle display on as a wake-like event
                }
            }
        }
        break;
    }
}
#endif
