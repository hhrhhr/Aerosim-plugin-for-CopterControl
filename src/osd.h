#ifndef OSD_H
#define OSD_H

#include <QObject>
#include <QPainter>
#include <QImage>
#include <QStaticText>
#include <QGraphicsScene>
#include <QtSvg/QSvgRenderer>
#include <QtSvg/QGraphicsSvgItem>

#include "aerosimdatastruct.h"

class OSD : public QObject
{
public:
    explicit OSD(quint16 width, quint16 height, QObject *parent = 0);
    ~OSD();

    void refresh(const simToPlugin *stp, pluginToSim *pts);

private:
    quint16 osdWidth;
    quint16 osdHeight;

    QGraphicsScene *m_scene;
    QSvgRenderer *m_renderer;
    QGraphicsSvgItem *m_background;
};

#endif // OSD_H
