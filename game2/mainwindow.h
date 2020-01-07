#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMessageBox>

#include "register.h"
#include "gamelobby.h"
#include "fightui.h"
#include "clientsocket.h"
#include "json.hpp"
#include "QtConcurrent"
#include "qfuture.h"
#include "qfuturewatcher.h"
#include "stackwidget.h"

using json=nlohmann::json;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void socket_connect();

public slots:
    void get_conn_ret();

private slots:

    void on_signupButtion_clicked();

    void on_loginButton_clicked();



    //void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Register *register_win;
    GameLobby *GameLobbyWin;
    ClientSocket* clientSocket;
    //int conn_code;//是否连接成功
    StackWidget* _stackWidget;
    QFutureWatcher<int> *wp ;
    QTimer *timer;
    QLabel *rebound;
    void attack();
    QLabel *myOptLabel = new QLabel(this);
};

#endif // MAINWINDOW_H
