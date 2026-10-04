#include "trayiconmanager.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QPainter>
#include <QFont>
#include <QColor>
#include <QActionGroup>
#include <QPainterPath>

using namespace AirpodsTrayApp::Enums;

TrayIconManager::TrayIconManager(QObject *parent) : QObject(parent)
{
    // Initialize tray icon
    trayIcon = new QSystemTrayIcon(QIcon(":/icons/assets/airpods.png"), this);
    trayMenu = new QMenu();

    // Setup basic menu actions
    setupMenuActions();

    // Connect signals
    trayIcon->setContextMenu(trayMenu);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &TrayIconManager::onTrayIconActivated);

    trayIcon->show();
}

void TrayIconManager::showNotification(const QString &title, const QString &message)
{
    if (!m_notificationsEnabled)
        return;
    trayIcon->showMessage(title, message, QSystemTrayIcon::Information, 3000);
}

void TrayIconManager::updateBatteryStatus(const QString &status)
{
    QString tip = m_deviceName.isEmpty() ? QString("AirPods") : m_deviceName;
    if (!status.isEmpty())
        tip += "\n" + QString(status).replace(", ", "\n");
    trayIcon->setToolTip(tip);
    updateIconFromBattery(status);
}

void TrayIconManager::setControlsAvailable(bool available)
{
    caToggleAction->setVisible(available);
    noiseControlGroup->setVisible(available);
    for (QAction *sep : controlSeparators)
        sep->setVisible(available);
}

void TrayIconManager::updateNoiseControlState(NoiseControlMode mode)
{
    QList<QAction *> actions = noiseControlGroup->actions();
    for (QAction *action : actions)
    {
        action->setChecked(action->data().toInt() == (int)mode);
    }
}

void TrayIconManager::updateConversationalAwareness(bool enabled)
{
    caToggleAction->setChecked(enabled);
}

void TrayIconManager::setupMenuActions()
{
    // Open action
    QAction *openAction = new QAction("Open", trayMenu);
    trayMenu->addAction(openAction);
    connect(openAction, &QAction::triggered, qApp, [this](){emit openApp();});

    // Settings Menu

    QAction *settingsMenu = new QAction("Settings", trayMenu);
    trayMenu->addAction(settingsMenu);
    connect(settingsMenu, &QAction::triggered, qApp, [this](){emit openSettings();});

    trayMenu->addSeparator();

    // Conversational Awareness Toggle
    caToggleAction = new QAction("Toggle Conversational Awareness", trayMenu);
    caToggleAction->setCheckable(true);
    trayMenu->addAction(caToggleAction);
    connect(caToggleAction, &QAction::triggered, this, [this](bool checked)
            { emit conversationalAwarenessToggled(checked); });

    controlSeparators.append(trayMenu->addSeparator());

    // Noise Control Options
    noiseControlGroup = new QActionGroup(trayMenu);
    const QPair<QString, NoiseControlMode> noiseOptions[] = {
        {"Adaptive", NoiseControlMode::Adaptive},
        {"Transparency", NoiseControlMode::Transparency},
        {"Noise Cancellation", NoiseControlMode::NoiseCancellation},
        {"Off", NoiseControlMode::Off}};

    for (auto option : noiseOptions)
    {
        QAction *action = new QAction(option.first, trayMenu);
        action->setCheckable(true);
        action->setData((int)option.second);
        noiseControlGroup->addAction(action);
        trayMenu->addAction(action);
        connect(action, &QAction::triggered, this, [this, mode = option.second]()
                { emit noiseControlChanged(mode); });
    }

    trayMenu->addSeparator();

    // Quit action
    QAction *quitAction = new QAction("Quit", trayMenu);
    trayMenu->addAction(quitAction);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
}

void TrayIconManager::updateIconFromBattery(const QString &status)
{
    // Parse "Left: 80%, Right: 70%, Case: 50%" or "Headset: 80%"; 0 means unavailable
    int minLevel = 0;
    for (const QString &part : status.split(", ", Qt::SkipEmptyParts))
    {
        const QStringList kv = part.split(": ");
        if (kv.size() != 2 || kv[0] == "Case")
            continue;
        const int level = QString(kv[1]).remove('%').toInt();
        if (level > 0)
            minLevel = (minLevel == 0) ? level : qMin(minLevel, level);
    }

    if (minLevel <= 0)
    {
        trayIcon->setIcon(QIcon(":/icons/assets/airpods.png"));
        return;
    }

    const int size = 64;
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    const QColor fill = minLevel <= 20 ? QColor("#FF453A") : QColor(Qt::white);

    // Battery level bar along the bottom
    const QRectF barRect(4, size - 12, size - 8, 9);
    painter.setPen(QPen(QColor(0, 0, 0, 200), 2));
    painter.setBrush(QColor(0, 0, 0, 90));
    painter.drawRoundedRect(barRect, 3, 3);
    painter.setPen(Qt::NoPen);
    painter.setBrush(fill);
    painter.drawRoundedRect(QRectF(barRect.x() + 1.5, barRect.y() + 1.5,
                                   (barRect.width() - 3) * minLevel / 100.0, barRect.height() - 3), 2, 2);

    // Percentage digits, as large as fit, with a dark outline for light taskbars
    const QString text = QString::number(minLevel);
    QFont font("Segoe UI", 10, QFont::Black);
    font.setPixelSize(text.size() >= 3 ? 34 : 46);
    QPainterPath path;
    path.addText(0, 0, font, text);
    const QRectF bounds = path.boundingRect();
    const QRectF textArea(0, 0, size, size - 14);
    path.translate(textArea.center() - bounds.center());
    painter.setPen(QPen(QColor(0, 0, 0, 220), 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(path);
    painter.setPen(Qt::NoPen);
    painter.setBrush(fill);
    painter.drawPath(path);
    painter.end();

    trayIcon->setIcon(QIcon(pixmap));
}

void TrayIconManager::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        emit trayClicked();
    }
}

