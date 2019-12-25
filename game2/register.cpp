#include "register.h"
#include "ui_register.h"
#include <QDebug>


Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

}

Register::~Register()
{
    delete ui;
}

void Register::on_signup_clicked()
{
    QString Register_un = ui->Register_Un->text();
    QString Register_pw = ui->Register_Pw->text();
    qDebug() << Register_un << Register_pw;
    int judge = 1;
    if(judge == 1)
    {
        QMessageBox::information(NULL, "", "注册成功",QMessageBox::Yes|QMessageBox::No);
        this->close();
    }
}
