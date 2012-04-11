#include "configgui.h"
#include "ui_configgui.h"

ConfigGUI::ConfigGUI(QString settingsFolder, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigGUI)
{
    qDebug() << this << "ConfigGUI";
    ui->setupUi(this);

    setGeometry(5, 25, 420, 300);

    Settings ini(settingsFolder);
    ini.read();
    ui->localHost->setText(ini.localHost());
    ui->localPort->setText(QString::number(ini.localPort()));
    ui->remoteHost->setText(ini.remoteHost());
    ui->remotePort->setText(QString::number(ini.remotePort()));
}

ConfigGUI::~ConfigGUI()
{
    qDebug() << this << "~ConfigGUI";
    delete ui;
}
