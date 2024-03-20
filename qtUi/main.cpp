#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include<QLabel>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "qtUi_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.setWindowTitle("SeamlessSync");
    w.resize(800,600);
    w.show();
    return a.exec();
}
