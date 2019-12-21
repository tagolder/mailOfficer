#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    initDataBase();

    AuthDialog * dia = new AuthDialog(this);
    dia->exec();

    login = dia->getUserName();
    password = dia->getUserPassword();

    ui->setupUi(this);

    postalItems->addTab(tab1, "Созданные");

    postalItems->addTab(tab2, "Отправленные");

    postalItems->setUsesScrollButtons(false);


    connect(postalItems, SIGNAL(currentChanged(int)), this, SLOT(clickTabWidget(int)));


    ui->verticalLayout->addWidget(postalItems);

    centralWidget()->setLayout(ui->gridLayout);

    QSqlQuery queryWaitPostal;
    QSqlQuery querySandPostal;

    queryWaitPostal.exec(QString("call get_all_postals_waiting()"));
    modelWaitPostal->setQuery(queryWaitPostal);
    qDebug() << modelWaitPostal->rowCount();
    tab1->setModel(modelWaitPostal);
    tab1->resizeColumnsToContents();
    tab1->resizeRowsToContents();
    tab1->horizontalHeader()->setStretchLastSection(true);

    querySandPostal.exec(QString("call get_all_postals()"));
    modelSandPostal->setQuery(querySandPostal);
    tab2->setModel(modelSandPostal);
    tab2->resizeColumnsToContents();
    tab2->resizeRowsToContents();
    tab2->horizontalHeader()->setStretchLastSection(true);

    ui->dropButton->setEnabled(false);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initDataBase()
{
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("taya");
    db.setDatabaseName("mail");

    db.open();
}

void MainWindow::on_exitAkkButton_clicked()
{
    AuthDialog * dia = new AuthDialog(this);
    dia->exec();

    login = dia->getUserName();
    password = dia->getUserPassword();

    QSqlQuery queryWaitPostal;
    QSqlQuery querySandPostal;

    queryWaitPostal.exec(QString("call get_postals_waiting()"));
    modelWaitPostal->setQuery(queryWaitPostal);
    tab1->setModel(modelWaitPostal);

    querySandPostal.exec(QString("call get_postals()"));
    modelSandPostal->setQuery(querySandPostal);
    tab2->setModel(modelSandPostal);
}

void MainWindow::on_createButton_clicked()
{
    QSqlQuery queryDeleteRow;
    QString index = modelWaitPostal->index(tab1->selectionModel()->currentIndex().row(), 0).data().toString();

    QMessageBox * pMes =  new QMessageBox(QMessageBox::Question, "Отправить",
                                          "Хотите подтвердить отправку с индексом : " + index + " ?",
                                          QMessageBox::Yes | QMessageBox::No, this);

    if(pMes->exec() == QMessageBox::Yes)
    {
        queryDeleteRow.exec(QString("call from_waiting('%1')")
                   .arg(index));

        QSqlQuery queryWaitPostal;
        queryWaitPostal.exec(QString("call get_all_postals_waiting()"));
        modelWaitPostal->setQuery(queryWaitPostal);
        tab1->setModel(modelWaitPostal);
    }
}

void MainWindow::on_dropButton_clicked()
{
    int size1 = modelSandPostal->rowCount();
    QSqlQuery queryDeleteRow;
    QString index = modelSandPostal->index(tab2->selectionModel()->currentIndex().row(), 0).data().toString();

    QMessageBox * pMes =  new QMessageBox(QMessageBox::Question, "Отправить",
                                          "Хотите подтвердить получение отправления с индексом : " + index + " ?",
                                          QMessageBox::Yes | QMessageBox::No, this);

    if(pMes->exec() == QMessageBox::Yes)
    {
        queryDeleteRow.exec(QString("call from_postal_items('%1')")
                   .arg(index));

        QSqlQuery querySandPostal;
        querySandPostal.exec(QString("call get_all_postals()"));
        modelSandPostal->setQuery(querySandPostal);
        int size2 = modelSandPostal->rowCount();
        tab2->setModel(modelSandPostal);
        if(size2 == size1)
        {
            QMessageBox * Mes =  new QMessageBox(QMessageBox::Information, "Ой...",
                                                  "Вы слишком торопите события",
                                                  QMessageBox::Ok, this);
            Mes->exec();
        }
    }
}

void MainWindow::clickTabWidget(int index)
{
    switch (index) {
    case 0:
    {
        QSqlQuery queryWaitPostal;
        queryWaitPostal.exec(QString("call get_all_postals_waiting()"));
        modelWaitPostal->setQuery(queryWaitPostal);
        tab1->setModel(modelWaitPostal);
        ui->dropButton->setEnabled(false);
        ui->createButton->setEnabled(true);
        break;
    }
    case 1:
    {
        QSqlQuery querySandPostal;
        querySandPostal.exec(QString("call get_all_postals()"));
        modelSandPostal->setQuery(querySandPostal);
        tab2->setModel(modelSandPostal);
        ui->createButton->setEnabled(false);
        ui->dropButton->setEnabled(true);
        break;
    }
    default:
        break;
    }
}
