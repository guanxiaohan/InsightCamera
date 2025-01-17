#include "MainInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <QMutex>
#include <QtWidgets/QApplication>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QTranslator>

static void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    return;

    /*static QMutex mutex;
    mutex.lock();

    QByteArray localMsg = msg.toLocal8Bit();

    QString strMsg("Info");
    switch (type)
    {
    case QtDebugMsg:
        strMsg = QString("Debug");
        break;
    case QtWarningMsg:
        strMsg = QString("Warning");
        break;
    case QtCriticalMsg:
        strMsg = QString("Critical");
        break;
    case QtFatalMsg:
        strMsg = QString("Fatal");
        break;
    }

    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString strMessage = QString("[%2][%3] %1")
        .arg(localMsg.constData()).arg(strDateTime).arg(strMsg);

    QFile file(".\\log.txt");
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << strMessage << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();*/
}

int main(int argc, char *argv[])
{
    Configuration configuration = Configuration(nullptr);
    QApplication a(argc, argv);

    QTranslator translator;
    QString lang = configuration.getConfig("language").toString();
    bool succeed = true;
    if (lang == "zh_cn")
        succeed = translator.load("InsightCamera_lang__zh_CN.qm");
    a.installTranslator(&translator);

    MainInterface w = MainInterface(&configuration);
    w.show();
    return a.exec();
}
