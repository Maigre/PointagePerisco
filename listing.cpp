#include "listing.h"
#include "ui_listing.h"

Listing::Listing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Listing)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    selectedAdh = -1;
}

Listing::~Listing()
{
    delete ui;
}

void Listing::init()
{
    //LOAD Listing
    QDir dir;
    dir.cd("config");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Time | QDir::Reversed);
    QStringList filters;
    filters.empty();
    filters << "*.list";
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    if (list.size() > 0)
    {
        QFile lfile(list.at(0).filePath());
        lfile.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream flux(&lfile);
        flux.setCodec("UTF-8");

        //get file content
        liste.empty();
        while(! flux.atEnd()) liste.append(flux.readLine().trimmed());
        lfile.close();
        liste.sort();
    }
}

void Listing::clear()
{
    selectedAdh = -1;
    ui->lineEdit->clear();
    updatelist();
}

void Listing::updatelist()
{
    ui->listWidget->clear();
    QString src = ui->lineEdit->text().trimmed();
    if (src == "") ui->listWidget->addItems(getListe());
    else ui->listWidget->addItems(getListe(src));
}

QStringList Listing::extract(QString line)
{
    QStringList ans = line.split(" - ");
    QStringList ret;
    ret.append(ans[0].trimmed());
    if (ans.size() > 1) ret.append(ans[1].trimmed());
    else ret.append("0");
    return ret;
}

QString Listing::findName(int adh)
{
    for(int i=0; i<liste.size(); i++)
    {
        qDebug() << i << " - " << liste.size();
        qDebug() << extract(liste[i]);
        if (extract(liste[i]).last().toInt() == adh) return extract(liste[i]).first();
    }
    return "";
}

//get the full listing
QStringList Listing::getListe()
{
    return liste;
}

//liste with search field
QStringList Listing::getListe(QString src)
{
    QStringList ans;
    for(int i=0; i<liste.size(); i++)
    {
        if (liste[i].indexOf(src) >= 0) ans.append(liste[i]);
    }
    return ans;
}

void Listing::on_cancel_clicked()
{
    clear();
    close();
}

void Listing::on_lineEdit_textChanged(const QString &arg1)
{
    updatelist();
}

//select adh
void Listing::selectAdh()
{
    //if (ui->listWidget->selectedItems().count() > 0)
    this->selectedAdh = extract(ui->listWidget->selectedItems().at(0)->text()).last().toInt();
    //else
    //selectedAdh = -1;
}

//adh selected
void Listing::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    on_pushButton_clicked();
}

//adh selected
void Listing::on_pushButton_clicked()
{
    if (ui->listWidget->selectedItems().count() > 0)
    {
        selectAdh();
        close();
    }
}

int Listing::getSelected()
{
    int ret = selectedAdh;
    return ret;
}


