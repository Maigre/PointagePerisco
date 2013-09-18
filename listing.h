#ifndef LISTING_H
#define LISTING_H

#include <QDialog>
#include <QDir>
#include <QFileInfoList>
#include <QTextStream>
#include <QListWidgetItem>
#include <QDebug>

namespace Ui {
class Listing;
}

class Listing : public QDialog
{
    Q_OBJECT
    
public:
    explicit Listing(QWidget *parent = 0);
    ~Listing();

    void init();
    void clear();
    void updatelist();

    QStringList extract(QString line);

    QString findName(int adh);

    QStringList getListe();
    QStringList getListe(QString src);

    int getSelected();

    
private slots:
    void on_cancel_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

private:
    Ui::Listing *ui;
    QStringList liste;
    int selectedAdh;

    void selectAdh();
};

#endif // LISTING_H
