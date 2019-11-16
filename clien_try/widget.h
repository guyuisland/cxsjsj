#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTcpSocket>
#include<QMessageBox>
#include<QString>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
     void on_pushButton_Connect_clicked();//链接按钮槽函数

     void on_pushButton_Send_clicked();//发送按钮槽函数

     void socket_Read_Data();//读服务器数据槽函数

     void socket_Disconnected();//都拿开与服务器连接槽函数
private:
    Ui::Widget *ui;
    QTcpSocket *socket;
};

#endif // WIDGET_H
