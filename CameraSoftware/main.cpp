#include "MainInterface.h"
#include <QtWidgets/QApplication>
#include <QtCore/QFile>
#include <QtCore/QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto file = QFile(":/MainInterface/StyleSheet");
    auto sheet = file.readAll().toStdString();
    a.setStyleSheet(QString::fromStdString(sheet));
    MainInterface w;
    w.show();
    return a.exec();
}
