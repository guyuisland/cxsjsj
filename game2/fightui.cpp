#include "fightui.h"
#include "ui_fightui.h"
#include<QMovie>
#include<QDebug>
#include<QPainter>

FightUI::FightUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FightUI)
{
    ui->setupUi(this);
    InitUi();
    ui->monster1->installEventFilter(this);
    myLabel= new MyLabel(this);
    //myLabel->setStyleSheet("background-color:rgb(100,150,150)"); //设置背景颜色
    //myLabel->setGeometry(QRect(100,100,300,80)); //设置位置和大小
    myLabel->show();  //显示控件


}

void FightUI::InitUi()
{
    setFixedSize(1280, 720);
    //导入
    bool adc = 1;
    if(adc)
    {
        QMovie *movie = new QMovie(":/Image/adc.gif");
        ui->monster1->setMovie(movie);
        //设置图片的大小
        movie->setScaledSize(QSize(201,121));
        movie->start();
    }
    /*label中画图*/
    /*QPixmap pixmap(":/Image/adc.gif");
    ui->monster1->setPixmap(pixmap);
    ui->monster1->show();
*/

}
bool FightUI::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->monster1)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

             if(mouseEvent->button() == Qt::LeftButton)
             {
                    qDebug() << "123";
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
     else
     {
         // pass the event on to the parent class
         return QWidget::eventFilter(obj, event);
     }
}
FightUI::~FightUI()
{
    delete ui;
}

