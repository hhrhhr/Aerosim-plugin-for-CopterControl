#include "udpconnect.h"

UdpConnect::UdpConnect(QObject *parent) : QObject(parent)
{
    qDebug() << this << "UdpConnect";
    inSocket = NULL;
    outSocket = NULL;
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

//void UdpConnect::sendDatagram(const QByteArray &data)
void UdpConnect::sendDatagram(const simToPlugin *stp)
{
//    outSocket.writeDatagram(data, outHost, outPort);
    static quint32 packetCounter = 0;
    QByteArray data;
    data.resize(116);
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    // magic header, "AERO", 1095977293
    out << quint32(0x4153494D);
    // home location
    out << stp->initPosX << stp->initPosY << stp->initPosZ;
    out << stp->wpHomeX << stp->wpHomeY << stp->wpHomeLat << stp->wpHomeLong;
    // position
    out << stp->posX << stp->posY << stp->posZ;
    // velocity
    out << stp->velX << stp->velY << stp->velZ;
    // angular velocity
    out << stp->angVelX << stp->angVelY << stp->angVelZ;
    // acceleration
    out << stp->accelX << stp->accelY << stp->accelZ;
    // coordinates
    out << stp->latitude << stp->longitude;
    // sonar
    out << stp->AGL;
    // attitude
    out << stp->heading << stp->pitch << stp->roll;
    // electric
    out << stp->voltage << stp->current;
    // packet counter
    out << packetCounter;

    outSocket->writeDatagram(data, outHost, outPort);
    ++packetCounter;
}


/////// private slots

void UdpConnect::onError(QAbstractSocket::SocketError error)
{
    qDebug() << this  << error;
}
void UdpConnect::onStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << this  << state;
}

void UdpConnect::onReadyRead()
{
//    qDebug() << this  << "onReadyRead";

//    inSocket.waitForReadyRead(3);
//    while (inSocket.hasPendingDatagrams()) {
//        QByteArray datagram;
//        datagram.resize(inSocket.pendingDatagramSize());
//        quint64 datagramSize;
//        datagramSize = inSocket.readDatagram(datagram.data(), datagram.size());
//        processDatagram(datagram);
//    }
}

////// private

void UdpConnect::processDatagram(QByteArray &datagram)
{
    Q_UNUSED(datagram);
    /*
    QDataStream stream(datagram);
    // stream.setFloatingPointPrecision();
    // check magic header
    quint32 magic;
    stream >> magic;
    QTextStream out(&udplog);
    out << magic << "\n";
    out.flush();
    if(magic == 0x52434D44) {  // "RCMD"
        // read channels
        stream >> channel[0];
        stream >> channel[1];
        stream >> channel[2];
        stream >> channel[3];
        stream >> channel[4];
        stream >> channel[5];

        // read counter
        stream >> recvPacketCounter;
    } else {
        QTextStream out(&udplog);
        out << "udplog: wrong magic: " << magic;
    }
    */
}
