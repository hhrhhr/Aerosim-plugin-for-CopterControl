#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QTime>
#include <QDebug>

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

private:
    Ui::Widget *ui;

    QTime screenTimeout;
    QUdpSocket *inSocket;
    QUdpSocket *outSocket;
    QHostAddress outHost;
    quint16 outPort;

    void processDatagram(const QByteArray &data);
    void sendDatagram();
};

#endif // WIDGET_H
