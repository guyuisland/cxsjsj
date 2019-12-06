#ifndef GAMELOBBY_H
#define GAMELOBBY_H

#include <QDialog>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QPaintEvent>
#include <QtWidgets>
#include "fightui.h"

namespace Ui {
class GameLobby;
}

class GameLobby : public QDialog
{
    Q_OBJECT

public:
    explicit GameLobby(QWidget *parent = nullptr);
    ~GameLobby();

private:
    Ui::GameLobby *ui;
    FightUI *FightUIWin;
    void InitUi();
    void SetUserList();
    void mousePressEvent(QMouseEvent *event);
    void OpenFightWin();
    void reshow();
};

#endif // GAMELOBBY_H
