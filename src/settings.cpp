#include "settings.h"

Settings::Settings(QString settingsPath, QObject *parent) :
    QObject(parent)
{
   settings = new QSettings(settingsPath + "/cc_plugin.ini", QSettings::IniFormat);
   // default settings
   sendToHost = "127.0.0.1";
   sendToPort = 40100;
   listenOnHost = "127.0.0.1";
   listenOnPort = 40200;
   channels.reserve(60);
   for (quint8 i = 0; i < 10; ++i)
       inputMap << 255;
   for (quint8 i = 0; i < 8; ++i)
       outputMap << 255;
   sendToRX = true;
   takeFromTX = true;
   videoModes << 1 << 50 << 50 << 800 << 600;
}

void Settings::read()
{
    // network
    listenOnHost = settings->value("listen_on_host", listenOnHost).toString();
    listenOnPort = settings->value("listen_on_port", listenOnPort).toInt();
    sendToHost = settings->value("send_to_host", sendToHost).toString();
    sendToPort = settings->value("send_to_port", sendToPort).toInt();

    QString allChannels = settings->value("all_channels").toString();
    QString chan;
    int i = 0;
    foreach (chan, allChannels.split(" "))
        channels.insert(chan, i++);

    // inputs
    QString num = "";
    QString map = "";
    for (quint8 i = 0; i < 10; ++i) {
        num = QString::number(i+1);
        map = settings->value("Input/cc_channel_" + num).toString();
        inputMap[i] = channels.value(map, inputMap.at(i));
    }

    QString sendTo = settings->value("Input/send_to", "RX").toString();
    sendToRX = (sendTo == "RX") ? true : false;

    // outputs
    for (quint8 i = 0; i < 8; ++i) {
        num = QString::number(i+1);
        map = settings->value("Output/sim_channel_" + num).toString();
        outputMap[i] = channels.value(map, outputMap.at(i));
    }

    QString takeFrom = settings->value("Output/take_from", "TX").toString();
    takeFromTX = (takeFrom == "TX") ? true : false;

    // video
    quint8 resolutionNum = settings->value("Video/number_of_resolutions", 0).toInt();
    if (resolutionNum > 0) {
        videoModes.clear();
        videoModes << resolutionNum;
        for (quint8 i = 0; i < resolutionNum; ++i) {
            num = QString::number(i+1);
            QString modes = settings->value("Video/resolution_" + num, "0, 0, 640, 480").toString();
            QString mode;
            foreach (mode, modes.split(" "))
                videoModes << mode.toInt();
        }
    }
}
