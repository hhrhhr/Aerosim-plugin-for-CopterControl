#include "settings.h"

Settings::Settings(QString settingsPath, QObject *parent) :
    QObject(parent)
{
   settings = new QSettings(settingsPath + "/cc_plugin.ini", QSettings::IniFormat);
   sendToHost = "127.0.0.1";
   sendToPort = 40100;
   listenOnHost = "127.0.0.1";
   listenOnPort = 40200;
   channels.reserve(60);
   for (quint8 i = 0; i < 10; ++i)
       channelsMap << i;
   outputToTX = true;
}

void Settings::read()
{
    sendToHost = settings->value("send_to_host", sendToHost).toString();
    sendToPort = settings->value("send_to_port", sendToPort).toInt();
    listenOnHost = settings->value("listen_on_host", listenOnHost).toString();
    listenOnPort = settings->value("listen_on_port", listenOnPort).toInt();

    QString allChannels = settings->value("channels/all_channels").toString();
    QString chan = "";
    int i = 0;
    foreach (chan, allChannels.split(" "))
        channels.insert(chan, i++);

    QString num = "";
    QString map = "";
    for (quint8 i = 0; i < 10; ++i) {
        num = QString::number(i+1);
        map = settings->value("channels/cc_channel_" + num).toString();
        channelsMap[i] = channels.value(map, channelsMap.at(i));
    }

    QString outputTo = settings->value("channels/output_to", "TX").toString();
    outputToTX = (outputTo == "TX") ? true : false;
}
