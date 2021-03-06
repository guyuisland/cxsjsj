﻿#include "fightui.h"
#include "ui_fightui.h"
#include<QMovie>
#include<QPainter>
#include<QPixmap> 
#include<QMessageBox>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include<QPropertyAnimation>

FightUI::FightUI(ClientSocket *client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FightUI),
    _client(client)
{
    ui->setupUi(this);
    InitUi();
    myOptTimer = new QTimer;
    eneOptTimer = new QTimer;
    messageTimer = new QTimer;
    connect(myOptTimer, SIGNAL(timeout()), this, SLOT(hide_ski_anm()));
    connect(eneOptTimer, SIGNAL(timeout()), this, SLOT(hide_opp_ski_anm()));
    connect(messageTimer, SIGNAL(timeout()), this, SLOT(hide_message()));
    fightEnv = new Fightenv(_client);
    ui->skill0->setMouseTracking(true);
    ui->skill1->setMouseTracking(true);
    ui->skill2->setMouseTracking(true);
    ui->skill3->setMouseTracking(true);
    ui->BackGround->setMouseTracking(true);
    QDialog::setMouseTracking(true);
    init_connect();
    ui->skill0->setScaledContents(false);
    ui->skill1->setScaledContents(false);
    ui->skill2->setScaledContents(false);
    ui->skill3->setScaledContents(false);
    //rebound_ski();
    //attack();
    //my_win();
}

