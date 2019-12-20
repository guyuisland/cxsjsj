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
    connect(timer, SIGNAL(timeout()), this, SLOT(hide_ski_anm()));
    //myLabel->setStyleSheet("background-color:rgb(100,150,150)"); //设置背景颜色
    //myLabel->setGeometry(QRect(100,100,300,80)); //设置位置和大小
   // myLabel->show();  //显示控件
    renew_enemy_monster();
//    QMovie *movie = new QMovie(":/Image/charizard_left.gif");
//    ui->test->setMovie(movie);
//    //设置图片的大小
//    ui->test->setScaledContents(true);
//    //movie->setScaledSize(QSize(100,100));
//    movie->start();
//    //ui->test->resize(200,200);
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

    /*开局把所有东西隐藏起来*/
    ui->myMonblood_1->setVisible(false);
    ui->myMonblood_2->setVisible(false);
    ui->myMonblood_3->setVisible(false);
    ui->eneMonblood_1->setVisible(false);
    ui->eneMonblood_2->setVisible(false);
    ui->eneMonblood_3->setVisible(false);
    ui->monSelect1->setVisible(false);

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
//    QPixmap img3;
//    img3.load(":/Image/gamelobby.png");
//    ui->monSelect1->setPixmap(img3);
    /*label中画图*/
    /*QPixmap pixmap(":/Image/adc.gif");
    ui->monster1->setPixmap(pixmap);
    ui->monster1->show();
*/

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
void FightUI::use_skill()
{
    if(ObjectSelect == 0 && SkillSelect == 3 && monsterNum <= 3)//召唤
    {
        show_summon();
        show_blood();
    }
    if(ObjectSelect != 0)
    {
        attack();
    }
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
                SkillSelect = 3;
                use_skill();
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
                use_skill();
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
                use_skill();
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
                use_skill();
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
                MySummonSeq[monsterNum] = 0;
                hide_summon();
                summon_monster(0);
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
                MySummonSeq[monsterNum] = 1;
                hide_summon();
                summon_monster(1);
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
                 MySummonSeq[monsterNum] = 2;
                 hide_summon();
                 summon_monster(2);
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
                MySummonSeq[monsterNum] = 3;
                hide_summon();
                summon_monster(3);
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

