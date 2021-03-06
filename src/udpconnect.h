#ifndef UDPCONNECT_H
#define UDPCONNECT_H

#include <QObject>
#include <QUdpSocket>
#include <QList>
#include <QTime>
#include <QMutex>
#include <QMutexLocker>
#include "aerosimdatastruct.h"

class UdpSender : public QObject
{
//    Q_OBJECT
public:
    explicit UdpSender(const QList<quint8> map, bool isTX, QObject *parent = 0);
    ~UdpSender();
    void init(const QString &remoteHost, quint16 remotePort);
    void sendDatagram(const simToPlugin *stp);
    quint32 pcks() { return packetsSended; }

private:
    QUdpSocket *outSocket;
    QHostAddress outHost;
    quint16 outPort;
    QList<float> channels;
    QList<quint8> channelsMap;
    bool takeFromTX;
    quint32 packetsSended;
};


class UdpReciever : public QThread
{
//    Q_OBJECT
public:
    explicit UdpReciever(const QList<quint8> map, bool isRX, QObject *parent = 0);
    ~UdpReciever();
    void init(const QString &localHost, quint16 localPort);
    void run();
    void stop();
    // function getChannels for other threads
    void setChannels(pluginToSim *pts);
    void getFlighStatus(quint8 &arm, quint8 &mod);
    quint8 getArmed() { return armed; }
    quint8 getMode() { return mode; }
    quint32 pcks() { return packetsRecived; }

private:
    volatile bool stopped;
    QMutex mutex;
    QUdpSocket *inSocket;
    QList<float> channels;
    QList<quint8> channelsMap;
    bool sendToRX;
    quint8 armed;
    quint8 mode;
    quint32 packetsRecived;
    void onReadyRead();
    void processDatagram(QByteArray &datagram);
};

#endif // UDPCONNECT_H
