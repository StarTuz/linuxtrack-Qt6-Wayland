/*
 * udp_bridge.cpp - UDP Pose Broadcasting for Linuxtrack GUI
 */

#include "udp_bridge.h"
#include <QByteArray>
#include <QFile>
#include <cstring>

// OpenTrack UDP Packet: 6 doubles (48 bytes)
#pragma pack(push, 1)
struct OpenTrackPacket {
    double x;     // cm
    double y;     // cm
    double z;     // cm
    double yaw;   // degrees
    double pitch; // degrees
    double roll;  // degrees
};

// FreeTrack UDP Packet
struct FreeTrackPacket {
    int32_t data_id;
    float cam_width;
    float cam_height;
    float yaw;
    float pitch;
    float roll;
    float x;
    float y;
    float z;
};
#pragma pack(pop)

UdpBridge::UdpBridge(QObject *parent)
    : QObject(parent)
    , socket(new QUdpSocket(this))
    , commandSocket(new QUdpSocket(this))
    , address(QHostAddress::LocalHost)
    , port(4242)
    , proto(OpenTrack)
    , running(false)
    , ltrUdpProcess(nullptr)
{
    // Listen for commands from hotkey utility on port 4243
    commandSocket->bind(QHostAddress::LocalHost, 4243);
    connect(commandSocket, &QUdpSocket::readyRead, this, &UdpBridge::onCommandReadyRead);
}

UdpBridge::~UdpBridge()
{
    stop();
}

void UdpBridge::setTarget(const QString &ip, int p)
{
    address = QHostAddress(ip);
    port = static_cast<quint16>(p);
}

void UdpBridge::setProtocol(Protocol p)
{
    proto = p;
}

QString UdpBridge::targetAddress() const
{
    return address.toString();
}

void UdpBridge::start()
{
    if (running) return;
    
    // Spawn ltr_udp process
    if (!ltrUdpProcess) {
        ltrUdpProcess = new QProcess(this);
    }
    
    // Build arguments based on settings
    QStringList args;
    args << QString::fromLatin1("--ip=") + address.toString();
    args << QString::fromLatin1("--port=") + QString::number(port);
    if (proto == FreeTrack) {
        args << QString::fromLatin1("--proto=freetrack");
    }
    
    // Find ltr_udp binary
    QString ltrUdpPath = QString::fromLatin1("/opt/linuxtrack/bin/ltr_udp");
    if (!QFile::exists(ltrUdpPath)) {
        // Try build directory  
        ltrUdpPath = QString::fromLatin1("/home/startux/Code/linuxtrackfixed/linuxtrack/build_local/src/ltr_udp");
    }
    
    if (QFile::exists(ltrUdpPath)) {
        ltrUdpProcess->start(ltrUdpPath, args);
        if (ltrUdpProcess->waitForStarted(3000)) {
            running = true;
            emit statusChanged(true);
        } else {
            emit error(QString::fromLatin1("Failed to start ltr_udp"));
        }
    } else {
        emit error(QString::fromLatin1("ltr_udp not found"));
    }
}

void UdpBridge::stop()
{
    if (!running) return;
    
    if (ltrUdpProcess && ltrUdpProcess->state() != QProcess::NotRunning) {
        ltrUdpProcess->terminate();
        if (!ltrUdpProcess->waitForFinished(2000)) {
            ltrUdpProcess->kill();
        }
    }
    
    running = false;
    emit statusChanged(false);
}

void UdpBridge::sendPose(float yaw, float pitch, float roll, float x, float y, float z)
{
    if (!running) return;
    
    QByteArray data;
    
    if (proto == OpenTrack) {
        OpenTrackPacket pkt;
        pkt.x = static_cast<double>(x) / 10.0;  // mm -> cm
        pkt.y = static_cast<double>(y) / 10.0;
        pkt.z = static_cast<double>(z) / 10.0;
        pkt.yaw = static_cast<double>(yaw);
        pkt.pitch = static_cast<double>(pitch);
        pkt.roll = static_cast<double>(roll);
        
        data = QByteArray(reinterpret_cast<const char*>(&pkt), sizeof(pkt));
    } else {
        FreeTrackPacket pkt;
        pkt.data_id = 2;
        pkt.cam_width = 0;
        pkt.cam_height = 0;
        pkt.yaw = yaw;
        pkt.pitch = pitch;
        pkt.roll = roll;
        pkt.x = x;
        pkt.y = y;
        pkt.z = z;
        
        data = QByteArray(reinterpret_cast<const char*>(&pkt), sizeof(pkt));
    }
    
    socket->writeDatagram(data, address, port);
}

void UdpBridge::sendCommand(const char *cmd)
{
    // Send 4-byte command (RECN or PAUS)
    QByteArray data(cmd, 4);
    socket->writeDatagram(data, address, port);
}

void UdpBridge::onCommandReadyRead()
{
    while (commandSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(static_cast<int>(commandSocket->pendingDatagramSize()));
        commandSocket->readDatagram(datagram.data(), datagram.size());
        
        if (datagram.size() >= 4) {
            QString cmd = QString::fromLatin1(datagram.data(), 4);
            emit commandReceived(cmd);
        }
    }
}

#include "moc_udp_bridge.cpp"