void FightUI::rebound_ski()
{
    rebound = new QLabel;
    rebound->setGeometry(rect().x()+745, rect().y()+375,
                                    50, 100);
    rebound->setParent(this);
    QMovie *movie = new QMovie(":/Image/rebound.gif");
    rebound->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(50,100));
    movie->start();
    eneOptTimer->start(2000);
}
void FightUI::init_player(QString &myName, QString &oppName, int myLv, int oppLv)
{
    ui->BackGround->lower();
    QPixmap img2;
    img2.load(":/Image/fightroom.png");
    QPixmap pixMap2= img2.scaled(1280,720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->BackGround->setPixmap(pixMap2);
    fightEnv->set_players(Player(myName.toStdString(), myLv), Player(oppName.toStdString(), oppLv));
    QString me,opp;
    me = "Name:"+myName+"  Lv:" + QString::number(myLv);
    opp = "Name:"+oppName+"  Lv:" + QString::number(oppLv);
    ui->me->setText(me);
    ui->enemy->setText(opp);

    update_my_HP();
    update_opp_HP();
}
//void FightUI::renew_MP(vector<int> MP)
//{
//    ui->myMP->setValue(MP[0]);
//    ui->myMP->setValue(MP[1]);
//}
void FightUI::renew_my_blood(vector<int> myMonster, vector<int> myBlood,vector<int> maxBlood)
{
    QProgressBar *myptr[4] = {ui->myBlood,ui->myMonblood_1,ui->myMonblood_2,ui->myMonblood_3};
    QLabel *mylabelptr[4] = {ui->my_label,ui->my_monster1,ui->my_monster2,ui->my_monster3};
    myptr[0]->setVisible(true);
    myptr[0]->setMaximum(1);
    myptr[0]->setValue(myBlood[0]);
    for (unsigned int i = 1; i < 4; i++) {
        if(myMonster[i]>0)
            myptr[i]->setMaximum(maxBlood[i]);
    }

    for (unsigned int i = 1; i < 4; i++) {
        if(myMonster[i] >= 0 && myBlood[i] >0)
        {
            myptr[i]->setVisible(true);
            mylabelptr[i]->setVisible(true);
            myptr[i]->setValue(myBlood[i]);
        }
        else {
            myptr[i]->setVisible(false);
            mylabelptr[i]->setVisible(false);
        }
    }
}

void FightUI::renew_enemy_blood(vector<int> eneMonster, vector<int> eneBlood,vector<int> maxBlood)
{
    int monster;
    QProgressBar *eneptr[4] = {ui->eneBlood,ui->eneMonblood_1,ui->eneMonblood_2,ui->eneMonblood_3};
    QLabel *enelabelptr[4] = {ui->enemy_label,ui->enemy_monster1,ui->enemy_monster2,ui->enemy_monster3};
    eneptr[0]->setVisible(true);
    eneptr[0]->setMaximum(1);
    eneptr[0]->setValue(eneBlood[0]);
    for(auto i:eneBlood)
        qDebug() <<  "eneBlood"<<i;
    for(auto i:maxBlood)
        qDebug() <<  "maxBlood"<<i;
    for (unsigned int i = 1;i<4;i++) {
        if(eneMonster[i]>0)
            eneptr[i]->setMaximum(maxBlood[i]);
    }
    for (unsigned int i = 1; i < 4; i++) {
        if(eneMonster[i] == 100)
        {
            monster = 0;
        }
        else if (eneMonster[i] == 101)
        {
            monster = 1;
        }
        else if (eneMonster[i] == 102)
        {
            monster = 2;
        }
        else if (eneMonster[i] == 103)
        {
            monster = 3;
        }
        else {
            monster = -1;
        }

        if(eneMonster[i] >= 0 &&eneBlood[i] > 0 )
        {

            eneptr[i]->setVisible(true);
            enelabelptr[i]->setVisible(true);
            eneptr[i]->setValue(eneBlood[i]);
            //eneptr[i]->setMaximum(maxBlood[monster]);
        }
        else {
            eneptr[i]->setVisible(false);
            enelabelptr[i]->setVisible(false);
        }
    }

}
QPixmap FightUI::PixmapToRound(const QPixmap &src, int radius)
{
    if (src.isNull()) {
        return QPixmap();
    }
    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}
void FightUI::hide_summon()
{

    ui->SummonLabel->setVisible(false);
    ui->summon_monster0->setVisible(false);
    ui->summon_monster1->setVisible(false);
    ui->summon_monster2->setVisible(false);
    ui->summon_monster3->setVisible(false);
}
void FightUI::show_blood(vector<int> myMonster,vector<int> oppMonster,vector<int> myMonBlood,vector<int> eneMonBlood)
{
    QProgressBar *myptr[4] = {ui->myBlood,ui->myMonblood_1,ui->myMonblood_2,ui->myMonblood_3};
    for (unsigned int i = 0; i < 4; i++) {
        if(myMonster[i] >= 0)
        {
            myptr[i]->setVisible(true);
            myptr[i]->setValue(myMonBlood[i]);
        }
        else {
            myptr[i]->setVisible(false);
        }
    }
    QProgressBar *enemyptr[4] = {ui->eneBlood,ui->eneMonblood_1,ui->eneMonblood_2,ui->eneMonblood_3};
    for (unsigned int i = 0; i < 4; i++) {
        if(oppMonster[i] == 1)
        {
            enemyptr[i]->setVisible(true);
            enemyptr[i]->setValue(eneMonBlood[i]);
        }
        else {
            myptr[i]->setVisible(false);
        }
    }
}
void FightUI::renew_skill_photo(int pos)
{
    QLabel *skiList[4] = {ui->skill0,ui->skill1,ui->skill2,ui->skill3};
    QPixmap img;
    vector<int> myMonster;
    fightEnv->get_my_monster(myMonster);
    int monster = myMonster[pos];//得到某个位置是什么怪物
    if(monster == -1)
    {
        for (int i=0 ;i<4;i++)
        {
            int type = skillType[4][i];//得到自己的技能是攻击防御还是治疗
            img.load(skillPhotoAdd[type].c_str());
            skiList[i]->setPixmap(img);
        }
    }
    else {
        for (int i=0 ;i<4;i++)
        {
            int type = skillType[monster-100][i];//得到自己的技能是攻击防御还是治疗
            img.load(skillPhotoAdd[type].c_str());
            skiList[i]->setPixmap(img);
        }
    }

}
void FightUI::attack()
{
    QLabel *label = new QLabel(this);
    label->setVisible(true);
    QPixmap img1,img2;
    img1.load(":/Image/Fire_left.png");
    label->setPixmap(img1);
    QPropertyAnimation *animation = new QPropertyAnimation(label, "pos");
    //qDebug() << time.elapsed()/1000 << endl;
    animation->setDuration(2000);
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->setStartValue(QPoint(340,250));
    animation->setEndValue(QPoint(870,250));
    animation->start();
    //timer->start(2000);
}

void FightUI::hide_ski_anm()
{
    ui->myAttcakLabel->setVisible(false);
}
void FightUI::hide_opp_ski_anm()
{
    ui->enemyAttcakLabel->setVisible(false);
}
void FightUI::show_summon()
{
//    QPalette pa;
//    pa.setColor(QPalette::WindowText,Qt::red);
//    ui->SummonLabel->setPalette(pa);
    QFont ft("Microsoft YaHei", 10);
    ui->SummonLabel->setFont(ft);
    ui->SummonLabel->setVisible(true);
    ui->summon_monster0->setVisible(true);
    ui->summon_monster1->setVisible(true);
    ui->summon_monster2->setVisible(true);
    ui->summon_monster3->setVisible(true);
}
void FightUI::renew_enemy_monster(vector<int> oppMonster)
{
    QLabel *p;
    QMovie *movie;
    int monster;
    QLabel * enePtr[4] = {ui->enemy_label,ui->enemy_monster1,ui->enemy_monster2,ui->enemy_monster3};
    for (unsigned int i = 0;i < 4; i++) {
        if(oppMonster[i] == -1)
        {
            enePtr[i]->setVisible(false);
        }
        else {
            enePtr[i]->setVisible(true);
        }
    }
    for (unsigned int i = 1;i < 4; i++) {
        if(oppMonster[i] == 100)
        {
            monster = 0;
        }
        else if (oppMonster[i] == 101)
        {
            monster = 1;
        }
        else if (oppMonster[i] == 102)
        {
            monster = 2;
        }
        else if (oppMonster[i] == 103)
        {
            monster = 3;
        }
        else {
            monster = -1;
        }
        if(monster != -1)
        {
            movie = new QMovie(EnePhotoAdd[monster].c_str());
            if(i == 1){
                p = ui->enemy_monster1;
            }
            else if(i == 2){
                p = ui->enemy_monster2;
            }
            else if(i == 3){
                p = ui->enemy_monster3;
            }
            p->setMovie(movie);
            //设置图片的大小
            movie->setScaledSize(QSize(200,200));
            movie->start();
        }
    }

}
void FightUI::renew_my_monster(vector<int> myMonster)
{
    QLabel *p;
    QMovie *movie;
    int monster;
    for (unsigned int i = 1;i < 4; i++) {
        if(myMonster[i] == 100)
        {
            monster = 0;
        }
        else if (myMonster[i] == 101)
        {
            monster = 1;
        }
        else if (myMonster[i] == 102)
        {
            monster = 2;
        }
        else if (myMonster[i] == 103)
        {
            monster = 3;
        }
        else {
            monster = -1;
        }
        if(monster != -1)
        {
            movie = new QMovie(PhotoAdd[monster].c_str());
            if(i == 1){
                p = ui->my_monster1;
            }
            else if(i == 2){
                p = ui->my_monster2;
            }
            else if(i == 3){
                p = ui->my_monster3;
            }
            p->setMovie(movie);
            //设置图片的大小
            movie->setScaledSize(QSize(200,200));
            movie->start();
        }
    }

}
void FightUI::InitUi()
{
    setFixedSize(1280, 720);
    ui->my_label->installEventFilter(this);
    ui->my_monster1->installEventFilter(this);
    ui->my_monster2->installEventFilter(this);
    ui->my_monster3->installEventFilter(this);
    ui->enemy_monster1->installEventFilter(this);
    ui->enemy_label->installEventFilter(this);
    ui->enemy_monster1->installEventFilter(this);
    ui->enemy_monster2->installEventFilter(this);
    ui->enemy_monster3->installEventFilter(this);
    ui->skill0->installEventFilter(this);
    ui->skill1->installEventFilter(this);
    ui->skill2->installEventFilter(this);
    ui->skill3->installEventFilter(this);
    ui->summon_monster0->installEventFilter(this);
    ui->summon_monster1->installEventFilter(this);
    ui->summon_monster2->installEventFilter(this);
    ui->summon_monster3->installEventFilter(this);

    ui->my_label->setScaledContents(true);
    ui->my_monster1->setScaledContents(true);
    ui->my_monster2->setScaledContents(true);
    ui->my_monster3->setScaledContents(true);
    ui->enemy_label->setScaledContents(true);
    ui->enemy_monster1->setScaledContents(true);
    ui->enemy_monster2->setScaledContents(true);
    ui->enemy_monster3->setScaledContents(true);
    ui->myAttcakLabel->setScaledContents(true);
    ui->enemyAttcakLabel->setScaledContents(true);

    //设置血条的类型
    QString qss = "QProgressBar {border-radius: 5px;background: #FFFFFF;text-align: center;}"
            "QProgressBar::chunk {border-radius:5px;border:1px solid black;background:#e74c3c;}";
    ui->myBlood->setStyleSheet(qss);
    ui->myMonblood_1->setStyleSheet(qss);
    ui->myMonblood_2->setStyleSheet(qss);
    ui->myMonblood_3->setStyleSheet(qss);
    ui->eneBlood->setStyleSheet(qss);
    ui->eneMonblood_1->setStyleSheet(qss);
    ui->eneMonblood_2->setStyleSheet(qss);
    ui->eneMonblood_3->setStyleSheet(qss);

    qss = "QProgressBar {border-radius: 5px;background: #FFFFFF;text-align: center;}"
            "QProgressBar::chunk {border-radius:5px;border:1px solid black;background:#3498db;}";
    ui->myMP->setStyleSheet(qss);
    ui->enemyMP->setStyleSheet(qss);

    /*开局把所有东西隐藏起来*/
    ui->myMonblood_1->setVisible(false);
    ui->myMonblood_2->setVisible(false);
    ui->myMonblood_3->setVisible(false);
    ui->eneMonblood_1->setVisible(false);
    ui->eneMonblood_2->setVisible(false);
    ui->eneMonblood_3->setVisible(false);
    ui->monSelect1->setVisible(false);
    ui->myMP->setValue(0);
    ui->enemyMP->setValue(0);

    /*把百分之多少去掉*/
    ui->myBlood->setTextVisible(false);
    ui->myMonblood_1->setTextVisible(false);
    ui->myMonblood_2->setTextVisible(false);
    ui->myMonblood_3->setTextVisible(false);
    ui->eneBlood->setTextVisible(false);
    ui->eneMonblood_1->setTextVisible(false);
    ui->eneMonblood_2->setTextVisible(false);
    ui->eneMonblood_3->setTextVisible(false);
    ui->myBlood->setMaximum(1);


   //myLabel= new MyLabel(this);
    QPixmap img1;
    img1.load(":/Image/ball.png");

    //100,100为QLabel的宽高
    QPixmap pixMap= img1.scaled(100,100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //50为圆形的半径
    pixMap =  PixmapToRound(pixMap, 50);

    ui->my_label->setPixmap(pixMap);
    /*设置召唤怪物的label*/
    ui->SummonLabel->setVisible(false);
    QPixmap img2;
    img2.load(":/Image/fightroom.png");
    QPixmap pixMap2= img2.scaled(1280,720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->BackGround->setPixmap(pixMap2);


    QMovie *movie = new QMovie(":/Image/flareon_left.gif");
    ui->summon_monster0->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(131,131));
    movie->start();

    movie = new QMovie(":/Image/pikachu_left.gif");
    ui->summon_monster1->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(131,131));
    movie->start();

    movie = new QMovie(":/Image/greninja_left.gif");
    ui->summon_monster2->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(131,131));
    movie->start();

    movie = new QMovie(":/Image/dragonite_left.gif");
    ui->summon_monster3->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(131,131));
    movie->start();
    ui->summon_monster2->setVisible(0);
    ui->summon_monster2->setVisible(1);
    hide_summon();

    /*画出对方的本体*/
    img1.load(":/Image/ball.png");

    //100,100为QLabel的宽高
    pixMap= img1.scaled(100,100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //50为圆形的半径
    pixMap =  PixmapToRound(pixMap, 50);
    ui->enemy_label->setPixmap(pixMap);
    QFont ft("KaiTi", 10);
    ft.setPointSize(10);
    ui->skill0->setFont(ft);
    ui->skill2->setFont(ft);
    ui->skill3->setFont(ft);
    ui->skill1->setFont(ft);

}

