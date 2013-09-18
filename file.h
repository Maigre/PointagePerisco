#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QFile>
#include <QDate>
#include <QTextStream>
#include <QStringList>

#define VERSION "2.1"

class file
{
public:
    file();
    void load(QDate date, QString ampm, QString school);
    bool checkHeader(QStringList file);

    void setPresences(QStringList IN,QStringList OUT);
    QStringList getPresencesIN();
    QStringList getPresencesOUT();

private:

    void loadContentInOut(QStringList file);
    void write();

    QFile hfile;
    QStringList header;
    QStringList contentIN;
    QStringList contentOUT;

};

#endif // FILE_H
