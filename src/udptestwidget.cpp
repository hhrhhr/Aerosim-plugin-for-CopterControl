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

    autoSendTimer = new QTimer(this);
    connect(autoSendTimer, SIGNAL(timeout()), this, SLOT(sendDatagram()), Qt::DirectConnection);
}

Widget::~Widget()
{
    if(outSocket) {
        delete outSocket;
    }
    if(inSocket) {
        delete inSocket;
    }
    delete ui;
}

// private slots //////////////////////////////////////////////////////////////

void Widget::on_btReciveStart_clicked()
{
    on_btReciveStop_clicked();

    inSocket = new QUdpSocket();
    QString host = ui->localHost->text();
    quint16 port = ui->localPort->text().toInt();

    if (inSocket->bind(QHostAddress(host), port)) {
        connect(inSocket, SIGNAL(readyRead()),
                this, SLOT(readDatagram()), Qt::DirectConnection);

        ui->listWidget->addItem("bind ok");
        ui->btReciveStop->setEnabled(1);
        ui->localHost->setDisabled(1);
        ui->localPort->setDisabled(1);
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
    ui->localHost->setEnabled(1);
    ui->localPort->setEnabled(1);
    ui->btReciveStop->setDisabled(1);
}

void Widget::on_btTransmitStart_clicked()
{
    on_btTransmitStop_clicked();

    outSocket = new QUdpSocket();
    outHost = ui->simHost->text();
    outPort = ui->simPort->text().toInt();

    ui->listWidget->addItem("transmit started");
    ui->btTransmitStop->setEnabled(1);
    ui->simHost->setDisabled(1);
    ui->simPort->setDisabled(1);
    ui->btTransmitStart->setDisabled(1);
}

void Widget::on_btTransmitStop_clicked()
{
    if(outSocket) {
        delete outSocket;
        outSocket = NULL;
        ui->listWidget->addItem("transmit stopped");
    } else {
        ui->listWidget->addItem("transmit socket not found");
    }
    ui->btTransmitStart->setEnabled(1);
    ui->simHost->setEnabled(1);
    ui->simPort->setEnabled(1);
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
        if (ui->autoAnswer->isChecked())
            sendDatagram();
    }
}

// private ////////////////////////////////////////////////////////////////////