void FightUI::init_connect()
{
    connect(fightEnv, SIGNAL(on_my_attack(int)), this, SLOT(my_attack(int)));
    connect(fightEnv, SIGNAL(on_my_defend()), this, SLOT(my_defend()));
    connect(fightEnv, SIGNAL(on_my_acc()), this, SLOT(my_acc()));
    connect(fightEnv, SIGNAL(on_my_summon(int, int)), this, SLOT(my_summon(int, int)));
    connect(fightEnv, SIGNAL(on_my_surrender()), this, SLOT(my_surrender()));
    connect(fightEnv, SIGNAL(on_my_win()), this, SLOT(my_win()));
    connect(fightEnv, SIGNAL(on_my_lose()), this, SLOT(my_lose()));
    connect(fightEnv, SIGNAL(on_my_monster_dead(int)), this, SLOT(my_monster_dead(int)));
    connect(fightEnv, SIGNAL(on_my_skill(int, int)), this, SLOT(my_skill(int, int)));
    connect(fightEnv, SIGNAL(on_my_rebound()), this, SLOT(my_rebound()));
    connect(fightEnv, SIGNAL(on_my_evade()), this, SLOT(my_evade()));
    connect(fightEnv, SIGNAL(on_my_heal(int)), this, SLOT(my_heal(int)));
    connect(fightEnv, SIGNAL(on_update_my_HP()), this, SLOT(update_my_HP()));
    connect(fightEnv, SIGNAL(on_update_my_MP()), this, SLOT(update_my_MP()));


    connect(fightEnv, SIGNAL(on_opp_attack(int)), this, SLOT(opp_attack(int)));
    connect(fightEnv, SIGNAL(on_opp_defend()), this, SLOT(opp_defend()));
    connect(fightEnv, SIGNAL(on_opp_acc()), this, SLOT(opp_acc()));
    connect(fightEnv, SIGNAL(on_opp_summon(int, int)), this, SLOT(opp_summon(int, int)));
    connect(fightEnv, SIGNAL(on_opp_surrender()), this, SLOT(opp_surrender()));
    connect(fightEnv, SIGNAL(on_opp_monster_dead(int)), this, SLOT(opp_monster_dead(int)));
    connect(fightEnv, SIGNAL(on_opp_skill(int,int)), this, SLOT(opp_skill(int,int)));
    connect(fightEnv, SIGNAL(on_opp_rebound()), this, SLOT(opp_rebound()));
    connect(fightEnv, SIGNAL(on_opp_evade()), this, SLOT(opp_evade()));
    connect(fightEnv, SIGNAL(on_opp_heal(int)), this, SLOT(opp_heal(int)));
    connect(fightEnv, SIGNAL(on_update_opp_HP()), this, SLOT(update_opp_HP()));
    connect(fightEnv, SIGNAL(on_update_opp_MP()), this, SLOT(update_opp_MP()));

    connect(fightEnv, SIGNAL(on_server_timeout()), this, SLOT(server_timeout()));

    connect(fightEnv, SIGNAL(on_back()), this, SLOT(back()));
    connect(fightEnv, SIGNAL(on_button_visible()), this, SLOT(button_visible()));
}
void FightUI::summon_monster(int n)
{
    QMovie *movie;
    movie = new QMovie(PhotoAdd[n].c_str());
    QLabel *p;
    if(monsterNum == 1){
        p = ui->my_monster1;
    }
    else if(monsterNum == 2){
        p = ui->my_monster2;
    }
    else if(monsterNum == 3){
        p = ui->my_monster3;
    }
    myObjectExist[monsterNum] = 1;
    p->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(200,200));
    movie->start();
    monsterNum++;
    //show_blood();

}
void FightUI::renew_skill(int seq)
{
    vector<int> myMonster;
    fightEnv->get_my_monster(myMonster);
    int monster = myMonster[seq];
    if(monster == -1)
    {
        skill[0] = skillDes[4][0];
        skill[1] = skillDes[4][1];
        skill[2] = skillDes[4][2];
        skill[3] = skillDes[4][3];
    }
    else {
        skill[0] = skillDes[monster-100][0];
        skill[1] = skillDes[monster-100][1];
        skill[2] = skillDes[monster-100][2];
        skill[3] = skillDes[monster-100][3];
    }

}
void FightUI::skill_des(int seq)
{
    renew_skill(seq);
    //skill1.Alignment = vbCenter
    //if(ObjectExist[seq])
    QFont ft("KaiTi", 15);
    //ft.setPointSize(11);
    ui->skiDes0->setFont(ft);
    ui->skiDes1->setFont(ft);
    ui->skiDes2->setFont(ft);
    ui->skiDes3->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#ff4757"));
    ui->skiDes0->setPalette(pa);
    ui->skiDes1->setPalette(pa);
    ui->skiDes2->setPalette(pa);
    ui->skiDes3->setPalette(pa);
    {
        ui->skiDes0->setText(skill[0]);
        ui->skiDes1->setText(skill[1]);
        ui->skiDes2->setText(skill[2]);
        ui->skiDes3->setText(skill[3]);
    }
}
void FightUI::submit()
{

}
void FightUI::use_skill()
{
    //attack();
    if(ObjectSelect == 0  && monsterNum <= 3)//召唤
    {
        if(SkillSelect == 3)
        {
            show_summon();
        }
        else if(SkillSelect == 2)
        {

        }
        else if(SkillSelect == 1)
        {

        }
        else if(SkillSelect == 0)
        {

        }
    }
    if(ObjectSelect != 0)
    {
        attack();
    }
    //show_blood();
}

