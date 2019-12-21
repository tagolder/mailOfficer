#include "authdialog.h"
#include "ui_authdialog.h"

AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::on_authButton_clicked()
{
    userName = ui->editName->text();
    userPassword = ui->editPassword->text();

    QSqlQuery query;
    query.exec(QString("call get_offiser_password('%1')")
               .arg(userName));
    QSqlQueryModel * modelPassword = new QSqlQueryModel;
    modelPassword->setQuery(query);

    if(userName != "" && userPassword != "")
    {
        if(modelPassword->rowCount() == 0)
        {
            QSqlQuery query1;
            query1.exec(QString("call create_new_offiser('%1', '%2')")
                       .arg(userName, userPassword));
        }
        this->close();
    }
}
