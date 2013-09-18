#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDesktopWidget>
#include "mainwindow.h"

//VERSION NUMBER DEFINED IN FILE.H

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Traduction
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    w.setFixedSize(w.width(),w.height());
    w.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            w.size(),
            qApp->desktop()->availableGeometry()
        ));

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    
    return a.exec();
}


/**
 *TODO
 *Recherche d'adhérent
 */
