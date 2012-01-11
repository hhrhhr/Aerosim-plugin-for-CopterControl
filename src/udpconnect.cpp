#include "udpconnect.h"

UdpConnect::UdpConnect(QObject *parent) :
    QObject(parent)
{
    inSocket = NULL;
    outSocket = NULL;

    for (int i = 0; i < 6; ++i) {
        channel[i] = 0.0;
    }
}

UdpConnect::~UdpConnect()
{
    if(outSocket) {
        delete outSocket;
//        outSocket = NULL;
    }
    if(inSocket) {
        delete inSocket;
//        inSocket = NULL;
    }
    if(udplog.isOpen()) {
        udplog.close();
    }
}

void UdpConnect::setDbgLog(QString &fn)
{
    udplog.setFileName(fn + "/udplog.txt");

/*    if (udplog.exists()) {
        QFile::remove(fn + "/udplog.bck");
        udplog.rename(fn + "/udplog.bck");
        udplog.setFileName(fn + "/udplog.txt");
    }*/

    if(!udplog.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&udplog);
    out << "udplog: init\n";
}

void UdpConnect::initSocket(QString remoteHost, quint16 remotePort,
                            QString localHost, quint16 localPort)
{
    QTextStream out(&udplog);
    out << "udplog: initSocket ";


    inSocket = new QUdpSocket();
//    connect(inSocket, SIGNAL(connected()), this, SLOT(onConnected()));
//    connect(inSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
//    connect(inSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(reciveDatagram()));
//    connect(inSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(reciveDatagram()));

    bool ok;
    ok = QObject::connect(inSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()), Qt::DirectConnection);
    if(ok)
        out << "udplog: signal connected to slot\n";
    else
        out << "udplog: signal NOT connected to slot\n";

    if(inSocket->bind(QHostAddress(localHost), localPort)) {
        out << "ok, host:port - " << localHost << ":" << localPort << "\n";
    } else {
        out << "failed: " << inSocket->errorString() << "\n";
    }

    outSocket = new QUdpSocket();
    outHost = remoteHost;
    outPort = remotePort;
}

void UdpConnect::sendDatagram(QByteArray &data)
{
    if(outSocket->writeDatagram(data, outHost, outPort) == -1) {
        QTextStream out(&udplog);
        out << " output socket error: " << outSocket->errorString() << "\n";
    }
}

void UdpConnect::getDataFromUdp(quint32 &pck)
{
//    QTextStream out(&udplog);
//    out << "!!! " << pck;
//    for (int i = 0; i < 6; ++i) {
//        out << QString::number(channel[i], 'f', 2) << ", ";
//        ch += i * sizeof(qreal);
//        **ch = channel[i];
//    }
//    out << "\n";
    pck = recvPacketCounter;
}

void UdpConnect::pushReadyRead()
{
    onReadyRead();
}

/////// private slots

void UdpConnect::onConnected()
{
    QTextStream out(&udplog);
    out << "onConnected\n";
    out.flush();
}
void UdpConnect::onDisconnected()
{
    QTextStream out(&udplog);
    out << "onDisconnected\n";
    out.flush();
}

void UdpConnect::onReadyRead()
{
    QTextStream out(&udplog);
    out << "udplog: <- data\n";
    out.flush();

//    inSocket->waitForReadyRead(3);
    while (inSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(inSocket->pendingDatagramSize());
        quint64 datagramSize;
        datagramSize = inSocket->readDatagram(datagram.data(), datagram.size());
        processDatagram(datagram);
    }
}

////// private

void UdpConnect::processDatagram(QByteArray &datagram)
{
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
}
