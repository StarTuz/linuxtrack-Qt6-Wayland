/*
 * udp_settings.cpp - UDP Bridge Settings Dialog
 */

#include "udp_settings.h"
#include "ui_udp_settings.h"
#include "ltr_gui_prefs.h"
#include "prefix_discovery_dialog.h"
#include <QSettings>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QDateTime>
#include <QCoreApplication>
#include "tracker.h"

UdpSettings::UdpSettings(UdpBridge *b, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UdpSettingsDialog)
    , bridge(b)
    , hotkeyProcess(nullptr)  // Not owned by dialog - will be reparented to main window if started
    , stackRunning(b ? b->isRunning() : false)
{
    ui->setupUi(this);
    
    connect(ui->recenterButton, &QPushButton::clicked, this, &UdpSettings::onRecenterClicked);
    connect(ui->pauseButton, &QPushButton::clicked, this, &UdpSettings::onPauseClicked);
    connect(ui->installWineButton, &QPushButton::clicked, this, &UdpSettings::onInstallWineClicked);
    connect(ui->startStopButton, &QPushButton::clicked, this, &UdpSettings::onStartStopClicked);
    
    loadSettings();
    updateStatus();
}

UdpSettings::~UdpSettings()
{
    // NOTE: hotkeyProcess is now owned by LinuxtrackGui, not this dialog.
    // We don't terminate it here - it should persist after dialog closes.
    // If dialog started a local process, reparent it to main window.
    if (hotkeyProcess && hotkeyProcess->state() != QProcess::NotRunning) {
        hotkeyProcess->setParent(parentWidget());  // Reparent to main window
    }
    delete ui;
}

void UdpSettings::loadSettings()
{
    QSettings settings(QString::fromLatin1("linuxtrack"), QString::fromLatin1("ltr_gui"));
    settings.beginGroup(QString::fromLatin1("UDP"));
    
    ui->ipEdit->setText(settings.value(QString::fromLatin1("TargetIP"), QString::fromLatin1("127.0.0.1")).toString());
    ui->portSpinBox->setValue(settings.value(QString::fromLatin1("Port"), 4242).toInt());
    ui->protocolCombo->setCurrentIndex(settings.value(QString::fromLatin1("Protocol"), 0).toInt());
    ui->autoStartCheck->setChecked(settings.value(QString::fromLatin1("AutoStart"), true).toBool());
    
    // Hotkey settings
    ui->enableHotkeysCheck->setChecked(settings.value(QString::fromLatin1("EnableHotkeys"), false).toBool());
    QString recenterKey = settings.value(QString::fromLatin1("RecenterKey"), QString::fromLatin1("F12")).toString();
    QString pauseKey = settings.value(QString::fromLatin1("PauseKey"), QString::fromLatin1("Pause")).toString();
    ui->recenterKeyEdit->setKeySequence(QKeySequence(recenterKey));
    ui->pauseKeyEdit->setKeySequence(QKeySequence(pauseKey));
    
    settings.endGroup();
}

void UdpSettings::saveSettings()
{
    QSettings settings(QString::fromLatin1("linuxtrack"), QString::fromLatin1("ltr_gui"));
    settings.beginGroup(QString::fromLatin1("UDP"));
    
    settings.setValue(QString::fromLatin1("TargetIP"), ui->ipEdit->text());
    settings.setValue(QString::fromLatin1("Port"), ui->portSpinBox->value());
    settings.setValue(QString::fromLatin1("Protocol"), ui->protocolCombo->currentIndex());
    settings.setValue(QString::fromLatin1("AutoStart"), ui->autoStartCheck->isChecked());
    
    // Hotkey settings
    settings.setValue(QString::fromLatin1("EnableHotkeys"), ui->enableHotkeysCheck->isChecked());
    settings.setValue(QString::fromLatin1("RecenterKey"), ui->recenterKeyEdit->keySequence().toString());
    settings.setValue(QString::fromLatin1("PauseKey"), ui->pauseKeyEdit->keySequence().toString());
    
    settings.endGroup();
}

QString UdpSettings::targetIp() const
{
    return ui->ipEdit->text();
}

int UdpSettings::targetPort() const
{
    return ui->portSpinBox->value();
}

UdpBridge::Protocol UdpSettings::protocol() const
{
    return static_cast<UdpBridge::Protocol>(ui->protocolCombo->currentIndex());
}

bool UdpSettings::autoStart() const
{
    return ui->autoStartCheck->isChecked();
}

