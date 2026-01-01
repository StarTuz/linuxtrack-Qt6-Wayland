/*
 * udp_bridge.h - UDP Pose Broadcasting for Linuxtrack GUI
 *
 * Broadcasts pose data via UDP in OpenTrack or FreeTrack format.
 */

#ifndef UDP_BRIDGE_H
#define UDP_BRIDGE_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QProcess>

class UdpBridge : public QObject {
    Q_OBJECT

public:
    enum Protocol { OpenTrack, FreeTrack };
    Q_ENUM(Protocol)

    explicit UdpBridge(QObject *parent = nullptr);
    ~UdpBridge();

    void setTarget(const QString &ip, int port);
    void setProtocol(Protocol p);
    
    bool isRunning() const { return running; }
    QString targetAddress() const;
    int targetPort() const { return port; }
    Protocol protocol() const { return proto; }

public slots:
    void start();
    void stop();
    void sendPose(float yaw, float pitch, float roll, float x, float y, float z);
    void sendCommand(const char *cmd); // "RECN" or "PAUS"

signals:
    void statusChanged(bool running);
    void error(const QString &message);
    void commandReceived(const QString &cmd);

private slots:
    void onCommandReadyRead();

private:
    QUdpSocket *socket;
    QUdpSocket *commandSocket;
    QHostAddress address;
    quint16 port;
    Protocol proto;
    bool running;
    QProcess *ltrUdpProcess;
};

#endif // UDP_BRIDGE_H
