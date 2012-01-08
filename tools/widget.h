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
    void on_btConnect_clicked();
    void on_btDisconnect_clicked();

    void readDatagram();
    void processDatagram(const QByteArray &data);

private:
    Ui::Widget *ui;

    QUdpSocket *inSocket;
    QTime screenTimeout;

};

#endif // WIDGET_H
