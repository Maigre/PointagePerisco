#ifndef PRESENCE_H
#define PRESENCE_H

#include <QDateTime>

class Presence
{
public:
    Presence();
    Presence(int adh);

    QTime getIN();
    void setIN();
    void setIN(QTime time);

    QTime getOUT();
    void setOUT();
    void setOUT(QTime time);

    QString getName();
    void setName(QString name);

    int getAdh();
    bool finnished();
    QString display();

private:
    QTime timeIN;
    QTime timeOUT;
    int adhNum;
    QString adhName;

};

#endif // PRESENCE_H
