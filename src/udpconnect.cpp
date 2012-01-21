#include "udpconnect.h"
#include "enums.h"

UdpSender::UdpSender(QObject *parent) : QObject(parent)
{
    qDebug() << this << "UdpSender::UdpSender thread:" << thread();
    outSocket = NULL;
    packetsSended = 1;
}

UdpSender::~UdpSender()
{
    qDebug() << this  << "UdpSender::~UdpSender";
    if (outSocket)
        delete outSocket;
}

// public
void UdpSender::init(const QString &remoteHost, quint16 remotePort)
{
    qDebug() << this << "UdpSender::init";
    outHost = remoteHost;
    outPort = remotePort;
    outSocket = new QUdpSocket();
}

void UdpSender::sendDatagram(const simToPlugin *stp)
{
    QByteArray data;
    data.resize(140);
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    // magic header, "AERO"
    out << quint32(0x4153494D);
    // home location
    out << stp->initPosX    << stp->initPosY    << stp->initPosZ;
    out << stp->wpHomeX     << stp->wpHomeY     << stp->wpHomeLat   << stp->wpHomeLong;
    // position
    out << stp->posX        << stp->posY        << stp->posZ;
    // velocity
    out << stp->velX        << stp->velY        << stp->velZ;
    // angular velocity
    out << stp->angVelX     << stp->angVelY     << stp->angVelZ;
    // acceleration
    out << stp->accelX      << stp->accelY      << stp->accelZ;
    // coordinates
    out << stp->latitude    << stp->longitude;
    // sonar
    out << stp->AGL;
    // attitude
    out << stp->heading     << stp->pitch       << stp->roll;
    // electric
    out << stp->voltage     << stp->current;
    // channels
    out << stp->chSimTX[ChAileron]
        << stp->chSimTX[ChElevator]
        << stp->chSimTX[ChThrottle]
        << stp->chSimTX[ChRudder]
        << stp->chSimTX[ChPlugin1]
        << stp->chSimTX[ChPlugin2];
    // packet counter
    out << packetsSended;

    outSocket->writeDatagram(data, outHost, outPort);
    ++packetsSended;
}

/***********************************************************************************************
 ***********************************************************************************************
 */

UdpReciever::UdpReciever(const QList<quint8> map,
                         bool isOutputToTX,
                         QObject *parent)
    : QThread(parent)
{
    qDebug() << this << "UdpReciever::UdpReciever thread:" << thread();

    stopped = false;
    inSocket = NULL;
    for (int i = 0; i < 10; ++i)
        channels << 0.f;
    channels[2] = -1.f;
    channelsMap = map;
    outputToTX = isOutputToTX;
    packetsRecived = 1;
}

UdpReciever::~UdpReciever()
{
    qDebug() << this  << "UdpReciever::~UdpReciever";
    if (inSocket)
        delete inSocket;
}

// public
void UdpReciever::init(const QString &localHost, quint16 localPort)
{
    qDebug() << this << "UdpReciever::init";

    inSocket = new QUdpSocket();
    qDebug() << this << "inSocket constructed" << inSocket->thread();

    inSocket->bind(QHostAddress(localHost), localPort);
}

void UdpReciever::run()
{
    qDebug() << this << "UdpReciever::run start";
    while (!stopped)
        onReadyRead();
    qDebug() << this << "UdpReciever::run ended";
}

void UdpReciever::stop()
{
    qDebug() << this << "UdpReciever::stop";
    stopped = true;
}

volatile void UdpReciever::getChannels(pluginToSim *pts)
{
    float channelValue;
    for (int i = 0; i < 10; ++i) {
        channelValue = qBound(-1.f, channels.at(i), 1.f);
        if (outputToTX) {
            // connect to simulators transmitter
            pts->chNewTX[channelsMap.at(i)] = channelValue;
            pts->chOverTX[channelsMap.at(i)] = true;
        } else {
            // direct connect to ESC/motors/ailerons/etc
            pts->chNewRX[channelsMap.at(i)] = channelValue;
            pts->chOverRX[channelsMap.at(i)] = true;
        }
    }
}

// private
void UdpReciever::onReadyRead()
{
    if (!inSocket->waitForReadyRead(8)) // 1/60fps ~= 16.7ms, 1/120fps ~= 8.3ms
        return;
    // TODO: add failsave
    // if a command not recieved then slowly return all channel to neutral
    //
    while (inSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(inSocket->pendingDatagramSize());
        quint64 datagramSize;
        datagramSize = inSocket->readDatagram(datagram.data(), datagram.size());

        processDatagram(datagram);
    }
}

void UdpReciever::processDatagram(QByteArray &datagram)
{
    QDataStream stream(datagram);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    // check magic header
    quint32 magic;
    stream >> magic;
    if (magic != 0x52434D44) // "RCMD"
        return;
    // read channels
    for (int i = 0; i < 10; ++i)
        stream >> channels[i];
    // read counter
    stream >> packetsRecived;
}
