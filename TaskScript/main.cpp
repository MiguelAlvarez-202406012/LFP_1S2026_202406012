#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/TaskScript/img/icon.png"));
    QIcon appIcon(":/img/icon.png");
    a.setWindowIcon(appIcon);
    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