bool FightUI::eventFilter(QObject *obj, QEvent *event)
{
    //
   /* if(time.elapsed()/1000.0 >=5)
    {
        QPixmap img2;
        ui->mymonSkill_1->setPixmap(img2);
    }*/
    if (obj == ui->my_label)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换
             if(mouseEvent->button() == Qt::LeftButton)
             {
                 if(myObjectExist[0]>0)
                 {

                     ObjectSelect = 0;
                     renew_skill_photo(0);
                     renew_skill(0);
                     skill_des(0);
                     hide_summon();
                     return true;
                 }
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->my_monster1)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
                 if(myObjectExist[1]>0)
                 {
                    ObjectSelect = 1;
                    renew_skill_photo(1);
                    renew_skill(1);
                    skill_des(1);
                    hide_summon();
                    return true;
                 }
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->my_monster2)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
                 if(myObjectExist[2] >0)
                 {
                     renew_skill_photo(2);
                     ObjectSelect = 2;
                     renew_skill(2);
                     skill_des(2);
                     hide_summon();
                     return true;
                 }
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->my_monster3)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
                 if(myObjectExist[3]>0)
                 {
                     renew_skill_photo(3);
                     ObjectSelect = 3;
                     renew_skill(3);
                     skill_des(3);
                     hide_summon();
                     return true;
                 }
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->skill3)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
                 if(ObjectSelect == 0)
                 {
                     show_summon();
                     monsterselect = -1;
                 }
                 SkillSelect = 3;
                //use_skill();
                return true;
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->skill2)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
                SkillSelect = 2;
                //use_skill();
                hide_summon();
                return true;
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->skill1)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
                SkillSelect = 1;
                //use_skill();
                hide_summon();
                return true;
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->skill0)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
                SkillSelect = 0;
                //use_skill();
                hide_summon();
                return true;
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->summon_monster0)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {

//                if(costmonster > 0)
//                {
//                    costmonster--;
//                    monsterselect = 0;
//                }
                monsterselect = 0;
                //MySummonSeq[monsterNum] = 1;
                //hide_summon();
                //summon_monster(0);
                return true;
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->summon_monster1)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
//                 if(costmonster > 0)
//                 {
//                     costmonster--;
//                     monsterselect = 1;
//                 }
//                 else {
//                     qDebug() << "无法召唤更多此怪物" << endl;
//                 }
                 monsterselect = 1;
