#include "qdebughandler.h"

void myQDebugHandler(QtMsgType type, const char *msg)
{
    static bool firstRun = true;
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        break;
    }
    QFile outFile("dbglog.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    QTime time;
    if (firstRun) {
        ts << endl << endl;
        firstRun = false;
    }
    ts << time.currentTime().toString("hh:mm:ss.zzz") << " " << txt << endl;

    if (type == QtFatalMsg)
        abort();
}
