/*
 * udp_settings.h - UDP Bridge Settings Dialog
 */

#ifndef UDP_SETTINGS_H
#define UDP_SETTINGS_H

#include <QDialog>
#include <QProcess>
#include "udp_bridge.h"

namespace Ui {
class UdpSettingsDialog;
}

class UdpSettings : public QDialog {
    Q_OBJECT

public:
    explicit UdpSettings(UdpBridge *bridge, QWidget *parent = nullptr);
    ~UdpSettings();

    QString targetIp() const;
    int targetPort() const;
    UdpBridge::Protocol protocol() const;
    bool autoStart() const;

public slots:
    void accept() override;

private slots:
    void onRecenterClicked();
    void onPauseClicked();
    void onInstallWineClicked();
    void onStartStopClicked();

private:
    Ui::UdpSettingsDialog *ui;
    UdpBridge *bridge;
    
    void loadSettings();
    void saveSettings();
    void saveHotkeyIniFile();
    void updateStatus();
    
    QProcess *hotkeyProcess;
};

#endif // UDP_SETTINGS_H