//                MySummonSeq[monsterNum] = 1;
//                hide_summon();
//                summon_monster(1);
                return true;
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->summon_monster2)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
//                 if(costmonster > 0)
//                 {
//                     costmonster--;
//                     monsterselect = 2;
//                 }
//                 else {
//                     qDebug() << "无法召唤更多此怪物" << endl;
//                 }
//                 MySummonSeq[monsterNum] = 2;
//                 hide_summon();
//                 summon_monster(2);
                   monsterselect = 2;
                return true;
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->summon_monster3)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
//                 if(costmonster > 0)
//                 {
//                     costmonster--;
//                     monsterselect = 3;
//                 }
//                 else {
//                     qDebug() << "无法召唤更多此怪物" << endl;
//                 }
//                MySummonSeq[monsterNum] = 3;
//                hide_summon();
//                summon_monster(3);
                 monsterselect = 3;
                return true;
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->enemy_monster1)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
                eneMonSelect = 1;
                return true;
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->enemy_monster2)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
                eneMonSelect = 2;
                return true;
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->enemy_monster3)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
                eneMonSelect = 3;
                return true;
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
    if (obj == ui->enemy_label)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
                eneMonSelect = 0;
                return true;
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
}
FightUI::~FightUI()
{
    delete ui;
}
int FightUI::have_object()
{
    if(ObjectSelect == 0)
    {
        if(SkillSelect == 0 || SkillSelect == 3)
            return 1;
        else {
            return 0;
        }
    }
    else if(ObjectSelect > 0 && ObjectSelect <= 3){
        return fightEnv->has_obj(ObjectSelect - 1, SkillSelect);
    }
}


