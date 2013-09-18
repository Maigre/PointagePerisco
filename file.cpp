#include "file.h"

file::file()
{
}

//test de charger un fichier, sinon création
void file::load(QDate date, QString ampm, QString school)
{
    //prepare header
    header.empty();
    header << "Pointage Periscolaire SuperGPL v2"; //line 0
    header << "-------";                           //line 1
    header << QString("Logiciel version ").append(VERSION); //line 2
    header << "Listing version ?";                 //line 3
    header << "-------";                           //line 4
    header << school;                              //line 5
    header << date.toString("dd/MM/yyyy/").append(ampm.toUpper()); //line 6
    header << "-------";                           //line 7

    //load file
    QString name = date.toString("yyyy-MM-dd").append("-").append(ampm).append("-").append(school).append(".perisco");

    hfile.setFileName(name);

    //fichier existant -> check if valid header
    if (hfile.exists())
    {
        hfile.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream flux(&hfile);
        QStringList contentFILE;

        //get file content
        while(! flux.atEnd())
        {
            contentFILE.append(flux.readLine());
        }
        hfile.close();

        //check header && get content
        if (checkHeader(contentFILE)) loadContentInOut(contentFILE);
    }
}

void file::write()
{
    hfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream flux(&hfile);
    //flux.setCodec("UTF-8");

    for (int i = 0; i < header.size(); ++i) flux << header[i] << endl;

    for (int i = 0; i < contentIN.size(); ++i) flux << contentIN[i] << endl;
    flux << "-------" << endl;

    for (int i = 0; i < contentOUT.size(); ++i) flux << contentOUT[i] << endl;
    flux << "-------" << endl;

    hfile.close();
}

//check file header
bool file::checkHeader(QStringList file)
{
    if (file.size()<10) return false;
    if (file[1] != header[1]) return false;
    if (file[4] != header[4]) return false;
    if (file[5] != header[5]) return false;
    if (file[6] != header[6]) return false;
    if (file[7] != header[7]) return false;

    return true;
}

//load in memory from file
void file::loadContentInOut(QStringList file)
{
    int i=8;

    //PART 1
    while ((i<file.size()) && (file[i]!="-------"))
    {
        contentIN << file[i];
        i++;
    }

    //PART2
    i++;
    while ((i<file.size()) && (file[i]!="-------"))
    {
        contentOUT << file[i];
        i++;
    }
}

void file::setPresences(QStringList IN,QStringList OUT)
{
    contentIN = IN;
    contentOUT = OUT;

    write();
}

QStringList file::getPresencesIN()
{
    return contentIN;
}

QStringList file::getPresencesOUT()
{
    return contentOUT;
}
