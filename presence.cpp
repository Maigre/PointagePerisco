#include "presence.h"

Presence::Presence()
{

}

Presence::Presence(int adh)
{
    adhNum = adh;
    setIN();
}

int Presence::getAdh()
{
    return adhNum;
}

bool Presence::finnished()
{
    QTime zero(0,0);
    return ((!timeOUT.isNull()) && (timeOUT > zero));
}

QString Presence::display()
{
    QString msg;
    msg.append(timeIN.toString("HH:mm"));
    if (finnished())
    {
        msg.append(" -> ");
        msg.append(timeOUT.toString("HH:mm"));
    }
    msg.append("   ");
    msg.append(adhName);
    msg.append(QString(" [%1] ").arg(adhNum));

    return msg;
}

QTime Presence::getIN()
{
    return timeIN;
}

void Presence::setIN()
{
    timeIN = QTime::currentTime ();
}

void Presence::setIN(QTime time)
{
    timeIN = time;
}

QTime Presence::getOUT()
{
    return timeOUT;
}

void Presence::setOUT()
{
    timeOUT = QTime::currentTime();
}

void Presence::setOUT(QTime time)
{
    QTime zero(0,0);
    if (time >= timeIN) timeOUT = time;
    else timeOUT = zero;
}

QString Presence::getName()
{
    return adhName;
}

void Presence::setName(QString name)
{
    adhName=name;
}