void FightUI::on_pushButton_clicked()
{

    if(ObjectSelect == -1)
    {
        //提示选择obj
        QMessageBox msg(this);
        msg.setWindowTitle("Invitation");
        msg.setText("没有选中对象");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
    else{
        if(SkillSelect == -1)
        {
            //提示选择skill
            QMessageBox msg(this);
            msg.setWindowTitle("error");
            msg.setText("请选择你的选项");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
        }
        else{
            if(have_object() == 1)
            {
                if(ObjectSelect == 0 && SkillSelect == 3 && monsterselect == -1)
                {
                    QMessageBox msg(this);
                    msg.setWindowTitle("error");
                    msg.setText("请选择你要召唤的怪兽");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.exec();
                }
                else if(eneMonSelect == -1 && ((ObjectSelect == 0 && SkillSelect == 0) || (ObjectSelect != 0)))
                {
                    //提示选择enemonster
                    QMessageBox msg(this);
                    msg.setWindowTitle("error");
                    msg.setText("请选择对方的怪兽");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.exec();
                }
                else {
                    //查看能否释放该技能
                    int third;
                    if(ObjectSelect == 0 && SkillSelect == 3)
                        third = monsterselect;
                    else {
                        third = eneMonSelect;
                    }
                    switch (fightEnv->verify_option(ObjectSelect, SkillSelect, third)) {
                        case 1:{
                            //检测通过，发送
                            fightEnv->dispatch(ObjectSelect, SkillSelect, third);
                            ui->pushButton->setVisible(false);
                            break;
                        }
                        case 2:{
                            //MP不足
                            QMessageBox msg(this);
                            msg.setWindowTitle("error");
                            msg.setText("MP不足");
                            msg.setStandardButtons(QMessageBox::Ok);
                            msg.exec();
                            break;
                        }
                        case 3:{
                            //怪兽已满
                            QMessageBox msg(this);
                            msg.setWindowTitle("error");
                            msg.setText("怪兽槽已满");
                            msg.setStandardButtons(QMessageBox::Ok);
                            msg.exec();
                            break;
                        }
                        case 4:{
                            //达到召唤上限
                            QMessageBox msg(this);
                            msg.setWindowTitle("error");
                            msg.setText("该怪兽已达到召唤上限");
                            msg.setStandardButtons(QMessageBox::Ok);
                            msg.exec();
                            break;
                        }
                        case 5:{
                            //技能释放对象选择玩家本体
                            QMessageBox msg(this);
                            msg.setWindowTitle("error");
                            msg.setText("技能不能用于玩家本体");
                            msg.setStandardButtons(QMessageBox::Ok);
                            msg.exec();
                            break;
                        }
                    }

                }
            }
            else if(have_object() == ATTACK_A){
                //无需选择obj，发送
                fightEnv->dispatch(ObjectSelect, SkillSelect, ATTACK_A);
                ui->pushButton->setVisible(false);
            }
            else{
                fightEnv->dispatch(ObjectSelect, SkillSelect, ATTACK_N);
                ui->pushButton->setVisible(false);
            }
        }
    }


}


//动画槽函数
void FightUI::my_attack(int pos)//攻击了对面pos位置
{
    ui->myAttcakLabel->setVisible(true);
    QPixmap img1,img2;
    img1.load(":/Image/Fire_left.png");
    ui->myAttcakLabel->setGeometry(rect().x()+340, rect().y()+250,
                                    140, 80);
    ui->myAttcakLabel->setPixmap(img1);
    QPropertyAnimation *animation = new QPropertyAnimation(ui->myAttcakLabel, "pos");
    animation->setDuration(800);
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->setStartValue(QPoint(340,250));
    animation->setEndValue(QPoint(870,250));
    animation->start();
    QString str;
    if(pos == 0)
    {
        str = "进攻敌方本体";
    }
    else 
    {
        str = "进攻敌方" +QString::number(pos) +"号怪物";
    }
    ui->message->setVisible(true);
    ui->message->setText(str);
    myOptTimer->start(800);

}
void FightUI::my_defend()
{
    ui->myAttcakLabel->setVisible(true);
    ui->myAttcakLabel->setGeometry(rect().x()+340, rect().y()+250,
                                    100, 200);
    //myOptLabel->setParent(this);
    QMovie *movie = new QMovie(":/Image/defend.gif");
    ui->myAttcakLabel->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(50,100));
    movie->start();
    QString str = "防御敌方进攻";
    ui->message->setVisible(true);
    ui->message->setText(str);
    myOptTimer->start(2000);

}
void FightUI::my_acc()//我方蓄气
{
    QString str = "我方蓄气";
    ui->message->setVisible(true);
    ui->message->setText(str);
    myMP += 10;
    //messageTimer->start(1000);
    vector<int> MP;
    fightEnv->get_MP(MP);
    ui->myMP->setValue(MP[0]);

}
void FightUI::my_summon(int pos, int monsterNO)//怪兽编号为monsterNO
{
    QString str = "我在位置" + QString::number(pos) + "召唤了" + NAME[monsterNO - FLAREON] ;
    ui->message->setVisible(true);
    ui->message->setText(str);
    messageTimer->start(1000);
    //myMP -= sumConsume[monsterNO];
    vector<int> myMonster,myBlood,maxBlood(4,-1);
    fightEnv->get_my_monster(myMonster);
    fightEnv->get_my_blood(myBlood);
    for (int i = 1;i<4;i++) {
        myObjectExist[i] = myMonster[i];
    }
    renew_my_monster(myMonster);
    maxBlood[0] = 1;
    for(unsigned int i = 1; i < myMonster.size(); i++)
    {
        if(myMonster[i] != -1)
            maxBlood[i] = fightEnv->mon_maxHP(myMonster[i]);
        else {
            maxBlood[i] = -1;
        }
    }
    renew_my_blood(myMonster,myBlood,maxBlood);
    hide_summon();

}
void FightUI::my_monster_dead(int pos)//pos位置上怪兽死亡
{
    QString str = "第" + QString::number(pos) +"号怪物死亡" ;
    ui->message->setVisible(true);
    ui->message->setText(str);
    messageTimer->start(1000);

    vector<int> myMonster,myBlood,maxBlood(4,-1);
    fightEnv->get_my_monster(myMonster);
    fightEnv->get_my_blood(myBlood);
    renew_my_monster(myMonster);
    maxBlood[0] = 1;
    for(unsigned int i = 1; i < myMonster.size(); i++)
    {
        if(myMonster[i] != -1)
            maxBlood[i] = fightEnv->mon_maxHP(myMonster[i]);
        else {
            maxBlood[i] = -1;
        }
    }
    renew_my_blood(myMonster,myBlood,maxBlood);
}
void FightUI::my_skill(int pos, int skiNo)//pos位置上的skiNo号技能
{
    vector<int> myMonster;
    fightEnv->get_my_monster(myMonster);
    int t = myMonster[pos] - FLAREON;
    QString str = "使用" + NAME[t] +"怪物的" + skillDes[t][skiNo] +"技能" ;
    ui->message->setVisible(true);
    ui->message->setText(str);
    messageTimer->start(1000);
    ui->myAttcakLabel->setVisible(true);
    QPixmap img1,img2;
    img1.load(":/Image/Grass_left.png");
    ui->myAttcakLabel->setGeometry(rect().x()+340, rect().y()+250,
                                    140, 80);
    ui->myAttcakLabel->setPixmap(img1);
    QPropertyAnimation *animation = new QPropertyAnimation(ui->myAttcakLabel, "pos");
    animation->setDuration(800);
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->setStartValue(QPoint(340,250));
    animation->setEndValue(QPoint(870,250));
    animation->start();
    myOptTimer->start(800);
}
void FightUI::my_surrender()
{
    QString str = "我方投降";
    ui->message->setVisible(true);
    ui->message->setText(str);
    messageTimer->start(1000);
}
void FightUI::my_win()
{
    QPixmap img2;
    img2.load(":/Image/win.png");
    QPixmap pixMap2= img2.scaled(1280,720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->BackGround->raise();
    ui->BackGround->setPixmap(pixMap2);
}

void FightUI::my_lose()
{
    QPixmap img2;
    img2.load(":/Image/lose.jpg");
    QPixmap pixMap2= img2.scaled(1280,720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->BackGround->raise();
    ui->BackGround->setPixmap(pixMap2);
}

void FightUI::my_rebound()
{
    ui->myAttcakLabel->setGeometry(rect().x()+340, rect().y()+250,
                                    100, 200);
    ui->myAttcakLabel->setParent(this);
    QMovie *movie = new QMovie(":/Image/rebound.gif");
    ui->myAttcakLabel->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(50,100));
    movie->start();
    QString str = "反弹敌方进攻";
    ui->message->setVisible(true);
    ui->message->setText(str);
    myOptTimer->start(2000);
    messageTimer->start(1000);

}

void FightUI::my_evade()
{

}

void FightUI::my_heal(int pos)
{

}

void FightUI::update_my_HP()//mode为1是+，0是-，pos为位置，value为变化的数值
{
    vector<int> myMonster,myBlood,maxBlood(4, -1);
    fightEnv->get_my_monster(myMonster);
    fightEnv->get_my_blood(myBlood);
    maxBlood[0] = 1;
    for(unsigned int i = 1; i < myMonster.size(); i++)
    {
        if(myMonster[i] != -1)
            maxBlood[i] = fightEnv->mon_maxHP(myMonster[i]);
        else {
            maxBlood[i] = -1;
        }
    }
    renew_my_blood(myMonster,myBlood,maxBlood);
}
void FightUI::update_my_MP()//value为变化的数值
{
    vector<int> MP;
    fightEnv->get_MP(MP);
    ui->myMP->setValue(MP[0]);
}

void FightUI::opp_attack(int pos)//攻击了对面pos位置
{
    ui->enemyAttcakLabel->setVisible(true);
    ui->enemyAttcakLabel->setGeometry(rect().x()+870, rect().y()+250,
                                    140, 80);
    QPixmap img1,img2;
    img1.load(":/Image/Fire.png");
    ui->enemyAttcakLabel->setPixmap(img1);
    QPropertyAnimation *animation = new QPropertyAnimation(ui->enemyAttcakLabel, "pos");
    animation->setDuration(800);
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->setStartValue(QPoint(870,250));
    animation->setEndValue(QPoint(340,250));
    animation->start();
    QString str;
    if(pos == 0)
    {
        str = "敌方进攻我方本体";
    }
    else
    {
        str = "敌方进攻我方" +QString::number(pos) +"号怪物";
    }
    ui->message_2->setVisible(true);
    ui->message_2->setText(str);
    eneOptTimer->start(800);
    messageTimer->start(1000);

//    vector<int> myMonster,eneMonster,myblood,eneblood;
//    fightEnv->get_my_monster(myMonster);
//    fightEnv->get_opp_monster(eneMonster);
//    fightEnv->get_my_blood(myblood);
//    fightEnv->get_opp_blood(eneblood);
//    renew_enemy_monster(eneMonster);
    //show_blood(myMonster,eneMonster,myblood,eneblood);
}

void FightUI::opp_defend()
{
    ui->enemyAttcakLabel->setVisible(true);
    ui->enemyAttcakLabel->setGeometry(rect().x()+870, rect().y()+250,
                                    100, 200);
    //ui->enemyAttcakLabel->setParent(this);
    QMovie *movie = new QMovie(":/Image/defend_right.gif");
    ui->enemyAttcakLabel->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(50,100));
    movie->start();
    QString str = "敌方防御我方进攻";
    ui->message_2->setVisible(true);
    ui->message_2->setText(str);
    eneOptTimer->start(1500);
    //设置图片的大小
}

