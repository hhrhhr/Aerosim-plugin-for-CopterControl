#include "osd.h"

OSD::OSD(quint16 width, quint16 height, QObject *parent)
    : QObject(parent)
{
    osdWidth = width;
    osdHeight = height;
}

OSD::~OSD()
{

}

void OSD::refresh(const simToPlugin *stp, pluginToSim *pts)
{
    QImage img(stp->OSDVideoBuf, osdWidth, osdHeight, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);

    static QImage logo(":/logo.png");

    QPainter p;
    p.begin(&img);
    /*
     * begin paint
    */
    // rects
    for (int i = 0; i < 16; ++i) {
        p.setBrush(QColor(i*16, 0, 255-i*16, 255-i*16));
        p.drawRect(i*16, i*16, osdWidth/8, osdHeight/8);
    }

    // image
    p.drawImage(QPoint(0, osdHeight - logo.height()), logo);

    // text
    p.setFont(QFont("Arial", 24));

    p.setPen(Qt::green);
    p.drawStaticText(0, 200, QStaticText("STATIC TEXT"));

    p.setPen(Qt::yellow);
    static int cnt = 0;
    p.drawText(0, 250, "DINAMIC " + QString::number(cnt) + " TEXT");
    ++cnt;

    /*
    * paint end
    */
    p.end();

    // swap R and B components
    for (int i = 0; i < osdWidth*osdHeight; ++i) {
        qSwap(stp->OSDVideoBuf[i * 4 + 0], stp->OSDVideoBuf[i * 4 + 2]);
    }

    pts->OSDWindow_DX = osdWidth;
    pts->OSDWindow_DY = osdHeight;
}
