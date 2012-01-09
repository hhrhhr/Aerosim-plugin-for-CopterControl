#include "udptestwidget.h"
#include "ui_udptestwidget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    inSocket = NULL;
    outSocket = NULL;
    screenTimeout.start();
    packetCounter = 0;
}

Widget::~Widget()
{
    if(outSocket) {
        delete outSocket;
        outSocket = NULL;
    }
    if(inSocket) {
        delete inSocket;
        inSocket = NULL;
    }
    delete ui;
}

// private slots //////////////////////////////////////////////////////////////

void Widget::on_btReciveStart_clicked()
{
    on_btReciveStop_clicked();

    inSocket = new QUdpSocket();
    connect(inSocket, SIGNAL(readyRead()), this, SLOT(readDatagram()), Qt::DirectConnection);

    QString host = ui->simHost->text();
    quint16 port = ui->simPort->text().toInt();

    if (inSocket->bind(QHostAddress(host), port)) {
        ui->listWidget->addItem("bind ok");
        ui->btReciveStop->setEnabled(1);
        ui->simHost->setDisabled(1);
        ui->simPort->setDisabled(1);
        ui->btReciveStart->setDisabled(1);
    } else {
        ui->listWidget->addItem("bind error: " + inSocket->errorString());
    }
}

void Widget::on_btReciveStop_clicked()
{
    if(inSocket) {
        delete inSocket;
        inSocket = NULL;
        ui->listWidget->addItem("unbind ok");
    } else {
        ui->listWidget->addItem("socket not found");
    }
    ui->btReciveStart->setEnabled(1);
    ui->simHost->setEnabled(1);
    ui->simPort->setEnabled(1);
    ui->btReciveStop->setDisabled(1);
}

void Widget::on_btTransmitStart_clicked()
{
    on_btTransmitStop_clicked();

    outSocket = new QUdpSocket();
    outHost = ui->localHost->text();
    outPort = ui->localPort->text().toInt();

    ui->listWidget->addItem("transmit started");
    ui->btTransmitStop->setEnabled(1);
    ui->localHost->setDisabled(1);
    ui->localPort->setDisabled(1);
    ui->btTransmitStart->setDisabled(1);

    // one-shot connection
//    sendDatagram();
}

void Widget::on_btTransmitStop_clicked()
{
    if(outSocket) {
        delete outSocket;
        outSocket = NULL;
        ui->listWidget->addItem("transmit stopped");
    }
    ui->btTransmitStart->setEnabled(1);
    ui->localHost->setEnabled(1);
    ui->localPort->setEnabled(1);
    ui->btTransmitStop->setDisabled(1);
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
        if(outSocket)
            sendDatagram();
    }
}

// private ////////////////////////////////////////////////////////////////////

void Widget::processDatagram(const QByteArray &data)
{
    QByteArray buf = data;
    QDataStream stream(&buf, QIODevice::ReadOnly);

    // check magic header
    quint32 magic;
    stream >> magic;
    if (magic == 0x4153494D) {  // "AERO"
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
        stream >> packetCounter;

        if(ui->tabWidget->currentIndex() != 0)
            return;

        if (screenTimeout.elapsed() < 200)
            return;

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
        ui->listWidget->addItem("datagram size (bytes), packet counter");
        ui->listWidget->addItem(QString::number(data.size()) + "\t" + QString::number(packetCounter));

        screenTimeout.restart();

    } else if(magic == 0x52434D44) { // "RCMD"
        qreal ch1, ch2, ch3, ch4, ch5, ch6;
        stream >> ch1 >> ch2 >> ch3 >> ch4 >> ch5 >> ch6;

        if(ui->sendCH->isChecked())
            return;

        if (screenTimeout.elapsed() < 200)
            return;

        ui->listWidget->clear();

        if(ui->tabWidget->currentIndex() == 0) {
            ui->listWidget->addItem("channels");
            ui->listWidget->addItem("CH1: " + QString::number(ch1));
            ui->listWidget->addItem("CH2: " + QString::number(ch2));
            ui->listWidget->addItem("CH3: " + QString::number(ch3));
            ui->listWidget->addItem("CH4: " + QString::number(ch4));
            ui->listWidget->addItem("CH5: " + QString::number(ch5));
            ui->listWidget->addItem("CH6: " + QString::number(ch6));
        } else if(ui->tabWidget->currentIndex() == 1) {
            ui->ch1->setValue(int(ch1 * 512));
            ui->ch2->setValue(int(ch2 * 512));
            ui->ch3->setValue(int(ch3 * 512));
            ui->ch4->setValue(int(ch4 * 512));
            ui->ch5->setValue(int(ch5 * 512));
            ui->ch6->setValue(int(ch6 * 512));
        }

        screenTimeout.restart();
    } else {
        qDebug() << "unknown magic:" << magic;
    }
}

void Widget::sendDatagram()
{
    if(ui->readCH->isChecked())
        return;

    qreal ch1, ch2, ch3, ch4, ch5, ch6;
    qreal coef = 1.0 / 512.0;

    ch1 = ui->ch1->value() * coef;
    ch2 = ui->ch2->value() * coef;
    ch3 = ui->ch3->value() * coef;
    ch4 = ui->ch4->value() * coef;
    ch5 = ui->ch5->value() * coef;
    ch6 = ui->ch6->value() * coef;

    QByteArray data;
    // 56 - current size of values
    data.resize(56);
    QDataStream stream(&data, QIODevice::WriteOnly);
    // magic header, "RCMD"
    stream << quint32(0x52434D44);
    // send channels
    stream << ch1 << ch2 << ch3 << ch4 << ch5 << ch6;
    // send readed counter
    stream << packetCounter;

    if(outSocket->writeDatagram(data, outHost, outPort) == -1) {
        qDebug() << "outHost" << outHost << " "
                 << "outPort " << outPort << " "
                 << outSocket->errorString();
    }
}
