#include "pointage.h"

Pointage::Pointage()
{
    message = "";
}

void Pointage::init(QDate date,QString ampm, QString school)
{
    //open file
    pfile.load(date,ampm,school);

    //load listing
    lister.init();

    //load
    loadFileContent();
}

//load content from file
void Pointage::loadFileContent()
{
    QStringList line;
    int adh;
    QTime time1;
    QTime time2;

    QStringList contentIN = pfile.getPresencesIN();

    //PART 1
    for (int i = 0; i < contentIN.size(); ++i)
    {
        //numero adherent
        line = contentIN[i].split("(");
        line = line[1].split(")");
        adh = line[0].toInt();

        //get in
        line = contentIN[i].split(" - ");
        line = line[0].trimmed().split(":");
        time1.setHMS(line[0].toInt(),line[1].toInt(),0);

        Presence newPres(adh);
        newPres.setName(lister.findName(adh));
        newPres.setIN(time1);
        presences.append(newPres);
    }

    QStringList contentOUT = pfile.getPresencesOUT();

    //PART2
    for (int i = 0; i < contentOUT.size(); ++i)
    {
        //numero adherent
        line = contentOUT[i].split("(");
        line = line[1].split(")");
        adh = line[0].toInt();

        //get in
        line = contentOUT[i].split("->");
        line = line[0].trimmed().split(":");
        time1.setHMS(line[0].toInt(),line[1].toInt(),0);

        //get out
        line = contentOUT[i].split(" - ");
        line = line[0].split("->");
        line = line[1].trimmed().split(":");
        time2.setHMS(line[0].toInt(),line[1].toInt(),0);

        Presence newPres(adh);
        newPres.setName(lister.findName(adh));
        newPres.setIN(time1);
        newPres.setOUT(time2);
        presences.append(newPres);
    }
}

bool Pointage::Point(QString input)
{
    messageTitle = "POINTAGE Adhérent ";
    messageTitle.append(input);

    //check input
    bool ok;
    int adh = input.toInt(&ok);
    if (!ok) message = "ERREUR: Le N° d'adhérent ne doit contenir que des chiffres !";
    else
    {

        int index = Search(adh);

        //Pointage arrivée
        if (index<0)
        {
            index = Add(adh);
            message = "Nouvelle présence ajoutée !";
        }

        //Pointage départ
        else
        {
            if (presences[index].finnished())
            {
                message = "ERREUR: Cet adhérent est déjà parti !";
                return false;
            }

            QTime now = QTime::currentTime();
            QTime in = presences[index].getIN();

            if ((now.hour()*60+now.minute()-in.hour()*60-in.minute()) >= 3)
            {
                presences[index].setOUT(now);
                message = "Présence terminée !";
            }
            else
            {
                message = "ERREUR: L'arrivée a été pointée il y a moins de 3 minutes ! Vous ne pouvez pas pointer le départ immédiatement.";
                return false;
            }
        }

        return true;
    }

    return false;
}

void Pointage::PointOutAll(QTime time)
{
    for (int j = 0; j < presences.size(); ++j)
    {
        if (!presences[j].finnished()) presences[j].setOUT(time);
    }
}

int Pointage::Search(int adh)
{
    for (int j = 0; j < presences.size(); ++j)
    {
        if (presences[j].getAdh() == adh) return j;
    }
    return -1;
}

int Pointage::Add(int adh)
{
    if (adh<=0) return -1;
    Presence newPres(adh);
    newPres.setName(lister.findName(adh));
    presences.append(newPres);
    return (presences.size() - 1);
}

void Pointage::Refresh()
{
    QTime zero(0,0);
    //check for deleted presence
    for (int j = 0; j < presences.size(); ++j)
    {
        if (!(presences[j].getIN() > zero)) presences.remove(j);
    }

    //push presences to file
    QStringList IN;
    QStringList OUT;
    QString h1;
    QString h2;
    QString adh;
    for (int j = 0; j < presences.size(); ++j)
    {
        h1 = presences[j].getIN().toString("HH:mm");
        h2 = presences[j].getOUT().toString(" -> HH:mm");
        adh = QString("  - (%1)").arg(presences[j].getAdh());

        if (!presences[j].finnished()) IN << h1.append(adh);
        else OUT << h1.append(h2).append(adh);
    }
    pfile.setPresences(IN,OUT);
}

int Pointage::Size()
{
    return presences.size();
}

bool Pointage::finnishedAll()
{
    for (int j = 0; j < presences.size(); ++j)
    {
        if (!presences[j].finnished()) return false;
    }
    return true;
}

QStringList Pointage::listDisplay(bool finnish)
{
    QStringList list;
    for (int j = 0; j < presences.size(); ++j)
    {
        if (presences[j].finnished() == finnish) list << presences[j].display();
    }
    return list;
}