void UdpSettings::accept()
{
    saveSettings();
    saveHotkeyIniFile(); // Write ini file for Wine hotkey utility
    
    if (bridge) {
        bridge->setTarget(targetIp(), targetPort());
        bridge->setProtocol(protocol());
    }
    
    QDialog::accept();
}

void UdpSettings::onRecenterClicked()
{
    // Call server-side recenter via TRACKER
    // This affects the data ltr_udp sends
    TRACKER.recenter();
    
    // Also tell the DLL to clear any Wine-side offsets; the host has recentered.
    if (bridge) {
        bridge->sendCommand("RSET");
    }
}

void UdpSettings::onPauseClicked()
{
    if (bridge) {
        bridge->sendCommand("PAUS");
    }
}

void UdpSettings::onInstallWineClicked()
{
    // Use existing prefix discovery dialog
    PrefixDiscoveryDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    
    QString prefix = dialog.getSelectedPath();
    if (prefix.isEmpty()) {
        return;
    }
    
    // Create both 32-bit and 64-bit destination directories
    QString destDir64 = prefix + QString::fromLatin1("/drive_c/Program Files/Linuxtrack");
    QString destDir32 = prefix + QString::fromLatin1("/drive_c/Program Files (x86)/Linuxtrack");
    QDir().mkpath(destDir64);
    QDir().mkpath(destDir32);
    
    // Find and copy the Wine/Proton bridge files. Some games probe
    // NPClient.dll even from 64-bit processes, so install both names.
    bool success = true;
    QStringList installNotes;
    QStringList sourcePaths;
    sourcePaths << QString::fromLatin1("../lib/linuxtrack")
                << QString::fromLatin1("../lib")
                << QString::fromLatin1("../../src/wine_bridge/client")
                << QString::fromLatin1("../../src/wine_bridge/controller")
                << QString::fromLatin1("../../src/wine_bridge")
                << QString::fromLatin1("../../wine_bridge/client")
                << QString::fromLatin1("../../wine_bridge/controller")
                << QString::fromLatin1("../../wine_bridge");

    auto copyBridgeFile = [&](const QString &src, const QString &dst,
                              const QString &label, bool required) {
        if (!QFile::exists(src)) {
            if (required) {
                installNotes << QString::fromLatin1("Missing %1 source").arg(label);
                success = false;
            } else {
                installNotes << QString::fromLatin1(
                    "%1 was not included in this build.\n"
                    "This is optional and only needed for legacy 32-bit games.")
                    .arg(label);
            }
            return;
        }

        QFile::remove(dst);
        if (!QFile::copy(src, dst) || !QFile::exists(dst)) {
            installNotes << QString::fromLatin1("Failed to copy %1 to %2")
                                .arg(label, dst);
            success = false;
        }
    };

    QString src64 = PrefProxy::findRuntimeFile(
        QString::fromLatin1("NPClient64UDP.dll.so"), sourcePaths);
    copyBridgeFile(src64, destDir64 + QString::fromLatin1("/NPClient64.dll"),
                   QString::fromLatin1("NPClient64UDP.dll.so"), true);
    copyBridgeFile(src64, destDir64 + QString::fromLatin1("/NPClient.dll"),
                   QString::fromLatin1("NPClient64UDP.dll.so"), true);

    QString src32 = PrefProxy::findRuntimeFile(
        QString::fromLatin1("NPClientUDP.dll.so"), sourcePaths);
    copyBridgeFile(src32, destDir32 + QString::fromLatin1("/NPClient.dll"),
                   QString::fromLatin1("NPClientUDP.dll.so"), false);

    QString srcHotkey = PrefProxy::findRuntimeFile(
        QString::fromLatin1("ltr_wine_hotkeys.exe"), sourcePaths);
    copyBridgeFile(srcHotkey,
                   destDir64 + QString::fromLatin1("/ltr_wine_hotkeys.exe"),
                   QString::fromLatin1("ltr_wine_hotkeys.exe"), false);
    
    // Apply registry entries - write directly to user.reg for Proton compatibility
    QString userRegPath = prefix + QString::fromLatin1("/user.reg");
    bool regApplied = false;
    
    // Method 1: Direct write to user.reg (works with Proton)
    if (QFile::exists(userRegPath)) {
        QFile userReg(userRegPath);
        if (userReg.open(QIODevice::ReadWrite | QIODevice::Text)) {
            QString content = QString::fromUtf8(userReg.readAll());

            // Always replace the NaturalPoint NPClient path. A stale key can
            // point games at an old TrackIR/Linuxtrack install and prevent our
            // bridge DLL from loading at all.
            QStringList lines = content.split(QLatin1Char('\n'));
            QStringList filtered;
            bool skippingNpClientBlock = false;
            const QString npClientHeader =
                QString::fromLatin1("[Software\\\\NaturalPoint\\\\NATURALPOINT\\\\NPClient Location]");
            for (const QString &line : lines) {
                if (line.startsWith(npClientHeader)) {
                    skippingNpClientBlock = true;
                    continue;
                }
                if (skippingNpClientBlock && line.startsWith(QLatin1Char('['))) {
                    skippingNpClientBlock = false;
                }
                if (!skippingNpClientBlock) {
                    filtered << line;
                }
            }

            QString entry = QString::fromLatin1(
                "\n[Software\\\\NaturalPoint\\\\NATURALPOINT\\\\NPClient Location] %1\n"
                "\"Path\"=\"C:\\\\Program Files\\\\Linuxtrack\\\\\"\n"
            ).arg(QDateTime::currentSecsSinceEpoch());

            content = filtered.join(QLatin1Char('\n'));
            if (!content.endsWith(QLatin1Char('\n'))) {
                content += QLatin1Char('\n');
            }
            content += entry;

            userReg.resize(0);
            userReg.seek(0);
            userReg.write(content.toUtf8());
            regApplied = true;
            userReg.close();
        }
    }
    
    // Method 2: Also try wine regedit as fallback (for regular Wine prefixes)
    QString regFile = PrefProxy::findRuntimeFile(
        QString::fromLatin1("ltr_udp.reg"),
        QStringList()
            << QString::fromLatin1("../Resources/linuxtrack")
            << QString::fromLatin1("../share/linuxtrack")
            << QString::fromLatin1("../../..")
            << QString::fromLatin1(".."));
    
    if (QFile::exists(regFile)) {
        QProcess proc;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert(QString::fromLatin1("WINEPREFIX"), prefix);
        proc.setProcessEnvironment(env);
        proc.start(QString::fromLatin1("wine"), QStringList() << QString::fromLatin1("regedit") << regFile);
        proc.waitForFinished(10000);
        // Don't warn on failure if direct write succeeded
    }
    
    if (!regApplied) {
        QMessageBox::warning(this, QString::fromLatin1("Warning"),
            QString::fromLatin1("Could not apply registry. Tracking may not work.\n"
                "You may need to manually add:\n"
                "[HKCU\\Software\\NaturalPoint\\NATURALPOINT\\NPClient Location]\n"
                "Path=C:\\Program Files\\Linuxtrack\\"));
    }
    
    // Copy current hotkey configuration to prefix for per-game customization
    saveHotkeyIniFile(); // Ensure latest settings are saved
    QString globalIni = QDir::homePath() + QString::fromLatin1("/.config/linuxtrack/ltr_hotkeys.ini");
    if (QFile::exists(globalIni)) {
        QString prefixIni64 = destDir64 + QString::fromLatin1("/ltr_hotkeys.ini");
        QString prefixIni32 = destDir32 + QString::fromLatin1("/ltr_hotkeys.ini");
        QFile::remove(prefixIni64);
        QFile::copy(globalIni, prefixIni64);
        QFile::remove(prefixIni32);
        QFile::copy(globalIni, prefixIni32);
    }
    
    if (success) {
        ui->protocolCombo->setCurrentIndex(static_cast<int>(UdpBridge::WineNpClient));
        saveSettings();
        if (bridge) {
            bridge->setProtocol(UdpBridge::WineNpClient);
        }
        QString message = QString::fromLatin1("UDP Bridge installed to:\n") +
                          destDir64 + QString::fromLatin1("\nand\n") +
                          destDir32;
        if (!installNotes.isEmpty()) {
            message += QString::fromLatin1("\n\nNotes:\n") +
                       installNotes.join(QString::fromLatin1("\n"));
        }
        QMessageBox::information(this, QString::fromLatin1("Success"),
            message);
    } else {
        QString message =
            QString::fromLatin1("Wine/Proton UDP bridge installation is incomplete.\n\n");
        if (!installNotes.isEmpty()) {
            message += installNotes.join(QString::fromLatin1("\n")) +
                       QString::fromLatin1("\n\n");
        }
        message += QString::fromLatin1(
            "Check that this Linuxtrack build or AppImage contains the Wine bridge files.");
        QMessageBox::warning(this, QString::fromLatin1("Warning"),
            message);
    }
}

