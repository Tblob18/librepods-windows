#pragma once

#include <QDebug>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(librepods)

#define LOG_INFO(msg) qCInfo(librepods) << "\033[32m" << msg << "\033[0m"
#define LOG_WARN(msg) qCWarning(librepods) << "\033[33m" << msg << "\033[0m"
#define LOG_ERROR(msg) qCCritical(librepods) << "\033[31m" << msg << "\033[0m"
#define LOG_DEBUG(msg) qCDebug(librepods) << "\033[34m" << msg << "\033[0m"
