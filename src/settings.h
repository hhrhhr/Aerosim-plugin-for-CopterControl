#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QHash>
#include <QList>
#include <QStringList>
#include <QDebug>

class Settings : public QObject
{
public:
    explicit Settings(QString settingsPath, QObject *parent = 0);
    void read();
    QString remoteHost() { return sendToHost; }
    quint16 remotePort() { return sendToPort; }
    QString localHost() { return listenOnHost; }
    quint16 localPort() { return listenOnPort; }
    QList<quint8> getInputMap() { return inputMap; }
    QList<quint8> getOutputMap() { return outputMap; }
    bool isToRX() { return sendToRX; }
    bool isFromTX() { return takeFromTX; }
    QList<quint16> getVideoModes() { return videoModes; }

private:
    QHash<QString, quint8> channels;
    QSettings *settings;
    QString sendToHost;
    quint16 sendToPort;
    QString listenOnHost;
    quint16 listenOnPort;
    QList<quint8> inputMap;
    QList<quint8> outputMap;
    bool sendToRX;
    bool takeFromTX;
    QList<quint16> videoModes;
};

#endif // SETTINGS_H
