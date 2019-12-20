#ifndef GAMELOBBY_H
#define GAMELOBBY_H

#include <QDialog>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QPaintEvent>
#include <QtWidgets>
#include "fightui.h"
#include "clientsocket.h"

namespace Ui {
class GameLobby;
}

class GameLobby : public QDialog
{
    Q_OBJECT

public:
    explicit GameLobby(ClientSocket *client, QWidget *parent = nullptr);
    ~GameLobby();

private slots:
    void on_rankButton_clicked();

signals:
    void clicked(int room);

private:
    Ui::GameLobby *ui;
    FightUI *FightUIWin;
    void InitUi();
    void SetUserList();
    void mousePressEvent(QMouseEvent *event);
    void OpenFightWin();
    void reshow();

    ClientSocket *_client;//与服务器连接的socket
};

#endif // GAMELOBBY_H