// Convert Qt key to Windows VK code (simplified mapping)
static int qtKeyToVk(int qtKey)
{
    // Function keys
    if (qtKey >= Qt::Key_F1 && qtKey <= Qt::Key_F24)
        return 0x70 + (qtKey - Qt::Key_F1); // VK_F1 = 0x70
    
    // Special keys
    switch (qtKey) {
    case Qt::Key_Pause: return 0x13; // VK_PAUSE
    case Qt::Key_Escape: return 0x1B;
    case Qt::Key_Space: return 0x20;
    case Qt::Key_Home: return 0x24;
    case Qt::Key_End: return 0x23;
    case Qt::Key_Insert: return 0x2D;
    case Qt::Key_Delete: return 0x2E;
    case Qt::Key_ScrollLock: return 0x91;
    case Qt::Key_NumLock: return 0x90;
    default:
        // Letters A-Z
        if (qtKey >= Qt::Key_A && qtKey <= Qt::Key_Z)
            return qtKey; // Qt::Key_A == 'A' == VK_A
        // Numbers 0-9
        if (qtKey >= Qt::Key_0 && qtKey <= Qt::Key_9)
            return qtKey;
        return 0x7B; // VK_F12 as fallback
    }
}

void UdpSettings::saveHotkeyIniFile()
{
    // Write ini file that ltr_wine_hotkeys.exe reads
    // Format: [Hotkeys] Recenter=<VK_CODE> Pause=<VK_CODE>
    
    QString configDir = QDir::homePath() + QString::fromLatin1("/.config/linuxtrack");
    QDir().mkpath(configDir);
    QString iniPath = configDir + QString::fromLatin1("/ltr_hotkeys.ini");
    
    QKeySequence recenterSeq = ui->recenterKeyEdit->keySequence();
    QKeySequence pauseSeq = ui->pauseKeyEdit->keySequence();
    
    int recenterVk = 0x7B; // VK_F12 default
    int pauseVk = 0x13;    // VK_PAUSE default
    
    if (!recenterSeq.isEmpty())
        recenterVk = qtKeyToVk(recenterSeq[0].key());
    if (!pauseSeq.isEmpty())
        pauseVk = qtKeyToVk(pauseSeq[0].key());
    
    QFile file(iniPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << QString::fromLatin1("[Hotkeys]\n");
        out << QString::fromLatin1("Recenter=") << recenterVk << QString::fromLatin1("\n");
        out << QString::fromLatin1("Pause=") << pauseVk << QString::fromLatin1("\n");
        file.close();
    }
}

