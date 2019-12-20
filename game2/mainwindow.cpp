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
    this->show();

    clientSocket = new ClientSocket;
//    std::thread t(clientSocket->Connect());
//    t.detach();
    int (ClientSocket::*p)() = &ClientSocket::Connect;
    QFutureWatcher<int> watcher;
    connect(&watcher, SIGNAL(finished()), this, SLOT(get_conn_ret()));

    QFuture<int> ret = QtConcurrent::run(clientSocket, p);
    watcher.setFuture(ret);
    //emit finished(watcher.future());
    //conn_code = ret.result();

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_signupButtion_clicked()
{
//    register_win = new Register;
//    register_win->show();
    QString Login_un = ui->usernameEdit->text();
    QString Login_pw = ui->passwordEdit->text();

    json sendinfo;
    sendinfo["define"] = SIGN_UP;
    sendinfo["username"]= Login_un.toStdString();
    sendinfo["password"]= Login_pw.toStdString();

    //如果注册成功
    json recvinfo = json::parse(clientSocket->Send(sendinfo.dump()));
    if(recvinfo["define"].get<int>() == SIGN_UP_SUCCESS)
    {
        QMessageBox::information(NULL, "", "注册成功",QMessageBox::Yes|QMessageBox::No);
        this->close();
        GameLobbyWin = new GameLobby(clientSocket);
        GameLobbyWin->show();
    }
    else{//用户名已存在
        QMessageBox::information(NULL, "", "用户名已存在",QMessageBox::Yes|QMessageBox::No);
    }
}

void MainWindow::on_loginButton_clicked()
{
    //setStyleSheet("border-image:url(/Image/main2.png);");
    QString Login_un = ui->usernameEdit->text();
    QString Login_pw = ui->passwordEdit->text();
    qDebug() << Login_un << Login_pw;
    Login_un = "hi";
    Login_pw = "123";
    json sendinfo;
    sendinfo["define"] = LOG_IN;
    sendinfo["username"]= Login_un.toStdString();
    sendinfo["password"]= Login_pw.toStdString();

    //如果登录成功
    json recvinfo = json::parse(clientSocket->Send(sendinfo.dump()));
    int ret = recvinfo["define"].get<int>();
    if(ret == LOG_IN_SUCCESS)
    {
        this->close();
        //GameLobbyWin = new GameLobby(clientSocket);
        //GameLobbyWin->show();
        _stackWidget = new StackWidget(clientSocket);
        _stackWidget->show();
    }
    else if(ret == LOG_IN_FAIL_AO)//用户已在线
    {
        QMessageBox::information(NULL, "", "用户已在线",QMessageBox::Yes|QMessageBox::No);
    }
    else if(ret == LOG_IN_FAIL_WP){//密码错误
        QMessageBox::information(NULL, "", "密码错误",QMessageBox::Yes|QMessageBox::No);
    }


}


void MainWindow::get_conn_ret()
{
    qDebug() << "here\n";
//    if(conn_code)
//    {
//        qDebug() << "Connect success.\n";
//    }
//    else {
//        qDebug() << "Connect failed.\n";
//    }
}
