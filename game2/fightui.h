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
#include <vector>

const std::vector<QString> NAME = {"火伊布", "皮卡丘", "甲贺忍蛙", "快龙"};

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
    void my_summon(int, int );//我方召唤
    void my_monster_dead(int );
    void my_skill(int , int);
    void my_surrender();
    void my_win();
    void my_lose();
    void my_rebound();
    void my_evade();
    void my_heal(int );
    void update_my_HP();
    void update_my_MP();


    void opp_attack(int );
    void opp_defend();
    void opp_acc();//对方蓄气
    void opp_summon(int, int );//对方召唤
    void opp_monster_dead(int );
    void opp_skill(int , int);
    void opp_surrender();
    void opp_rebound();
    void opp_evade();
    void opp_heal(int );
    void update_opp_HP();
    void update_opp_MP();


    void serv_timeout();//服务器超时

    void back();
    void button_visible();

private slots:
    void hide_ski_anm();
    void hide_opp_ski_anm();
    void on_pushButton_clicked();
    void hide_message();


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
    void renew_my_blood(vector<int> ,vector<int>,vector<int> maxBlood);
    void renew_enemy_blood(vector<int> ,vector<int>,vector<int> maxBlood);
    void renew_skill_photo(int);
    //void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);

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
    QString skillDes[5][4] = { {"幻雷","焚界","霖愈","聚灵"},{"疾风步","攻击","剑刃风暴","再生"},{"反弹","撕咬","淬毒","枯萎"},{"血暮","怖刃","暗瞳","敏捷"},{"攻击","防御","蓄积能量","召唤"}};
    QString skillDetDes[5][4] = { {"秒杀对手某一只怪兽","穿透，对每个怪兽造成2点伤害","治疗己方怪兽4点血量","为所有怪兽回复1点血量"},{"躲避当前攻击，并且下次攻击翻倍","对对手的怪兽造成2点伤害","对对方的所有怪兽造成3点伤害","如果对方能让自己某只怪兽死亡，则恢复全部血量并且抵挡此次攻击"},{"可在接下来两回合内反弹对手的攻击","造成3点伤害，回复自身1点血量。","对敌人所有怪物造成1点伤害","穿透，造成8点伤害"},{"击杀对手所有怪兽","攻击","造成1点伤害，加1点能量","闪避当前伤害，回敬1点伤害"},{"攻击","防御","蓄积能量","召唤"}};
    int skillType[5][4] = {{0,0,4,4},{2,0,0,4},{2,0,0,0},{0,0,0,2},{0,2,1,3}};//记录怪物的技能类型,0-3是四种怪物的，4是自己的技能
    int EnemySummonSeq[4] = {-1,-1,-1,-1};//敌人召唤某个位置是什么怪物
    bool myObjectExist[4] = {1,0,0,0};//怪物是否已经被召唤
    bool eneObjectExist[4] = {1,0,0,0};//怪物是否已经被召唤
    bool SummonExist[4] = {1,0,0,0};//使用召唤技能时显示
    int maxBlood[4] = {6,6,6,6};
    string PhotoAdd[4] = {":/Image/flareon_left.gif",":/Image/pikachu_left.gif",":/Image/greninja_left.gif",":/Image/dragonite_left.gif"};
    string EnePhotoAdd[4] = {":/Image/flareon.gif",":/Image/pikachu.gif",":/Image/greninja.gif",":/Image/dragonite.gif",};
    string skillPhotoAdd[5] = {":/Image/attack.jpeg",":/Image/store.jpg",":/Image/defend.jpeg",":/Image/summon.jpg",":/Image/heal.jpg"};
    void use_skill();
    void attack();

    int temp = 0;
    int eneMonSelect = -1;
    //void enterEvent(QEvent *event);
    QTimer *myOptTimer;
    QTimer *eneOptTimer;
    QTimer *messageTimer;
    Fightenv *fightEnv;//记录战斗场面信息
    void submit();

    int have_object();
    int costmonster = 2;
    int num = 2;
    int monsterselect = -1;
    QLabel *rebound;
    void rebound_ski();//
    
    
    QLabel *myOptLabel = new QLabel(this);
    QLabel *eneOptLabel= new QLabel(this);
};

#endif // FIGHTUI_H
