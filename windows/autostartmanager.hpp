#ifndef AUTOSTARTMANAGER_HPP
#define AUTOSTARTMANAGER_HPP

#include <QObject>
#include <QSettings>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QCoreApplication>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class AutoStartManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool autoStartEnabled READ autoStartEnabled WRITE setAutoStartEnabled NOTIFY autoStartEnabledChanged)

public:
    explicit AutoStartManager(QObject *parent = nullptr) : QObject(parent)
    {
#ifdef Q_OS_WIN
        // On Windows, we use the registry Run key
        m_registryKey = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
        m_appName = QCoreApplication::applicationName();
#else
        QString autostartDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart";
        QDir().mkpath(autostartDir);
        m_autostartFilePath = autostartDir + "/" + QCoreApplication::applicationName() + ".desktop";
#endif
    }

    bool autoStartEnabled() const
    {
#ifdef Q_OS_WIN
        QSettings settings(m_registryKey, QSettings::NativeFormat);
        return settings.contains(m_appName);
#else
        return QFile::exists(m_autostartFilePath);
#endif
    }

    void setAutoStartEnabled(bool enabled)
    {
        if (autoStartEnabled() == enabled)
        {
            return;
        }

        if (enabled)
        {
            createAutoStartEntry();
        }
        else
        {
            removeAutoStartEntry();
        }

        emit autoStartEnabledChanged(enabled);
    }

signals:
    void autoStartEnabledChanged(bool enabled);

private:
    void createAutoStartEntry()
    {
#ifdef Q_OS_WIN
        QSettings settings(m_registryKey, QSettings::NativeFormat);
        QString appPath = QCoreApplication::applicationFilePath();
        // Add quotes around the path if it contains spaces
        if (appPath.contains(' '))
        {
            appPath = "\"" + appPath + "\"";
        }
        appPath += " --hide";
        settings.setValue(m_appName, appPath);
        settings.sync();
#else
        QFile desktopFile(m_autostartFilePath);
        if (!desktopFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qWarning() << "Failed to create autostart file:" << desktopFile.errorString();
            return;
        }

        QString appPath = QCoreApplication::applicationFilePath();
        // Handle cases where the path might contain spaces
        if (appPath.contains(' '))
        {
            appPath = "\"" + appPath + "\"";
        }

        QString content = QStringLiteral(
                              "[Desktop Entry]\n"
                              "Type=Application\n"
                              "Name=%1\n"
                              "Exec=%2 --hide\n"
                              "Icon=%3\n"
                              "Comment=%4\n"
                              "X-GNOME-Autostart-enabled=true\n"
                              "Terminal=false\n")
                              .arg(
                                  QCoreApplication::applicationName(),
                                  appPath,
                                  QCoreApplication::applicationName().toLower(),
                                  QCoreApplication::applicationName() + " autostart");

        desktopFile.write(content.toUtf8());
        desktopFile.close();
#endif
    }

    void removeAutoStartEntry()
    {
#ifdef Q_OS_WIN
        QSettings settings(m_registryKey, QSettings::NativeFormat);
        settings.remove(m_appName);
        settings.sync();
#else
        if (QFile::exists(m_autostartFilePath))
        {
            QFile::remove(m_autostartFilePath);
        }
#endif
    }

#ifdef Q_OS_WIN
    QString m_registryKey;
    QString m_appName;
#else
    QString m_autostartFilePath;
#endif
};

#endif // AUTOSTARTMANAGER_HPP