#include "widget.h"
#include "ui_widget.h"
#include "json.hpp"
#include <QDebug>

using json = nlohmann::json;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    server= new QTcpServer();//初始化QTcpSever对象
    //当服务器被客户端访问时,会触发newconnect信号,可以利用信号作为触发信号,绑定槽函数server_New_Connect
    connect(server,&QTcpServer::newConnection,this,&Widget::server_New_Connect);
    ui->lineEdit_IP->setText("127.0.0.1");//设置默认IP
    ui->lineEdit_Port->setText("6666");//设置默认端口号
}
void Widget::on_pushButton_Listen_clicked(){
    //侦听一个端口,客户端可以使用这个端口访问服务器
    QHostAddress IP(ui->lineEdit_IP->text());//服务器端ip
    quint16 port=ui->lineEdit_Port->text().toUInt();//服务器端口号port
    if(server->listen(QHostAddress::Any,port)){//监听客户端ip和port
        //监听成功
        qDebug()<<"侦听成功";
    }
    else{
        //监听失败
        QMessageBox::warning(this,"warning",tr("监听失败!"),QMessageBox::Yes,QMessageBox::No);
    }
}

void Widget::on_pushButton_send_clicked(){

    qint64 writeResult=socket->write(ui->textEdit_send->toPlainText().toLatin1());//发送socket数据包，toLatin1把Qstring转换为char*

    bool BoolFlush=socket->flush();//从缓冲区读写数据,如果从缓冲区读写了数据,则返回true,没有数据读写,则返回false
    if(writeResult!=-1&&BoolFlush==1){
        //返回值不为-1,则数据发送成功
        if(writeResult==0){
            QMessageBox::warning(this,"warning",tr("写数据结果返回值为0"),QMessageBox::Yes,QMessageBox::No);
        }
        QMessageBox::warning(this,"warning",tr("写数据成功! %1").arg(writeResult),QMessageBox::Yes,QMessageBox::No);
    }
}

void Widget::server_New_Connect(){
    //获取客户端连接
    QTcpSocket* clientConnection = socket=server->nextPendingConnection();
    tcpSocket.append(clientConnection);
    ui->textEdit_recv->append("Newclient:"+clientConnection->peerAddress().toString());
    //连接QTcpSocket的信号槽,以读取新数据
    //&QTcpSocket::readyRead是服务器收到客户端数据后,自动触发readyRead信号
    QObject::connect(socket,&QTcpSocket::readyRead,this,&Widget::socket_Read_Data);
    QObject::connect(socket,&QTcpSocket::disconnected,this,&Widget::socket_Disconnect);
}

void Widget::socket_Read_Data(){
    QByteArray buffer;
    QString recv_Buf=buffer;
    //buffer=socket->readAll();//读取缓冲区数据
    foreach(QTcpSocket* soc, tcpSocket){
        buffer=soc->readAll();
        recv_Buf=buffer;
        ui->textEdit_recv->append(buffer);
        if(recv_Buf=="hello"){
            //soc->write("hi",6);
            json j = "{ \"happy\": "", \"pi\": 3.141 }"_json;
            qDebug("here1");
            //auto s = j.get<std::string>();
            //qDebug() << typeid(s).name();
            std::string s = j.dump();
            qDebug("here2");
            QString s1 = QString::fromStdString(s);
            int a= s1.size();
            soc->write(s.c_str(),a);
        }

    }

}
void Widget::socket_Disconnect(){
    //
    int i=0;
    foreach(QTcpSocket* soc, tcpSocket){
        if(soc->state()==QTcpSocket::UnconnectedState)
        {
            qDebug()<<soc->peerAddress().toString()<<"closed"<<endl;
            soc->deleteLater();
            tcpSocket.removeAt(i);
        }
        i++;
    }

}
Widget::~Widget()
{
    server->close();
    server->deleteLater();
    delete ui;
}
