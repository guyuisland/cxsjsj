#ifndef FIGHTENV_H
#define FIGHTENV_H

#include <QDialog>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QLabel>
#include"player.h"
#include"monsterfactory.h"
#include "clientsocket.h"

namespace Ui {
class Fightenv;
}

class Fightenv : public QDialog
{
    Q_OBJECT

public:
    explicit Fightenv(ClientSocket *, QWidget *parent = nullptr);
    Fightenv(Player& , Player&);
    ~Fightenv();
    /*xxx(){
     * me.attack(sendinfo);
     * send();
     * recv();
     * cmp_excute(sendinfo,recvinfo);
     * if(gameover){send(win/lose) recv(xxx);}
     * draw();
     *
     * }
    */
    void set_players(Player , Player);

    void cmp_excute(json&,json&);
    int game_over();
    void attack_handle(int);
    void defend_handle();
    void acc_handle();
    void summon_handle(int );
    void skill_handle(int , int, int);
    void surrender_handle();

    int has_obj(int , int);

    //根据UI获得选择转到相应处理逻辑
    void dispatch(int , int , int );

signals:
    //动画槽函数
    void on_my_attack(int );
    void on_my_defend();
    void on_my_acc();//我方蓄气
    void on_my_summon(int );//我方召唤
    void on_my_monster_dead(int );
    void on_my_skill(int , int);
    void on_my_surrender();
    void on_my_win();
    void on_my_lose();
    void on_update_my_HP(int , int, int);
    void on_update_my_MP(int);

    void on_opp_attack(int );
    void on_opp_defend();
    void on_opp_acc();//对方蓄气
    void on_opp_summon(int );//对方召唤
    void on_opp_monster_dead(int );
    void on_opp_skill(int , int);
    void on_opp_surrender();
    void on_update_opp_HP(int , int, int);
    void on_update_opp_MP(int);

    void on_serv_timeout();//服务器超时

private slots:
    void round_handle();

    void round_result();

private:
    void summon_summon(json&,json&);
    void summon_attack(json&,json&);
    void summon_accumulate(json&,json&);
    void summon_defend(json&,json&);
    void summon_skill(json&,json&);
    void summon_surrender(json&,json&);

    void attack_attack(json&,json&);
    void attack_accumulate(json&,json&);
    void attack_defend(json&,json&);
    void attack_skill(json&,json&);
    void attack_surrender(json&,json&);

    void accumulate_accumulate(json&,json&);
    void accumulate_defend(json&,json&);
    void accumulate_skill(json&,json&);
    void accumulate_surrender(json&,json&);

    void defend_defend(json&,json&);
    void defend_skill(json&,json&);
    void defend_surrender(json&,json&);

    void skill_skill(json&,json&);
    void skill_surrender(json&,json&);

    void surrender_surrender(json&,json&);

    void send_choice(json&);

    Ui::Fightenv *ui;
    Player me;
    Player opponent;
    int round;//当前回合数
    MonsterFactory* monsterFactory;
    ClientSocket *_client;
    QFutureWatcher<std::string> *strWatcherPtr;
    json jsBuf;
    QWidget *_parent;

};

#endif // FIGHTENV_H
