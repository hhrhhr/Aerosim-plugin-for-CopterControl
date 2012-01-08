#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    inSocket = NULL;
    screenTimeout.start();
}

Widget::~Widget()
{
    if(inSocket) {
        delete inSocket;
        inSocket = NULL;
    }
    delete ui;
}

void Widget::on_btConnect_clicked()
{
    on_btDisconnect_clicked();

    inSocket = new QUdpSocket();
    connect(inSocket, SIGNAL(readyRead()), this, SLOT(readDatagram()), Qt::DirectConnection);
    QString host = ui->host->text();
    quint16 port = ui->port->text().toInt();

    if (inSocket->bind(QHostAddress(host), port)) {
        ui->listWidget->addItem("bind ok");
        ui->btDisconnect->setEnabled(1);
        ui->host->setDisabled(1);
        ui->port->setDisabled(1);
        ui->btConnect->setDisabled(1);
    } else {
        ui->listWidget->addItem("bind error: " + inSocket->errorString());
    }
}

void Widget::on_btDisconnect_clicked()
{
    if(inSocket) {
        delete inSocket;
        inSocket = NULL;
        ui->listWidget->addItem("unbind ok");
        ui->btConnect->setEnabled(1);
        ui->host->setEnabled(1);
        ui->port->setEnabled(1);
        ui->btDisconnect->setDisabled(1);
    }
}

void Widget::readDatagram()
{
    while (inSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(inSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        quint64 datagramSize = inSocket->readDatagram(datagram.data(), datagram.size(),
                                                      &sender, &senderPort);
        Q_UNUSED(datagramSize);
        processDatagram(datagram);
    }
}

void Widget::processDatagram(const QByteArray &data)
{
    QByteArray buf = data;
    QDataStream stream(&buf, QIODevice::ReadOnly);

    // check magic header
    quint32 magic;
    stream >> magic;
    if (magic != 0x4153494D) {
        qDebug() << magic;
        return;
    }

    qreal homeX, homeY, homeZ;
    qreal WpHX, WpHY, WpLat, WpLon;
    qreal posX, posY, posZ;
    qreal velX, velY, velZ;
    qreal angX, angY, angZ;
    qreal accX, accY, accZ;
    qreal lat, lon;
    qreal alt;
    qreal head, pitch, roll;
    qreal volt, curr;

    stream >> homeX >> homeY >> homeZ;
    stream >> WpHX >> WpHY >> WpLat >> WpLon;
    stream >> posX >> posY >> posZ;
    stream >> velX >> velY >> velZ;
    stream >> angX >> angY >> angZ;
    stream >> accX >> accY >> accZ;
    stream >> lat >> lon;
    stream >> alt;
    stream >> head >> pitch >> roll;
    stream >> volt >> curr;

    if (screenTimeout.elapsed() >= 200) {
        ui->listWidget->clear();
        ui->listWidget->addItem("home location (m)");
        ui->listWidget->addItem(QString::number(homeX) + "\t" +
                                QString::number(homeY) + "\t" +
                                QString::number(homeZ));
        ui->listWidget->addItem("home waypoint");
        ui->listWidget->addItem(QString::number(WpHX) + "m\t" +
                                QString::number(WpHY) + "m");
        ui->listWidget->addItem(QString::number(WpLat) + "°\t" +
                                QString::number(WpLon) + "°");
        ui->listWidget->addItem("model position (m)");
        ui->listWidget->addItem(QString::number(posX) + "\t" +
                                QString::number(posY) + "\t" +
                                QString::number(posZ));
        ui->listWidget->addItem("model velocity (m/s)");
        ui->listWidget->addItem(QString::number(velX) + "\t" +
                                QString::number(velY) + "\t" +
                                QString::number(velZ));
        ui->listWidget->addItem("model angular velocity (rad/s)");
        ui->listWidget->addItem(QString::number(angX) + "\t" +
                                QString::number(angY) + "\t" +
                                QString::number(angZ));
        ui->listWidget->addItem("model acceleration (m/s/s)");
        ui->listWidget->addItem(QString::number(accX) + "\t" +
                                QString::number(accY) + "\t" +
                                QString::number(accZ));
        ui->listWidget->addItem("model coordinates (deg)");
        ui->listWidget->addItem(QString::number(lat) + "°\t" +
                                QString::number(lon) + "°\t" +
                                QString::number(alt) + "m");
        ui->listWidget->addItem("model attitude (rad)");
        ui->listWidget->addItem(QString::number(head) + "\t" +
                                QString::number(pitch) + "\t" +
                                QString::number(roll));
        ui->listWidget->addItem("model electrics");
        ui->listWidget->addItem(QString::number(volt) + "V\t" +
                                QString::number(curr) + "A");
        ui->listWidget->addItem("datagram size (bytes)");
        ui->listWidget->addItem(QString::number(data.size()));

        screenTimeout.restart();
    }
}