void FightUI::opp_acc()
{
    QString str = "敌方蓄气";
    ui->message_2->setVisible(true);
    ui->message_2->setText(str);
    eneMP += 10;
    messageTimer->start(1000);
    vector<int> MP;
    fightEnv->get_MP(MP);
    ui->enemyMP->setValue(MP[1]);
}
void FightUI::opp_summon(int pos, int monsterNO)//怪兽编号为monsterNO
{
    QString str = "敌方在位置"+QString::number(pos)+ "召唤了" + NAME[monsterNO - FLAREON] ;
    enemonsterNum++;
    ui->message_2->setVisible(true);
    ui->message_2->setText(str);
    messageTimer->start(1000);
    //eneMP -= sumConsume[monsterNO];
    vector<int> eneMonster,eneBlood,maxBlood(4,-1);
    fightEnv->get_opp_monster(eneMonster);
    fightEnv->get_opp_blood(eneBlood);
    renew_enemy_monster(eneMonster);
    maxBlood[0] = 1;
    for(int i = 1; i < eneMonster.size(); i++)
    {
        if(eneMonster[i] != -1)
            maxBlood[i] = fightEnv->mon_maxHP(eneMonster[i]);
        else {
            maxBlood[i] = -1;
        }
    }
    renew_enemy_blood(eneMonster,eneBlood,maxBlood);
}

void FightUI::opp_monster_dead(int pos)//pos位置上怪兽死亡
{
    eneObjectExist[pos] = 0;
    EnemySummonSeq[pos] = -1;
    QString str = "敌方第" + QString::number(pos) +"号怪物死亡" ;
    ui->message_2->setVisible(true);
    ui->message_2->setText(str);
    messageTimer->start(1000);

    vector<int> eneMonster,eneblood,maxBlood(4,-1);
    fightEnv->get_opp_monster(eneMonster);
    fightEnv->get_opp_blood(eneblood);
    maxBlood[0] = 1;
    for(int i = 1; i < eneMonster.size(); i++)
    {
        if(eneMonster[i] != -1)
            maxBlood[i] = fightEnv->mon_maxHP(eneMonster[i]);
        else {
            maxBlood[i] = -1;
        }
    }
    renew_enemy_monster(eneMonster);
    renew_enemy_blood(eneMonster,eneblood,maxBlood);
}

