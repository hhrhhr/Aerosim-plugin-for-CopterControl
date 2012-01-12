#ifndef UDPCONNECT_H
#define UDPCONNECT_H

#include <QObject>
#include <QUdpSocket>
#include <QList>
#include "aerosimdatastruct.h"

class UdpConnect : public QObject
{
    Q_OBJECT
public:
    explicit UdpConnect(QObject *parent = 0);
    ~UdpConnect();

    void initSocket(QString &remoteHost, quint16 remotePort,
                    QString &localHost, quint16 localPort);
//    void sendDatagram(const QByteArray &data)
    void sendDatagram(const simToPlugin *stp);
    void onReadyRead(pluginToSim *pts);
    void getStats (quint32 &s, quint32 &r, quint32 &l);

private slots:
    void onStateChanged(QAbstractSocket::SocketState state);
    void onError(QAbstractSocket::SocketError error);

private:
    QUdpSocket *inSocket;
    QUdpSocket *outSocket;
    QHostAddress outHost;
    quint16 outPort;

    void processDatagram(QByteArray &datagram);
    QList<float> channel;
    QList<int> channelMap;
    quint32 packetsSended;
    quint32 packetsRecived;
    quint32 packetsLost;
};

#endif // UDPCONNECT_H
