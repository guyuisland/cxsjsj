#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //this->setFixedSize( 540,360);
    ui->setupUi(this);
    attack();
    /*QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/Image/start.jpg")));
    setPalette(pal);*/

    QPainter paint(this);
    paint.setPen(QPen(Qt::green,4,Qt::SolidLine));
    paint.drawRect(20,20,160,160);
    this->show();

    clientSocket = new ClientSocket;
    int (ClientSocket::*p)() = &ClientSocket::Connect;
    wp = new QFutureWatcher<int>;

    QFuture<int> ret = QtConcurrent::run(clientSocket, p);
    connect(wp, SIGNAL(finished()), this, SLOT(get_conn_ret()));
    wp->setFuture(ret);





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
    json recvinfo = json::parse(clientSocket->Send_Recv(sendinfo.dump()));
    if(recvinfo["define"].get<int>() == SIGN_UP_SUCCESS)
    {
        //QMessageBox::information(NULL, "", "注册成功",QMessageBox::Yes|QMessageBox::No);
        this->close();
        _stackWidget = new StackWidget(clientSocket, recvinfo);
        _stackWidget->show();
//        GameLobbyWin = new GameLobby(clientSocket);
//        GameLobbyWin->show();
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
    //Login_un = "hi";
    //Login_pw = "123";
    json sendinfo;
    sendinfo["define"] = LOG_IN;
    sendinfo["username"]= Login_un.toStdString();
    sendinfo["password"]= Login_pw.toStdString();

    //如果登录成功
    json recvinfo = json::parse(clientSocket->Send_Recv(sendinfo.dump()));
    int ret = recvinfo["define"].get<int>();
    if(ret == LOG_IN_SUCCESS)
    {
        this->close();
        //GameLobbyWin = new GameLobby(clientSocket);
        //GameLobbyWin->show();
        _stackWidget = new StackWidget(clientSocket, recvinfo);
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
//    qDebug() << wp->result();
//    if(conn_code)
//    {
//        qDebug() << "Connect success.\n";
//    }
//    else {
//        qDebug() << "Connect failed.\n";
//    }
    delete wp;
}
void MainWindow::attack()
{
    qDebug() << "attack" << ui->skill->y()<<  endl;
    QLabel *label = new QLabel(this);
    label->setVisible(true);
    QPixmap img1,img2;
    img1.load(":/Image/Fire_left.png");
    label->setPixmap(img1);
    QPropertyAnimation *animation = new QPropertyAnimation(label, "pos");



    //qDebug() << time.elapsed()/1000 << endl;
    animation->setDuration(2000);
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->setStartValue(QPoint(340,250));
    animation->setEndValue(QPoint(870,250));
    animation->start();
}
void MainWindow::on_pushButton_clicked()
{
    /*test*/
//    QLabel *myAttcakLabel = new QLabel(this);
//    myAttcakLabel->setGeometry(rect().x()+340, rect().y()+250,
//                                    70, 40);
//    myAttcakLabel->setParent(this);
//    //myAttcakLabel->setVisible(true);
//    QPixmap img1;
//    img1.load(":/Image/Fire_left.png");
//    myAttcakLabel->setPixmap(img1);
//    QPropertyAnimation *animation = new QPropertyAnimation(myAttcakLabel, "pos");
//    animation->setDuration(2000);
//    animation->setEasingCurve(QEasingCurve::InQuad);
//    animation->setStartValue(QPoint(340,250));
//    animation->setEndValue(QPoint(870,250));
//    animation->start();
//    timer->start(2000);
    /*test*/
    qDebug() << "attack" << ui->skill->y()<<  endl;
    ui->skill->setVisible(true);
    QPixmap img1,img2;
    img1.load(":/Image/Fire_left.png");
    ui->skill->setPixmap(img1);
    QPropertyAnimation *animation = new QPropertyAnimation(ui->skill, "pos");



    //qDebug() << time.elapsed()/1000 << endl;
    animation->setDuration(2000);
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->setStartValue(QPoint(340,250));
    animation->setEndValue(QPoint(870,250));
    animation->start();

}
