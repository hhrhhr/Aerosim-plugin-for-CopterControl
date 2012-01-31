#include "udpconnect.h"
#include "enums.h"

UdpSender::UdpSender(const QList<quint8> map,
                     bool isTX,
                     QObject *parent)
    : QObject(parent)
{
    qDebug() << this << "UdpSender::UdpSender thread:" << thread();
    outSocket = NULL;
    for (int i = 0; i < 8; ++i)
        channels << 0.0;
    channelsMap = map;
    takeFromTX = isTX;
    packetsSended = 0;
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
    data.resize(184);
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    // magic header, "AERO"
    out << quint32(0x4153494D);
    // home location
    out << stp->initPosX    << stp->initPosY    << stp->initPosZ;
    out << stp->wpHomeX     << stp->wpHomeY     << stp->wpHomeLat   << stp->wpHomeLong;
    // position
    out << stp->posX        << stp->posY        << stp->posZ;
    // velocity (world)
    out << stp->velX        << stp->velY        << stp->velZ;
    // angular velocity (model)
    out << stp->angVelXm    << stp->angVelYm    << stp->angVelZm;
    // acceleration (model)
    out << stp->accelXm     << stp->accelYm     << stp->accelZm;
    // coordinates
    out << stp->latitude    << stp->longitude;
    // sonar
    out << stp->AGL;
    // attitude
    out << stp->heading     << stp->pitch       << stp->roll;
    // electric
    out << stp->voltage     << stp->current;
    // matrix
    out << stp->axisXx << stp->axisXy << stp->axisXz;
    out << stp->axisYx << stp->axisYy << stp->axisYz;
    out << stp->axisZx << stp->axisZy << stp->axisZz;
    // channels
    for (int i = 0; i < 8; ++i) {
        quint8 mapTo = channelsMap.at(i);
        if (mapTo == 255)       // unused channel
            out << 0.0;
        else if (takeFromTX)    // use values from simulators transmitter
            out << stp->chSimTX[mapTo];
        else                    // direct use values from ESC/motors/ailerons/etc
            out << stp->chSimRX[mapTo];
    }

    // packet counter
    out << packetsSended;

    outSocket->writeDatagram(data, outHost, outPort);
    ++packetsSended;
}

/***********************************************************************************************
 ***********************************************************************************************
 */

UdpReciever::UdpReciever(const QList<quint8> map,
                         bool isRX,
                         QObject *parent)
    : QThread(parent)
{
    qDebug() << this << "UdpReciever::UdpReciever thread:" << thread();

    stopped = false;
    inSocket = NULL;
    for (int i = 0; i < 10; ++i)
        channels << -1.0;
    channelsMap = map;
    sendToRX = isRX;
    armed = 0;
    mode = 0;
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

void UdpReciever::setChannels(pluginToSim *pts)
{
    for (int i = 0; i < 10; ++i) {
        quint8 mapTo = channelsMap.at(i);
        if (mapTo != 255) {
            float channelValue = qBound(-1.0f, channels.at(i), 1.0f);
            if (sendToRX) {
                // direct connect to ESC/motors/ailerons/etc
                pts->chNewRX[mapTo] = channelValue;
                pts->chOverRX[mapTo] = true;
            } else {
                // replace simulators transmitter
                pts->chNewTX[mapTo] = channelValue;
                pts->chOverTX[mapTo] = true;
            }
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
    // read flight mode
    stream >> armed >> mode;
    // read counter
    stream >> packetsRecived;
}