void Widget::processDatagram(const QByteArray &data)
{
    QByteArray buf = data;
    QDataStream stream(&buf, QIODevice::ReadOnly);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    // check magic header
    quint32 magic;
    stream >> magic;
    if (magic == 0x4153494D) {  // "AERO"

        float   homeX, homeY, homeZ,
                WpHX, WpHY, WpLat, WpLon,
                posX, posY, posZ,
                velX, velY, velZ,
                angX, angY, angZ,
                accX, accY, accZ,
                lat, lon, alt,
                head, pitch, roll,
                volt, curr,
                chAil, chEle, chThr, chRud, chPlg;

        stream >> homeX >> homeY >> homeZ;
        stream >> WpHX >> WpHY >> WpLat >> WpLon;
        stream >> posX >> posY >> posZ;
        stream >> velX >> velY >> velZ;
        stream >> angX >> angY >> angZ;
        stream >> accX >> accY >> accZ;
        stream >> lat >> lon >> alt;
        stream >> head >> pitch >> roll;
        stream >> volt >> curr;
        stream >> chAil >> chEle >> chThr >> chRud >> chPlg;
        stream >> packetCounter;

        if(ui->tabWidget->currentIndex() != 0)
            return;

        if (screenTimeout.elapsed() < 200)
            return;

        ui->listWidget->clear();
        ui->listWidget->addItem("home location (m)");
        ui->listWidget->addItem(QString("%1, %2, %3")
                                .arg(homeX, 7, 'f', 4)
                                .arg(homeY, 7, 'f', 4)
                                .arg(homeZ, 7, 'f', 4));
        ui->listWidget->addItem("home waypoint");
        ui->listWidget->addItem(QString("%1, %2, %3, %4")
                                .arg(WpHX, 7, 'f', 4)
                                .arg(WpHY, 7, 'f', 4)
                                .arg(WpLat, 7, 'f', 4)
                                .arg(WpLon, 7, 'f', 4));
        ui->listWidget->addItem("model position (m)");
        ui->listWidget->addItem(QString("%1, %2, %3")
                                .arg(posX, 7, 'f', 4)
                                .arg(posY, 7, 'f', 4)
                                .arg(posZ, 7, 'f', 4));
        ui->listWidget->addItem("model velocity (m/s)");
        ui->listWidget->addItem(QString("%1, %2, %3")
                                .arg(velX, 7, 'f', 4)
                                .arg(velY, 7, 'f', 4)
                                .arg(velZ, 7, 'f', 4));
        ui->listWidget->addItem("model angular velocity (rad/s)");
        ui->listWidget->addItem(QString("%1, %2, %3")
                                .arg(angX, 7, 'f', 4)
                                .arg(angY, 7, 'f', 4)
                                .arg(angZ, 7, 'f', 4));
        ui->listWidget->addItem("model acceleration (m/s/s)");
        ui->listWidget->addItem(QString("%1, %2, %3")
                                .arg(accX, 7, 'f', 4)
                                .arg(accY, 7, 'f', 4)
                                .arg(accZ, 7, 'f', 4));
        ui->listWidget->addItem("model coordinates (deg, deg, m)");
        ui->listWidget->addItem(QString("%1, %2, %3")
                                .arg(lat, 7, 'f', 4)
                                .arg(lon, 7, 'f', 4)
                                .arg(alt, 7, 'f', 4));
        ui->listWidget->addItem("model attitude (rad)");
        ui->listWidget->addItem(QString("%1, %2, %3")
                                .arg(head, 7, 'f', 4)
                                .arg(pitch, 7, 'f', 4)
                                .arg(roll, 7, 'f', 4));
        ui->listWidget->addItem("model electrics");
        ui->listWidget->addItem(QString("%1V, %2A")
                                .arg(volt, 7, 'f', 4)
                                .arg(curr, 7, 'f', 4));
        ui->listWidget->addItem("channels");
        ui->listWidget->addItem(QString("%1 %2 %3 %4 %5 %6")
                                .arg(chAil, 6, 'f', 3)
                                .arg(chEle, 6, 'f', 3)
                                .arg(chThr, 6, 'f', 3)
                                .arg(chRud, 6, 'f', 3)
                                .arg(chPlg, 6, 'f', 3));
        ui->listWidget->addItem("datagram size (bytes), packet counter");
        ui->listWidget->addItem(QString("%1 %2")
                                .arg(data.size())
                                .arg(packetCounter));

        screenTimeout.restart();

    } else if (magic == 0x52434D44) { // "RCMD"

        qreal ch1, ch2, ch3, ch4, ch5, ch6;
        stream >> ch1 >> ch2 >> ch3 >> ch4 >> ch5 >> ch6;

        if(ui->tabWidget->currentIndex() == 0) {
            if (screenTimeout.elapsed() < 200)
                return;
            ui->listWidget->clear();
            ui->listWidget->addItem("channels");
            ui->listWidget->addItem("CH1: " + QString::number(ch1));
            ui->listWidget->addItem("CH2: " + QString::number(ch2));
            ui->listWidget->addItem("CH3: " + QString::number(ch3));
            ui->listWidget->addItem("CH4: " + QString::number(ch4));
            ui->listWidget->addItem("CH5: " + QString::number(ch5));
            ui->listWidget->addItem("CH6: " + QString::number(ch6));
        }
        screenTimeout.restart();
    } else {
        qDebug() << "unknown magic:" << magic;
    }
}

void Widget::sendDatagram()
{
    if(!outSocket)
        return;

    float ch1, ch2, ch3, ch4, ch5, ch6;
    float coeff = 1.0 / 512.0;

    ch1 = ui->ch1->value() * coeff;
    ch2 = ui->ch2->value() * coeff;
    ch3 = ui->ch3->value() * coeff;
    ch4 = ui->ch4->value() * coeff;
    ch5 = ui->ch5->value() * coeff;
    ch6 = ui->ch6->value() * coeff;

    QByteArray data;
    // 32 - current size of values, 4(quint32) + 6*4(float) + 4(quint32)
    data.resize(32);
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    // magic header, "RCMD"
    stream << quint32(0x52434D44);
    // send channels
    stream << ch1 << ch2 << ch3 << ch4 << ch5 << ch6;
    // send readed counter
    stream << packetCounter;

    if (outSocket->writeDatagram(data, outHost, outPort) == -1) {
        qDebug() << "write failed: outHost" << outHost << " "
                 << "outPort " << outPort << " "
                 << outSocket->errorString();
    }
}

void Widget::on_autoSend_clicked()
{
    autoSendTimer->start(100);
    qDebug() << "timer start";
}

void Widget::on_autoAnswer_clicked()
{
    autoSendTimer->stop();
    qDebug() << "timer stop";
}
