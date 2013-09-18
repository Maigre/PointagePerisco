#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTimer>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDir>
#include <QFileInfoList>
#include <QDesktopWidget>
#include <QDebug>
#include "pointage.h"
#include "file.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent( QCloseEvent * event );

    void ResultPointage(bool success);
    void DisplayMessage(QString msg);
    void DisplayMessage(QString title, QString msg, bool success);
    void RefreshLists();
    void SelectItem(QString sel);
    
private slots:
    void on_pushButton_clicked();
    void on_lineEdit_lostFocus();
    void on_lineEdit_returnPressed();
    void Clock();

    void on_listIN_itemClicked(QListWidgetItem *item);

    void on_listOUT_itemClicked(QListWidgetItem *item);

    void on_pushButton_5_clicked();

    void on_cancelOUT_clicked();

    void on_cancelIN_clicked();

    void on_selOK_clicked();

    void on_pointOUT_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_cancelALL_clicked();

    void on_QUIT_clicked();

    void on_MORE_clicked();

    void on_LISTING_clicked();

private:
    Ui::MainWindow *ui;
    Pointage pointer;
    QTimer *timer;

    QDate SysDate;
    QString SysAmpm;
    QString SysSchool;
};

#endif // MAINWINDOW_H
