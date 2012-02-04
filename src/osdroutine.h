#ifndef OSDROUTINE_H
#define OSDROUTINE_H

#include <QObject>
#include <QPainter>
#include <QImage>
#include <QStaticText>
#include "aerosimdatastruct.h"

class OSDRoutine : public QObject
{
public:
    explicit OSDRoutine(quint16 width, quint16 height, QObject *parent = 0);
    ~OSDRoutine();

    void refresh(const simToPlugin *stp, pluginToSim *pts);

private:
    quint16 osdWidth;
    quint16 osdHeight;
};

#endif // OSDROUTINE_H
