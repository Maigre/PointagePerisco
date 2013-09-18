#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Clock()));
    timer->start(1000);
    Clock();

    ui->selGroup->hide();
    ui->selHider->hide();
    ui->cancelALL->hide();

    //Date
    SysDate = QDate::currentDate();

    // AM-PM
    if (QTime::currentTime().hour() > 12) SysAmpm = "PM";
    else SysAmpm = "AM";

    //Ecole
    SysSchool = QString::fromUtf8("Periscolaire");
    QDir dir;
    dir.cd("config");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QStringList filters;
    filters << "*.ecole";
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        SysSchool = list.at(i).fileName().split('.').at(0);
    }
    ui->Ecole->setText(SysSchool);

    //Initialize Pointer
    pointer.init(SysDate,SysAmpm,SysSchool);

    //Display init
    RefreshLists();
    ui->lineEdit->setFocus();
}

MainWindow::~MainWindow()
{
    timer->stop();
    delete ui;
}

//Gestion de la fermeture
void MainWindow::closeEvent( QCloseEvent * event )
{
    pointer.Refresh();

    if (pointer.finnishedAll())
    {
        event->accept();
    }
    else
    {
        int ans = QMessageBox::question(this, "Alerte", "Il reste des pointages non terminés !\nÊtes vous sur de vouloir quitter ?", QMessageBox::Yes | QMessageBox::No);
        if (ans == QMessageBox::Yes) event->accept();
        else event->ignore();
    }
}

//Bouton pointage
void MainWindow::on_pushButton_clicked()
{
    //point numero & affiche result
    this->ResultPointage( pointer.Point(ui->lineEdit->text()) );
}

//Touche ENTER pour pointer
void MainWindow::on_lineEdit_returnPressed()
{
    //point numero & affiche result
    ui->pushButton->click();
}

//Affichage resultat pointage
void MainWindow::ResultPointage(bool success)
{
    on_pushButton_5_clicked();

    ui->lineEdit->clear();
    RefreshLists();
    DisplayMessage(pointer.messageTitle,pointer.message,success);
}

//Affiche message INFO
void MainWindow::DisplayMessage(QString msg)
{
    ui->messageTitle->setText("");
    ui->message->setText(msg);
}

//Affiche message TITLE + INFO + COLOR
void MainWindow::DisplayMessage(QString title, QString msg, bool success)
{
    if (success) ui->message->setStyleSheet("QLabel { color : green; }");
    else ui->message->setStyleSheet("QLabel { color : red; }");

    if (success) ui->messageTitle->setStyleSheet("QLabel { color : green; }");
    else ui->messageTitle->setStyleSheet("QLabel { color : red; }");

    DisplayMessage(msg);
    ui->messageTitle->setText(title);
}

//Recharge les lists
void MainWindow::RefreshLists()
{
    //update stored list
    pointer.Refresh();

    //update display lists
    ui->listIN->clear();
    ui->listIN->addItems(pointer.listDisplay(false));

    ui->listOUT->clear();
    ui->listOUT->addItems(pointer.listDisplay(true));
}

//Garde le focus sur la ligne de saisie
void MainWindow::on_lineEdit_lostFocus()
{
    ui->lineEdit->setFocus();
}

//Clock update
void MainWindow::Clock()
{
    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit->setTime(QTime::currentTime());
}

void MainWindow::on_listIN_itemClicked(QListWidgetItem *item)
{
    ui->listOUT->clearSelection();
    SelectItem(item->text());
}

void MainWindow::on_listOUT_itemClicked(QListWidgetItem *item)
{
    ui->listIN->clearSelection();
    SelectItem(item->text());
}

void MainWindow::SelectItem(QString sel)
{
    QStringList explode;
    explode = sel.split("]");
    explode = explode[0].split("[");
    if (explode.size() > 1)
    {
        bool ok;
        int adh = explode[1].toInt(&ok);
        if(ok && (adh>0))
        {
            int index = pointer.Search(adh);
            if (index >= 0)
            {
                QString msg = "Modification en cours du pointage ";
                msg.append(sel);
                DisplayMessage(msg);

                ui->selGroup->show();
                ui->selHider->show();

                ui->selAdh->setText(QString("%1").arg(pointer.presences[index].getAdh()));

                ui->cancelIN->show();
                ui->selIN->show();
                ui->labelIN->show();

                //terminé
                if (pointer.presences[index].finnished())
                {
                    ui->selStatut->setText("Terminé");
                    ui->cancelOUT->show();
                    ui->selOUT->show();
                    ui->labelOUT->show();
                    ui->pointOUT->hide();

                    ui->selOUT->setTime(pointer.presences[index].getOUT());
                }
                //en cours
                else on_cancelOUT_clicked();


                ui->selIN->setTime(pointer.presences[index].getIN());
            }
        }
    }
}

