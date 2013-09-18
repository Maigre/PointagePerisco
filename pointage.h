#ifndef POINTAGE_H
#define POINTAGE_H

#include <QString>
#include <QStringList>
#include <QVector>
#include "presence.h"
#include "file.h"
#include "listing.h"

class Pointage
{
public:
    Pointage();
    void init(QDate date,QString ampm, QString school);

    bool Point(QString input);
    void PointOutAll(QTime time);

    int Search(int adh);
    int Add(int adh);
    void Refresh();

    bool finnishedAll();

    int Size();

    QStringList listDisplay(bool finnish);

    QString messageTitle;
    QString message;

    QVector<Presence> presences;

    Listing lister;

private:

    file pfile;
    void loadFileContent();

};

#endif // POINTAGE_H
