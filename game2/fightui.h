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
    void summon_monster(int n);
    void renew_enemy_monster();
    void show_blood();
    void get_add(QMovie *movie, int seq);

    MyLabel *myLabel;
    QPixmap PixmapToRound(const QPixmap &src, int radius);
    QString skill[4] ={"1","2","3","4"};
    ClientSocket *_client;//与服务器连接的socket
    int ObjectSelect = -1;
    int SkillSelect = -1;
    int monsterNum = 1;
    int MySummonSeq[4] = {-1,-1,-1,-1};//我召唤在某个位置是哪个怪物
    int myMonBlood[4] = {100,100,100,100};//我的怪物血量
    int eneMonBlood[4] =  {10,10,10,10};//敌人怪物血量
    int EnemySummonSeq[4] = {-1,3,1,2};//敌人召唤某个位置是什么怪物
    bool myObjectExist[4] = {1,0,0,0};//怪物是否已经被召唤
    bool eneObjectExist[4] = {1,1,1,1};//怪物是否已经被召唤
    bool SummonExist[4] = {1,0,0,0};//使用召唤技能时显示
    string PhotoAdd[4] = {":/Image/charizard_left.gif",":/Image/dodrio_left.gif",":/Image/lapras_left.gif",":/Image/talonflame_left.gif"};
    string EnePhotoAdd[4] = {":/Image/charizard.gif",":/Image/dodrio.gif",":/Image/lapras.gif",":/Image/talonflame.gif"};
    void use_skill();
    void attack();
    int temp = 0;
    int eneMonSelect = -1;
    //void enterEvent(QEvent *event);
    QTimer *timer;
    Fightenv *fightEnv;//记录战斗场面信息
    void submit();

    int have_object();
    int costmonster = 2;
    int num = 2;
    int monsterselect = 0;
};

#endif // FIGHTUI_H
