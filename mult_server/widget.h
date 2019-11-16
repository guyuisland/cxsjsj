#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTcpServer>
#include<QTcpSocket>
#include<QMessageBox>
#include<QList>
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
    void on_pushButton_Listen_clicked();

    void on_pushButton_send_clicked();

    void server_New_Connect();

    void socket_Read_Data();

    void socket_Disconnect();
private:

    Ui::Widget *ui;
    QTcpServer *server;
    QTcpSocket *socket;
    QList<QTcpSocket*> tcpSocket;
};

#endif // WIDGET_H