//Supprimer le départ
void MainWindow::on_cancelOUT_clicked()
{
    ui->selOUT->setTime(QTime(0,0,0));
    ui->cancelOUT->hide();
    ui->selOUT->hide();
    ui->labelOUT->hide();
    ui->pointOUT->show();
    ui->selStatut->setText("En cours");
}

//supprimer arrivée
void MainWindow::on_cancelIN_clicked()
{
    on_cancelOUT_clicked();
    ui->selIN->setTime(QTime(0,0,0));
    ui->cancelIN->hide();
    ui->selIN->hide();
    ui->labelIN->hide();
    ui->pointOUT->hide();
    ui->selStatut->setText("Annulé");
}

//Annuler les modifications de selection
void MainWindow::on_pushButton_5_clicked()
{
    ui->selGroup->hide();
    ui->selHider->hide();

    QString msg = "Modifications annulées !";
    DisplayMessage(msg);
    RefreshLists();
}

//Confirmer les modifications
void MainWindow::on_selOK_clicked()
{
    bool ok;
    int adh = ui->selAdh->text().toInt(&ok);
    if(ok && (adh>0))
    {
        int index = pointer.Search(adh);
        if (index >= 0)
        {
            pointer.presences[index].setIN(ui->selIN->time());
            pointer.presences[index].setOUT(ui->selOUT->time());
        }
    }

    //close select win
    on_pushButton_5_clicked();

    //refresh
    RefreshLists();

    QString msg = "Modifications enregistrées !";
    DisplayMessage(msg);
}

//pointer le départ manuellement
void MainWindow::on_pointOUT_clicked()
{
    //point numero & affiche result
    ui->lineEdit->setText(ui->selAdh->text());
    ui->pushButton->click();
}

void MainWindow::on_pushButton_2_clicked()
{
    QTime now = QTime::currentTime();
    QString msg = "Êtes vous sûr de vouloir terminer toutes les présences en cours ?  L'heure utilisée pour les départ sera ";
    msg.append(now.toString());
    int ans = QMessageBox::question(this, "Confirmation", msg, QMessageBox::Yes | QMessageBox::No);
    if (ans == QMessageBox::Yes)
    {
        pointer.PointOutAll(now);
        RefreshLists();
        DisplayMessage("Tous les pointages sont terminés !");
    }
}

//CHECK FILE INTEGRITY
void MainWindow::on_pushButton_3_clicked()
{
    pointer.Refresh();

    QString name = SysDate.toString("yyyy-MM-dd").append("-").append(SysAmpm).append("-").append(SysSchool).append(".perisco");
    QFile tfile(name);

    tfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream flux(&tfile);
    QString tout = "Si ces informations ne vous semblent pas cohérentes, veuillez noter le pointage à la main sur la feuille de présence et informer un responsable du problème.\n\n";

    QString ligne;
    int i = 0;
    while(! flux.atEnd())
    {
        ligne = flux.readLine();
        i++;
    }

    tout.append("Fichier: ").append(name);
    tout.append("\nNombre d'adhérents dans le fichier de pointage: ").append(QString("%1").arg(i-10));

    tfile.close();

    QMessageBox::information(this, "Verification du fichier", tout);

}

void MainWindow::on_cancelALL_clicked()
{
    int ans = QMessageBox::question(this, "Alerte", "Êtes vous sûr de vouloir supprimer TOUT le contenu de ce pointage ?", QMessageBox::Yes | QMessageBox::No);
    if (ans == QMessageBox::Yes)
    {
        pointer.presences.clear();
        RefreshLists();
    }
}

void MainWindow::on_QUIT_clicked()
{
    close();
}

void MainWindow::on_MORE_clicked()
{
    ui->MORE->hide();
    ui->cancelALL->show();
}

void MainWindow::on_LISTING_clicked()
{

    pointer.lister.setWindowFlags( windowFlags() & ~Qt::WindowMinimizeButtonHint );
    pointer.lister.setModal(true);

    pointer.lister.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            pointer.lister.size(),
            qApp->desktop()->availableGeometry()
        ));

    pointer.lister.setFixedSize(pointer.lister.width(),pointer.lister.height());

    pointer.lister.clear();
    pointer.lister.show();
    pointer.lister.exec();
    int selected = pointer.lister.getSelected();

    if (selected > 0)
    {
        ui->lineEdit->setText(QString::number(selected));
        ui->pushButton->click();
    }
}


