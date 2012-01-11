#ifndef UDPCONNECT_H
#define UDPCONNECT_H

#include <QObject>
#include <QUdpSocket>
#include <QFile>
#include <QTextStream>

class UdpConnect : public QObject
{
    Q_OBJECT
public:
    explicit UdpConnect(QObject *parent = 0);
    ~UdpConnect();

    void setDbgLog(QString &fn);
    void initSocket(QString remoteHost, quint16 remotePort,
                    QString localHost, quint16 localPort);
    void sendDatagram(QByteArray &data);
    void getDataFromUdp(quint32 &pck);
    void pushReadyRead();

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();

private:
    QFile udplog;
//    QTextStream dbglog;

    QUdpSocket *inSocket;
    QUdpSocket *outSocket;
    QHostAddress outHost;
    quint16 outPort;
//    QByteArray datagram;
    qreal channel[6];
    quint32 recvPacketCounter;

    void processDatagram(QByteArray &datagram);
};

#endif // UDPCONNECT_H
