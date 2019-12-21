#ifndef GAMELOBBY_H
#define GAMELOBBY_H

#include <QDialog>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QPaintEvent>
#include <QtWidgets>
#include <QDebug>
#include <QMessageBox>
#include <QFutureWatcher>
#include <QtConcurrent>
#include "fightui.h"
#include "clientsocket.h"
#include <vector>


namespace Ui {
class GameLobby;
}

class GameLobby : public QDialog
{
    Q_OBJECT

public:
    explicit GameLobby(ClientSocket *client, json &, QWidget *parent = nullptr);
    ~GameLobby();

public slots:
    void on_updateButton_clicked();

private slots:
    void on_rankButton_clicked();

    void on_waitButton_clicked();

    void invite_handle();

    void reply_handle();

    void update_handle();

    void send_handle();

signals:
    void clicked(int room);
    void closeAll();

private:
    void send_invitation(int);


    Ui::GameLobby *ui;
    FightUI *FightUIWin;
    void InitUi();
    void SetUserList();
    void mousePressEvent(QMouseEvent *event);
    void OpenFightWin();
    void reshow();
    QString myName;
    std::vector<std::string> wList;

    ClientSocket *_client;//与服务器连接的socket
    QFutureWatcher<std::string> *strWatcherPtr;
    QString invitedName;
    bool userExist[4] = {0,0,0,0};
};

#endif
