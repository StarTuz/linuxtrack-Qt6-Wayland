/*
 * udp_settings.cpp - UDP Bridge Settings Dialog
 */

#include "udp_settings.h"
#include "ui_udp_settings.h"
#include "prefix_discovery_dialog.h"
#include <QSettings>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QDateTime>
#include "tracker.h"

UdpSettings::UdpSettings(UdpBridge *b, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UdpSettingsDialog)
    , bridge(b)
    , hotkeyProcess(nullptr)
{
    ui->setupUi(this);
    
    connect(ui->recenterButton, &QPushButton::clicked, this, &UdpSettings::onRecenterClicked);
    connect(ui->pauseButton, &QPushButton::clicked, this, &UdpSettings::onPauseClicked);
    connect(ui->installWineButton, &QPushButton::clicked, this, &UdpSettings::onInstallWineClicked);
    connect(ui->startStopButton, &QPushButton::clicked, this, &UdpSettings::onStartStopClicked);
    
    if (bridge) {
        connect(bridge, &UdpBridge::commandReceived, this, [this](const QString &cmd){
            if (cmd == QString::fromLatin1("RECN")) onRecenterClicked();
            else if (cmd == QString::fromLatin1("PAUS")) onPauseClicked();
        });
        connect(bridge, &UdpBridge::statusChanged, this, [this](bool running){
            (void)running;
            updateStatus();
        });
    }
    
    loadSettings();
    updateStatus();
}

UdpSettings::~UdpSettings()
{
    // Stop hotkey process if running
    if (hotkeyProcess && hotkeyProcess->state() != QProcess::NotRunning) {
        hotkeyProcess->terminate();
        hotkeyProcess->waitForFinished(1000);
    }
    delete hotkeyProcess;
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
    
    // Also send RECN to DLL to reset its client-side offsets
    if (bridge) {
        bridge->sendCommand("RECN");
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
    
    // Find the source DLLs (from install location or build directory)
    QString libDir = QString::fromLatin1("/opt/linuxtrack/lib/linuxtrack");
    if (!QFile::exists(libDir + QString::fromLatin1("/NPClient64UDP.dll.so"))) {
        // Try build directory
        libDir = QString::fromLatin1("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/wine_bridge");
    }
    
    bool success = true;
    
    // Copy 64-bit DLL to Program Files
    QString src64 = libDir + QString::fromLatin1("/NPClient64UDP.dll.so");
    QString dst64 = destDir64 + QString::fromLatin1("/NPClient64.dll");
    if (QFile::exists(src64)) {
        QFile::remove(dst64);
        if (!QFile::copy(src64, dst64)) success = false;
    }
    
    // Copy 32-bit DLL to both directories (games may look in either)
    QString src32 = libDir + QString::fromLatin1("/NPClientUDP.dll.so");
    if (QFile::exists(src32)) {
        // Program Files (for 64-bit loader looking for 32-bit compat)
        QString dst32_64 = destDir64 + QString::fromLatin1("/NPClient.dll");
        QFile::remove(dst32_64);
        if (!QFile::copy(src32, dst32_64)) success = false;
        
        // Program Files (x86) (for native 32-bit games)
        QString dst32_32 = destDir32 + QString::fromLatin1("/NPClient.dll");
        QFile::remove(dst32_32);
        if (!QFile::copy(src32, dst32_32)) success = false;
    }
    
    // Copy hotkey utility to 64-bit dir
    QString srcHotkey = libDir + QString::fromLatin1("/ltr_wine_hotkeys.exe");
    QString dstHotkey = destDir64 + QString::fromLatin1("/ltr_wine_hotkeys.exe");
    if (QFile::exists(srcHotkey)) {
        QFile::remove(dstHotkey);
        if (!QFile::copy(srcHotkey, dstHotkey)) success = false;
    }
    
    // Apply registry entries - write directly to user.reg for Proton compatibility
    QString userRegPath = prefix + QString::fromLatin1("/user.reg");
    bool regApplied = false;
    
    // Method 1: Direct write to user.reg (works with Proton)
    if (QFile::exists(userRegPath)) {
        QFile userReg(userRegPath);
        if (userReg.open(QIODevice::ReadWrite | QIODevice::Text)) {
            QString content = QString::fromUtf8(userReg.readAll());
            
            // Check if entry already exists
            if (!content.contains(QString::fromLatin1("NPClient Location"))) {
                // Append registry entry
                QString entry = QString::fromLatin1(
                    "\n[Software\\\\NaturalPoint\\\\NATURALPOINT\\\\NPClient Location] %1\n"
                    "\"Path\"=\"C:\\\\Program Files\\\\Linuxtrack\\\\\"\n"
                ).arg(QDateTime::currentSecsSinceEpoch());
                
                userReg.seek(userReg.size());
                userReg.write(entry.toUtf8());
                regApplied = true;
            } else {
                regApplied = true; // Already exists
            }
            userReg.close();
        }
    }
    
    // Method 2: Also try wine regedit as fallback (for regular Wine prefixes)
    QString regFile = QString::fromLatin1("/opt/linuxtrack/share/linuxtrack/ltr_udp.reg");
    if (!QFile::exists(regFile)) {
        regFile = QString::fromLatin1("/home/startux/Code/linuxtrackfixed/linuxtrack/ltr_udp.reg");
    }
    
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
        QMessageBox::information(this, QString::fromLatin1("Success"),
            QString::fromLatin1("UDP Bridge installed to:\n") + destDir64 + 
            QString::fromLatin1("\nand\n") + destDir32);
    } else {
        QMessageBox::warning(this, QString::fromLatin1("Warning"),
            QString::fromLatin1("Some files could not be copied. Check if ltr_gui is installed or built."));
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
    if (bridge && bridge->isRunning()) {
        // Stop the stack
        bridge->stop();
        if (hotkeyProcess && hotkeyProcess->state() != QProcess::NotRunning) {
            hotkeyProcess->terminate();
            hotkeyProcess->waitForFinished(1000);
        }
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
        
        // Start hotkey utility if enabled
        if (ui->enableHotkeysCheck->isChecked()) {
            if (!hotkeyProcess) {
                hotkeyProcess = new QProcess(this);
            }
            // Path to hotkey exe - in prefix or installed location
            QString exePath = QString::fromLatin1("/opt/linuxtrack/lib/linuxtrack/ltr_wine_hotkeys.exe");
            if (QFile::exists(exePath)) {
                hotkeyProcess->start(QString::fromLatin1("wine"), QStringList() << exePath);
            }
        }
    }
    updateStatus();
}

void UdpSettings::updateStatus()
{
    if (bridge && bridge->isRunning()) {
        ui->startStopButton->setText(QString::fromLatin1("Stop UDP Stack"));
        ui->statusLabel->setText(QString::fromLatin1("Status: Running"));
    } else {
        ui->startStopButton->setText(QString::fromLatin1("Start UDP Stack"));
        ui->statusLabel->setText(QString::fromLatin1("Status: Stopped"));
    }
}

#include "moc_udp_settings.cpp"


