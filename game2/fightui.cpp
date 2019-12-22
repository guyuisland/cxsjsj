#include "fightui.h"
#include "ui_fightui.h"
#include<QMovie>
#include<QDebug>
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
    timer = new QTimer;
    timer2 = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(hide_ski_anm()));
    connect(timer2, SIGNAL(timeout()), this, SLOT(hide_ski_anm()));
    fightEnv = new Fightenv(_client);
    rebound_ski();
    renew_enemy_monster();
}
void FightUI::rebound_ski()
{
    rebound = new QLabel;
    rebound->setGeometry(rect().x()+745, rect().y()+375,
                                    100, 50);
    rebound->setParent(this);
    QMovie *movie = new QMovie(":/Image/rebound.gif");
    rebound->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(250,250));
    movie->start();
    timer2->start(2000);
}
void FightUI::init_player(QString &myName, QString &oppName, int myLv, int oppLv)
{
    fightEnv->set_players(Player(myName.toStdString(), myLv), Player(oppName.toStdString(), oppLv));
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
void FightUI::show_blood()
{
    QProgressBar *myptr[4] = {ui->myBlood,ui->myMonblood_1,ui->myMonblood_2,ui->myMonblood_3};
    for (int i = 0; i < 4; i++) {
        if(myObjectExist[i] == 1)
        {
            myptr[i]->setVisible(true);
            myptr[i]->setValue(myMonBlood[i]);
        }
    }
    QProgressBar *enemyptr[4] = {ui->eneBlood,ui->eneMonblood_1,ui->eneMonblood_2,ui->eneMonblood_3};
    for (int i = 0; i < 4; i++) {
        if(eneObjectExist[i] == 1)
        {
            enemyptr[i]->setVisible(true);
            enemyptr[i]->setValue(eneMonBlood[i]);
        }
    }
}
void FightUI::attack()
{
    qDebug() << "attack" << ui->mymonSkill_1->y()<<  endl;
    ui->mymonSkill_1->setVisible(true);
    QPixmap img1,img2;
    img1.load(":/Image/Fire_left.png");
    ui->mymonSkill_1->setPixmap(img1);
    QPropertyAnimation *animation = new QPropertyAnimation(ui->mymonSkill_1, "pos");



    //qDebug() << time.elapsed()/1000 << endl;
    animation->setDuration(2000);
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->setStartValue(QPoint(340,250));
    animation->setEndValue(QPoint(870,250));
    animation->start();
    timer->start(2000);



}

void FightUI::hide_ski_anm()
{
    ui->mymonSkill_1->setVisible(false);
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
void FightUI::renew_enemy_monster()
{
    QPixmap img1;
    img1.load(":/Image/ball.png");
    QMovie *movie;
    QLabel *p;
    //100,100为QLabel的宽高
    QPixmap pixMap= img1.scaled(100,100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //50为圆形的半径
    pixMap =  PixmapToRound(pixMap, 50);
    ui->enemy_label->setPixmap(pixMap);

    for (int i = 1;i < 4; i++) {
        if(EnemySummonSeq[i] != -1)
        {
            movie = new QMovie(EnePhotoAdd[EnemySummonSeq[i]].c_str());
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
void FightUI::InitUi()
{
    setFixedSize(1280, 720);
    ui->my_label->installEventFilter(this);
    ui->my_monster1->installEventFilter(this);
    ui->my_monster2->installEventFilter(this);
    ui->my_monster3->installEventFilter(this);
    ui->enemy_monster1->installEventFilter(this);
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
    /*ui->myBlood->setTextVisible(false);
    ui->myMonblood_1->setTextVisible(false);
    ui->myMonblood_2->setTextVisible(false);
    ui->myMonblood_3->setTextVisible(false);
    ui->eneBlood->setTextVisible(false);
    ui->eneMonblood_1->setTextVisible(false);
    ui->eneMonblood_2->setTextVisible(false);
    ui->eneMonblood_3->setTextVisible(false);*/


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


    QMovie *movie = new QMovie(":/Image/charizard_left.gif");
    ui->summon_monster0->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(131,131));
    movie->start();

    movie = new QMovie(":/Image/dodrio_left.gif");
    ui->summon_monster1->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(131,131));
    movie->start();

    movie = new QMovie(":/Image/lapras_left.gif");
    ui->summon_monster2->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(131,131));
    movie->start();

    movie = new QMovie(":/Image/talonflame_left.gif");
    ui->summon_monster3->setMovie(movie);
    //设置图片的大小
    movie->setScaledSize(QSize(131,131));
    movie->start();
    ui->summon_monster2->setVisible(0);
    ui->summon_monster2->setVisible(1);
    hide_summon();

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
    show_blood();

}
void FightUI::renew_skill(int seq)
{
    if(seq == 0)
    {
        skill[0] = "防御";
        skill[1] = "攻击";
        skill[2] = "蓄积能量";
        skill[3] = "召唤";
    }
    if(seq == 1)
    {
        skill[0] = "白居易";
        skill[1] = "李白";
        skill[2] = "杜甫";
        skill[3] = "孙膑";
    }
    if(seq == 2)
    {
        skill[0] = "A";
        skill[1] = "B";
        skill[2] = "C";
        skill[3] = "D";
    }
}
void FightUI::skill_des(int seq)
{
    renew_skill(seq);
    //skill1.Alignment = vbCenter
    //if(ObjectExist[seq])
    {
        ui->skill0->setText(skill[0]);
        ui->skill1->setText(skill[1]);
        ui->skill2->setText(skill[2]);
        ui->skill3->setText(skill[3]);
    }
}
void FightUI::submit()
{

}
void FightUI::use_skill()
{
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
    show_blood();
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
                 if(myObjectExist[0])
                 {
                     ObjectSelect = 0;
                     skill_des(0);
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
                 if(myObjectExist[1])
                 {
                    ObjectSelect = 1;
                    skill_des(1);
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
                 if(myObjectExist[2])
                 {
                     ObjectSelect = 2;
                     skill_des(2);
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
                 if(myObjectExist[3])
                 {
                     ObjectSelect = 3;
                     skill_des(3);
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

                if(costmonster > 0)
                {
                    costmonster--;
                    monsterselect = 0;
                }
                else {
                    qDebug() << "无法召唤更多此怪物" << endl;
                }
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
                 if(costmonster > 0)
                 {
                     costmonster--;
                     monsterselect = 1;
                 }
                 else {
                     qDebug() << "无法召唤更多此怪物" << endl;
                 }
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
                 if(costmonster > 0)
                 {
                     costmonster--;
                     monsterselect = 2;
                 }
                 else {
                     qDebug() << "无法召唤更多此怪物" << endl;
                 }
//                 MySummonSeq[monsterNum] = 2;
//                 hide_summon();
//                 summon_monster(2);
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
                 if(costmonster > 0)
                 {
                     costmonster--;
                     monsterselect = 3;
                 }
                 else {
                     qDebug() << "无法召唤更多此怪物" << endl;
                 }
//                MySummonSeq[monsterNum] = 3;
//                hide_summon();
//                summon_monster(3);
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
        return fightEnv->has_obj(ObjectSelect, SkillSelect);
    }
}


void FightUI::on_pushButton_clicked()
{
    if(ObjectSelect == -1)
    {
        //提示选择obj
    }
    else{
        if(SkillSelect == -1)
        {
            //提示选择skill
        }
        else{
            if(have_object() == 1)
            {
                if(eneMonSelect == -1)
                {
                    //提示选择enemonster
                    //当skill为召唤时，enemonster为monster的编号
                }
                else {
                    //检测通过，发送
                    fightEnv->dispatch(ObjectSelect, SkillSelect, eneMonSelect);
                }
            }
            else if(have_object() == ATTACK_A){
                //无需选择obj，发送
                fightEnv->dispatch(ObjectSelect, SkillSelect, ATTACK_A);
            }
            else{
                fightEnv->dispatch(ObjectSelect, SkillSelect, ATTACK_N);
            }
        }
    }


}


//动画槽函数
void FightUI::my_attack(int pos)//攻击了对面pos位置
{

}
void FightUI::my_defend()
{

}
void FightUI::my_acc()//我方蓄气
{

}
void FightUI::my_summon(int monsterNO)//怪兽编号为monsterNO
{

}
void FightUI::my_monster_dead(int pos)//pos位置上怪兽死亡
{

}
void FightUI::my_skill(int pos, int skiNo)//pos位置上的skiNo号技能
{

}
void FightUI::my_surrender()
{

}
void FightUI::my_win()
{

}

void FightUI::my_lose()
{

}
void FightUI::update_my_HP(int mode, int pos, int value)//mode为1是+，0是-，pos为位置，value为变化的数值
{

}
void FightUI::update_my_MP(int value)//value为变化的数值
{

}

void FightUI::opp_attack(int pos)//攻击了对面pos位置
{

}

void FightUI::opp_defend()
{

}

void FightUI::opp_acc()
{

}
void FightUI::opp_summon(int monsterNO)//怪兽编号为monsterNO
{

}

void FightUI::opp_monster_dead(int pos)//pos位置上怪兽死亡
{

}

void FightUI::opp_skill(int pos, int skiNo)//pos位置上的skiNo号技能
{

}

void FightUI::opp_surrender()
{

}
void FightUI::update_opp_HP(int mode, int pos, int value)//mode为1是+，0是-，pos为位置，value为变化的数值
{

}
void FightUI::update_opp_MP(int value)//value为变化的数值
{

}

void FightUI::serv_timeout()//服务器超时
{

}
