#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QTime>
#include <QDebug>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private slots:
    void on_btReciveStart_clicked();
    void on_btReciveStop_clicked();
    void on_btTransmitStart_clicked();
    void on_btTransmitStop_clicked();

    void readDatagram();
    void sendDatagram();

    void on_autoSend_clicked(bool checked);

private:
    Ui::Widget *ui;

    QTime screenTimeout;
    QUdpSocket *inSocket;
    QUdpSocket *outSocket;
    QHostAddress outHost;
    quint16 outPort;
    quint32 packetCounter;

    void processDatagram(const QByteArray &data);

    QTimer *autoSendTimer;
};

#endif // WIDGET_H