void UdpSettings::onStartStopClicked()
{
    if (stackRunning) {
        // Stop the stack
        if (bridge) {
            bridge->stop();
        }
        if (hotkeyProcess && hotkeyProcess->state() != QProcess::NotRunning) {
            hotkeyProcess->terminate();
            hotkeyProcess->waitForFinished(1000);
        }
        stackRunning = false;
    } else {
        // Start the stack
        
        // Apply current settings
        saveSettings();
        saveHotkeyIniFile();
        
        if (bridge) {
            bridge->setTarget(targetIp(), targetPort());
            bridge->setProtocol(protocol());
            bridge->start();
        }
        
        // Start native hotkey daemon if enabled
        if (ui->enableHotkeysCheck->isChecked()) {
            if (!hotkeyProcess) {
                hotkeyProcess = new QProcess(this);
                connect(hotkeyProcess, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
                    qWarning() << "ltr_hotkeyd error:" << error;
                });
            }
            
            QString exePath =
                PrefProxy::getExecutablePath(QString::fromLatin1("ltr_hotkeyd"));
            
            if (!exePath.isEmpty()) {
                qDebug() << "Starting ltr_hotkeyd from:" << exePath;
                hotkeyProcess->start(exePath, QStringList());
            } else {
                qWarning() << "ltr_hotkeyd not found in any expected location";
            }
        }
        
        stackRunning = true;
    }
    updateStatus();
}

void UdpSettings::updateStatus()
{
    if (stackRunning) {
        ui->startStopButton->setText(QString::fromLatin1("Stop UDP Stack"));
        ui->statusLabel->setText(QString::fromLatin1("Status: Running"));
    } else {
        ui->startStopButton->setText(QString::fromLatin1("Start UDP Stack"));
        ui->statusLabel->setText(QString::fromLatin1("Status: Stopped"));
    }
}

#include "moc_udp_settings.cpp"
