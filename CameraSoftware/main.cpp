#include "MainInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <QMutex>
#include <QtWidgets/QApplication>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QDateTime>

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    static QMutex mutex;
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

    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(myMessageOutput);
    auto file = QFile(":/MainInterface/StyleSheet");
    auto sheet = file.readAll().toStdString();
    a.setStyleSheet(QString::fromStdString(sheet));
    MainInterface w;
    w.show();
    return a.exec();
}
