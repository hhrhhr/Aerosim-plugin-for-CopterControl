#ifndef UDPCONNECT_H
#define UDPCONNECT_H

#include <QObject>
#include <QUdpSocket>
#include "datafromaerosim.h"

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

private slots:
    void onStateChanged(QAbstractSocket::SocketState state);
    void onError(QAbstractSocket::SocketError error);
    void onReadyRead();

private:
    QUdpSocket *inSocket;
    QUdpSocket *outSocket;
    QHostAddress outHost;
    quint16 outPort;

    void processDatagram(QByteArray &datagram);
};

#endif // UDPCONNECT_H
