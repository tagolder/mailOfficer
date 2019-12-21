#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableView>
#include <QDialog>
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFile>
#include <QDebug>
#include "authdialog.h"
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void clickTabWidget(int index);

    void on_exitAkkButton_clicked();


    void on_createButton_clicked();

    void on_dropButton_clicked();

private:

    Ui::MainWindow *ui;

    QString login = "";
    QString password = "";

    QTabWidget * postalItems = new QTabWidget;
    QTableView * tab1 = new QTableView;
    QTableView * tab2 = new QTableView;

    QSqlQueryModel * modelWaitPostal = new QSqlQueryModel;
    QSqlQueryModel * modelSandPostal = new QSqlQueryModel;

    void initDataBase();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

};

#endif // MAINWINDOW_H
