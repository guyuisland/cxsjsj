#ifndef FIGHTENV_H
#define FIGHTENV_H

#include"player.h"
#include"monsterfactory.h"
#include "clientsocket.h"
#include <QFutureWatcher>
#include <QtConcurrent>
#include<QObject>

class Fightenv
{
public:
    Fightenv(ClientSocket *);
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

private slots:
    void round_handle();


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

    Player me;
    Player opponent;
    int round;//当前回合数
    MonsterFactory* monsterFactory;
    ClientSocket *_client;
    QFutureWatcher<std::string> *strWatcherPtr;
    json jsBuf;
};

#endif // FIGHTENV_H
