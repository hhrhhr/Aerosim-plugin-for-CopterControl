#include "udpconnect.h"
#include "enums.h"

UdpConnect::UdpConnect(QObject *parent) : QObject(parent)
{
    qDebug() << this << "UdpConnect";
    inSocket = NULL;
    outSocket = NULL;
    channel     << 0.0       << 0.0        << -1.0       << 0.0      << 0.0      << 0.0;
    channelMap  << ChAileron << ChElevator << ChThrottle << ChRudder << ChFpvPan << ChFpvTilt;
    packetsSended = 0;
    packetsRecived = 0;
    packetsLost = 0;
}

UdpConnect::~UdpConnect()
{
    qDebug() << this  << "~UdpConnect";
    if(outSocket) {
        delete outSocket;
    }
    if(inSocket) {
        delete inSocket;
    }
}

void UdpConnect::initSocket(QString &remoteHost, quint16 remotePort,
                            QString &localHost, quint16 localPort)
{
    qDebug() << this  << "initSocket" << this->thread();
    inSocket = new QUdpSocket();
    qDebug() << this << "inSocket" << inSocket->thread();
//    connect(inSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(inSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,     SLOT(onStateChanged(QAbstractSocket::SocketState)));
    connect(inSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,     SLOT(onError(QAbstractSocket::SocketError)));
    inSocket->bind(QHostAddress(localHost), localPort);

    outHost = remoteHost;
    outPort = remotePort;
    outSocket = new QUdpSocket();
    qDebug() << this << "outSocket" << outSocket->thread();
    connect(inSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,     SLOT(onStateChanged(QAbstractSocket::SocketState)));
    connect(inSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,     SLOT(onError(QAbstractSocket::SocketError)));
}

void UdpConnect::sendDatagram(const simToPlugin *stp)
{
    QByteArray data;
    data.resize(136);
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    // magic header, "AERO"
    out << quint32(0x4153494D);
    // home location
    out << stp->initPosX
        << stp->initPosY
        << stp->initPosZ;
    out << stp->wpHomeX
        << stp->wpHomeY
        << stp->wpHomeLat
        << stp->wpHomeLong;
    // position
    out << stp->posX
        << stp->posY
        << stp->posZ;
    // velocity
    out << stp->velX
        << stp->velY
        << stp->velZ;
    // angular velocity
    out << stp->angVelX
        << stp->angVelY
        << stp->angVelZ;
    // acceleration
    out << stp->accelX
        << stp->accelY
        << stp->accelZ;
    // coordinates
    out << stp->latitude
        << stp->longitude;
    // sonar
    out << stp->AGL;
    // attitude
    out << stp->heading
        << stp->pitch
        << stp->roll;
    // electric
    out << stp->voltage
        << stp->current;
    // channels
    out << stp->chSimTX[ChAileron]
        << stp->chSimTX[ChElevator]
        << stp->chSimTX[ChThrottle]
        << stp->chSimTX[ChRudder]
        << stp->chSimTX[ChPlugin1];
    // packet counter
    out << packetsSended;

    outSocket->writeDatagram(data, outHost, outPort);
    ++packetsSended;
}

/////// private slots

void UdpConnect::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
//    qDebug() << this  << error;
}
void UdpConnect::onStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << this  << state;
}

void UdpConnect::onReadyRead(pluginToSim *pts)
{
    //    qDebug() << this  << "onReadyRead";
    if (!inSocket->waitForReadyRead(8)) {
        ++packetsLost;
    } else {
        while (inSocket->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(inSocket->pendingDatagramSize());
            quint64 datagramSize;
            datagramSize = inSocket->readDatagram(datagram.data(), datagram.size());

            processDatagram(datagram);
        }
    }
        for (int i = 0; i < 6; ++i) {
            pts->chNewRX[channelMap.at(i)] = channel.at(i);
            pts->chOverRX[channelMap.at(i)] = TRUE;
        }
}

void UdpConnect::getStats(quint32 &s, quint32 &r, quint32 &l)
{
    s = packetsSended;
    r = packetsRecived;
    l = packetsLost;
}

////// private

void UdpConnect::processDatagram(QByteArray &datagram)
{
    QDataStream stream(datagram);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    // check magic header
    quint32 magic;
    stream >> magic;
    if (magic != 0x52434D44) // "RCMD"
        return;
    // read channels
    for (int i = 0; i < 6; ++i)
        stream >> channel[i];
    // read counter
    stream >> packetsRecived;
}
