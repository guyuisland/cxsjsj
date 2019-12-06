#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //this->setFixedSize( 540,360);
    ui->setupUi(this);
//    setTabOrder(ui->Username_Input,ui->Password_Input);
    //setStyleSheet ("background: #ffffff;color: #000000;");
    /*QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/Image/login.jpg")));
    setPalette(pal);*/

    QPainter paint(this);
    paint.setPen(QPen(Qt::green,4,Qt::SolidLine));
    paint.drawRect(20,20,160,160);

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_signupButtion_clicked()
{
    register_win = new Register;
    register_win->show();
}

void MainWindow::on_loginButton_clicked()
{
    //setStyleSheet("border-image:url(/Image/main2.png);");
    QString Login_un = ui->usernameEdit->text();
    QString Login_pw = ui->passwordEdit->text();
    qDebug() << Login_un << Login_pw;
    int judge = 1;
    if(judge == 1)
    {
        this->close();
        GameLobbyWin = new GameLobby;
        GameLobbyWin->show();
    }
}
