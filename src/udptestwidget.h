#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QTime>
#include <qmath.h>
#include <QVector3D>
#include <QMatrix4x4>
#include <QDebug>
#include <QTimer>

namespace Ui {
class Widget;
}

const float RAD2DEG = (180.0/M_PI);
const float DEG2RAD = (M_PI/180.0);

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

    void on_autoSend_clicked();

    void on_autoAnswer_clicked();

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

    void asMatrix2Quat(const QMatrix4x4 &M, QQuaternion &q);
    void asQuat2RPY(const QQuaternion &q, QVector3D &rpy);
// TODO: check result
//    void asQuat2RPY_v2(const QQuaternion &q, QVector3D &rpy);

/*  // not used
 *  void asRPY2Quat(const QVector3D &rpy, QQuaternion &q);
 *  void asQuat2Matrix(const QQuaternion &q, QMatrix4x4 &m);
 *  void asMatrix2RPY(const QMatrix4x4 &M, QVector3D &rpy);
 */
};

#endif // WIDGET_H
