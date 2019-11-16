#include "widget.h"
#include "ui_widget.h"
#include "json.hpp"
#include <string>

using json = nlohmann::json;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    socket=new QTcpSocket();//初始化客户端对象

    //链接信号槽
    //关联readyRead信号和socket_Read_Data槽函数.服务器有数据发送时,会自动触发readyRead信号
    QObject::connect(socket,&QTcpSocket::readyRead,this,&Widget::socket_Read_Data);
    ui->lineEdit_IP->setText("127.0.0.1");
    ui->lineEdit_port->setText("6666");
}

void Widget::on_pushButton_Send_clicked(){
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

void Widget::on_pushButton_Connect_clicked(){
    socket->abort();
    socket->connectToHost(ui->lineEdit_IP->text(),ui->lineEdit_port->text().toInt());//连接服务器

    if(!socket->waitForConnected(30000)){
        qDebug()<<tr("连接服务器失败,请重新操作!");
    }
    else{
        qDebug()<<tr("连接服务器成功!");
        QMessageBox::warning(this,tr("连接提示"),tr("连接服务器成功!"),QMessageBox::Yes, QMessageBox::No);
    }
}
void Widget::socket_Read_Data(){
    QByteArray buffer;
    buffer=socket->readAll();
    QString b = buffer;
    std::string b1 = b.toStdString();
    qDebug("here1");
    json j = json::parse(b1);
    qDebug("here2");
    ui->textEdit_recv->append("aaaa\n");
    double t = j["pi"].get<double>();
    qDebug("here3");
    ui->textEdit_recv->append(QString("%1").arg(t));

    ui->textEdit_recv->append(QString::number(t));

}
void Widget::socket_Disconnected(){

}
Widget::~Widget()
{
    delete  this->socket;
    delete ui;
}
