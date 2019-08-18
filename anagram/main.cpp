#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setApplicationName("Anagram");
    QApplication::setApplicationDisplayName("Anagram Chain Finder");
    QApplication::setApplicationVersion("1.0");

    MainWindow w;
    w.show();

    return a.exec();
}