void FightUI::opp_skill(int pos, int skiNo)//pos位置上的skiNo号技能
{
    vector<int> eneMonster;
    fightEnv->get_opp_monster(eneMonster);
    int t = eneMonster[pos] - FLAREON;
    QString str = "敌方使用" + NAME[t] +"怪物的" + skillDes[t][skiNo] +"技能" ;
    ui->message_2->setVisible(true);
    ui->message_2->setText(str);
    //messageTimer->start(1000);
    ui->enemyAttcakLabel->setVisible(true);
    ui->enemyAttcakLabel->setGeometry(rect().x()+870, rect().y()+250,
                                    140, 80);
    QPixmap img1,img2;
    img1.load(":/Image/Grass.png");
    ui->enemyAttcakLabel->setPixmap(img1);
    QPropertyAnimation *animation = new QPropertyAnimation(ui->enemyAttcakLabel, "pos");
    animation->setDuration(800);
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->setStartValue(QPoint(870,250));
    animation->setEndValue(QPoint(340,250));
    animation->start();
    eneOptTimer->start(800);

}

void FightUI::opp_surrender()
{
    QString str = "敌方投降";
    ui->message_2->setVisible(true);
    ui->message_2->setText(str);
    messageTimer->start(1000);
}

void FightUI::opp_rebound()
{
    ui->enemyAttcakLabel->setGeometry(rect().x()+870, rect().y()+250,
                                    100, 200);
    ui->enemyAttcakLabel->setParent(this);
    QMovie *movie = new QMovie(":/Image/rebound_right.gif");
    ui->enemyAttcakLabel->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(50,100));
    movie->start();
    QString str = "敌方反弹我方进攻";
    ui->message_2->setVisible(true);
    ui->message_2->setText(str);
    eneOptTimer->start(1000);
    messageTimer->start(1000);
}

void FightUI::opp_evade()
{

}

void FightUI::opp_heal(int )
{

}

void FightUI::update_opp_HP()//mode为1是+，0是-，pos为位置，value为变化的数值
{
    vector<int> eneMonster,eneBlood, maxBlood(4, -1);
    fightEnv->get_opp_monster(eneMonster);
    fightEnv->get_opp_blood(eneBlood);
    maxBlood[0] = 1;
    for(int i = 1; i < eneMonster.size(); i++)
    {
        if(eneMonster[i] != -1)
            maxBlood[i] = fightEnv->mon_maxHP(eneMonster[i]);
        else {
            maxBlood[i] = -1;
        }
    }
    renew_enemy_blood(eneMonster,eneBlood,maxBlood);


}
void FightUI::update_opp_MP()//value为变化的数值
{
    vector<int> MP;
    fightEnv->get_MP(MP);
    ui->enemyMP->setValue(MP[1]);
}

void FightUI::serv_timeout()//服务器超时
{
    QString str = "服务器超时";
    ui->message->setVisible(true);
    ui->message->setText(str);
    messageTimer->start(1000);
}
/*void FightUI::renew_massage()
{
    vector<int> myMonster,eneMonster,myblood,eneblood,MP;
    fightEnv->get_my_monster(myMonster);
    fightEnv->get_opp_monster(eneMonster);
    fightEnv->get_my_blood(myblood);
    fightEnv->get_opp_blood(eneblood);
    fightEnv->get_MP(MP);
    renew_MP(MP);
    renew_my_monster(myMonster);
    renew_enemy_monster(eneMonster);
    show_blood(myMonster,eneMonster,myblood,eneblood);

}*/

void FightUI::hide_message()
{
    //ui->message->setVisible(false);
}
void FightUI::mouseReleaseEvent(QMouseEvent * event)
{
    int skillNum = -1;
    vector<int> myMonster;
    fightEnv->get_my_monster(myMonster);
    int monster = myMonster[ObjectSelect];
    if(ObjectSelect >= 0 && ObjectSelect <=3)
        renew_skill_photo(ObjectSelect);
}
void FightUI::mousePressEvent(QMouseEvent * event)
{
    //获取点击的下标
    if(event->button() == Qt::RightButton)//右键按下
    {
        if(ObjectSelect == -1)
            return;
        int skillNum = -1;
        vector<int> myMonster;
        fightEnv->get_my_monster(myMonster);
        int monster = myMonster[ObjectSelect];
        if(monster == -1)
        {
            monster = 4;
        }
        else {
            monster -=100;
        }
        if(event->x() > 351 && event->x()<483 && event->y() > 571 && event->y() < 690)
        {
            ui->skill0->setText(skillDetDes[monster][0]);
            skillNum = 0;
        }
        else if(event->x() > 490 && event->x()< 622 && event->y() > 571 && event->y() < 690)
        {
            ui->skill1->setText(skillDetDes[monster][1]);
            skillNum = 1;
        }
        else if(event->x() > 629 && event->x()<761 && event->y() > 571 && event->y() < 690)
        {

            ui->skill2->setText(skillDetDes[monster][2]);
            skillNum = 2;
        }
        else if(event->x() > 768 && event->x()<900 && event->y() > 571 && event->y() < 690)
        {
            ui->skill3->setText(skillDetDes[monster][3]);
            skillNum = 3;
        }
        else {
            if(ObjectSelect >= 0 && ObjectSelect <=3)
                renew_skill_photo(ObjectSelect);
        }
    }
}

void FightUI::back()
{
    emit clicked(0);
}

void FightUI::button_visible()
{
    ObjectSelect = -1;
    SkillSelect = -1;
    monsterselect = -1;
    eneMonSelect =-1 ;
    ui->pushButton->setVisible(true);
}
