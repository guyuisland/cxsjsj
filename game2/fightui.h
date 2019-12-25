#ifndef FIGHTUI_H
#define FIGHTUI_H

#include <QDialog>
#include<QTimer>
#include <QLabel>
#include<QPixmap>
#include<QBitmap>
#include <QMouseEvent>
#include "mylabel.h"
#include "clientsocket.h"
#include "fightenv.h"
#include "player.h"


namespace Ui {
class FightUI;
}

class FightUI : public QDialog
{
    Q_OBJECT

public:
    explicit FightUI(ClientSocket *client, QWidget *parent = nullptr);
    ~FightUI();
    bool eventFilter(QObject *obj, QEvent *event);	// 添加时间过滤器声明
    void init_player(QString &, QString &, int , int);
signals:
    void sendsignal();
    void clicked(int );

public slots:
    //动画槽函数
    void my_attack(int );
    void my_defend();
    void my_acc();//我方蓄气
    void my_summon(int );//我方召唤
    void my_monster_dead(int );
    void my_skill(int , int);
    void my_surrender();
    void my_win();
    void my_lose();
    void my_rebound();
    void my_evade(int );
    void my_heal(int );
    void update_my_HP(int , int, int);
    void update_my_MP(int);


    void opp_attack(int );
    void opp_defend();
    void opp_acc();//对方蓄气
    void opp_summon(int );//对方召唤
    void opp_monster_dead(int );
    void opp_skill(int , int);
    void opp_surrender();
    void opp_rebound();
    void opp_evade(int );
    void opp_heal(int );
    void update_opp_HP(int , int, int);
    void update_opp_MP(int);


    void serv_timeout();//服务器超时

private slots:
    void hide_ski_anm();
    void on_pushButton_clicked();



private:
    Ui::FightUI *ui;
    void InitUi();
    void click_monster1();
    void skill_des(int seq);
    void renew_skill(int seq);
    void hide_summon();
    void show_summon();
    void init_connect();
    void summon_monster(int n);
    void renew_enemy_monster(vector<int> );
    void show_blood(vector<int> ,vector<int>,vector<int> ,vector<int>);
    void get_add(QMovie *movie, int seq);
    void reshow_my_HP();
    void renew_my_monster(vector<int> );
    //void renew_massage();
    void renew_MP(vector<int>);
    void renew_my_blood(vector<int> ,vector<int>);
    void renew_enemy_blood(vector<int> ,vector<int>);

    MyLabel *myLabel;
    QPixmap PixmapToRound(const QPixmap &src, int radius);
    QString skill[4] ={"1","2","3","4"};
    ClientSocket *_client;//与服务器连接的socket
    int ObjectSelect = -1;
    int SkillSelect = -1;
    int monsterNum = 1;
    int enemonsterNum = 1;
    int MySummonSeq[4] = {-1,-1,-1,-1};//我召唤在某个位置是哪个怪物
    //int myMonBlood[4] = {100,100,100,100};//我的怪物血量
    //int eneMonBlood[4] =  {10,10,10,10};//敌人怪物血量
    int myMP = 100;
    int eneMP = 100;
    int sumConsume[4] = {0,20,30,40};
    QString skillDes[4][4] = { {"防御","攻击","蓄积能量","召唤"}};
    int EnemySummonSeq[4] = {-1,-1,-1,-1};//敌人召唤某个位置是什么怪物
    bool myObjectExist[4] = {1,0,0,0};//怪物是否已经被召唤
    bool eneObjectExist[4] = {1,0,0,0};//怪物是否已经被召唤
    bool SummonExist[4] = {1,0,0,0};//使用召唤技能时显示
    string PhotoAdd[4] = {":/Image/charizard_left.gif",":/Image/dodrio_left.gif",":/Image/lapras_left.gif",":/Image/talonflame_left.gif"};
    string EnePhotoAdd[4] = {":/Image/charizard.gif",":/Image/dodrio.gif",":/Image/lapras.gif",":/Image/talonflame.gif"};
    void use_skill();
    void attack();

    int temp = 0;
    int eneMonSelect = -1;
    //void enterEvent(QEvent *event);
    QTimer *timer;
    QTimer *timer2;
    QTimer *messageTimer;
    Fightenv *fightEnv;//记录战斗场面信息
    void submit();

    int have_object();
    int costmonster = 2;
    int num = 2;
    int monsterselect = 0;
    QLabel *rebound;
    void rebound_ski();//
    
    
    QLabel *myAttackLabel;
    QLabel *eneAttackLabel;
};

#endif // FIGHTUI_H
