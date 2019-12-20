#include "gamelobby.h"
#include "ui_gamelobby.h"
#include <QLabel>

#include <QMovie>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QMessageBox>
#include <QLabel>
#include <QStackedLayout>
#include <QHBoxLayout>
#include<QMouseEvent>
#include <QDebug>

GameLobby::GameLobby(ClientSocket *client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameLobby)
{
    ui->setupUi(this);
    //设置图标
    //setWindowIcon(QIcon(":/Image/ball.png"));
    //设置标题
    setWindowTitle("游戏大厅");
    SetUserList();
    InitUi();

}
void GameLobby::InitUi()
{
    this->setFixedSize(1280, 720);
    //导入

}
void GameLobby::SetUserList()
{
    //设置背景图片
//    QPalette pal = this->palette();
//    pal.setBrush(QPalette::Background,QBrush(QPixmap("H:/QT/game2/Image/user.png")));
//    setPalette(pal);
    QPixmap img1;
    img1.load(":/Image/user.png");
    QPixmap pixMap= img1.scaled(1280,720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->background->setPixmap(pixMap);

    //设置字体字号
    QFont ft("Microsoft YaHei", 20);
    //ft.setPointSize(11);
    ui->User1->setFont(ft);
    ui->User2->setFont(ft);
    //设置颜色
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#ff4757"));
    ui->User1->setPalette(pa);

    QString str = QString::fromWCharArray(L"  USERNAME");
    ui->User1->setText(str);

    pa.setColor(QPalette::WindowText,QColor("#57606f"));
    ui->User2->setPalette(pa);
    QString str2 = QString::fromWCharArray(L"  USERNAME");
    ui->User2->setText(str);


}
void GameLobby::OpenFightWin()
{
//    this->hide();//主界面关闭
//    FightUIWin = new FightUI(_client);//新建子界面
//    //connect(FightUIWin,&QPushButton::released,this,GameLobby::reshow());  //【抬起】按钮b2时，修改按钮b2的标题//当点击子界面时，调用主界面的reshow()函数
//    FightUIWin->show();
}
void GameLobby::mousePressEvent(QMouseEvent *event)
{
    // 鼠标坐标是否在多边形区域中
    auto isPointInPolygon = []( QVector<QPointF> points, QPointF point )
    {
        QPolygonF polygon(points);
        return polygon.containsPoint(point, Qt::OddEvenFill);
    };
    int area_x = 0;
    int area_y = 0;
    static const QVector<QPointF> User1 = { QPointF(area_x + ui->User1->x(),area_y + ui->User1->y()),
                                                        QPointF(area_x + ui->User1->x(),area_y + ui->User1->y() + ui->User1->height()),
                                                        QPointF(area_x + ui->User1->x()+ui->User1->width(), area_y + ui->User1->y() + ui->User1->height()),
                                                        QPointF(area_x + ui->User1->x()+ui->User1->width(), area_y + ui->User1->y()) };
    static const QVector<QPointF> User2 = { QPointF(area_x + ui->User2->x(),area_y + ui->User2->y()),
                                                        QPointF(area_x + ui->User2->x(),area_y + ui->User2->y() + ui->User2->height()),
                                                        QPointF(area_x + ui->User2->x()+ui->User2->width(), area_y + ui->User2->y() + ui->User2->height()),
                                                        QPointF(area_x + ui->User2->x()+ui->User2->width(), area_y + ui->User2->y()) };
    static const QVector<QPointF> User3 = { QPointF(area_x + ui->User3->x(),area_y + ui->User3->y()),
                                                        QPointF(area_x + ui->User3->x(),area_y + ui->User3->y() + ui->User3->height()),
                                                        QPointF(area_x + ui->User3->x()+ui->User3->width(), area_y + ui->User3->y() + ui->User3->height()),
                                                        QPointF(area_x + ui->User3->x()+ui->User3->width(), area_y + ui->User3->y()) };
    static const QVector<QPointF> User4 = { QPointF(area_x + ui->User4->x(),area_y + ui->User4->y()),
                                                        QPointF(area_x + ui->User4->x(),area_y + ui->User4->y() + ui->User4->height()),
                                                        QPointF(area_x + ui->User4->x()+ui->User4->width(), area_y + ui->User4->y() + ui->User4->height()),
                                                        QPointF(area_x + ui->User4->x()+ui->User4->width(), area_y + ui->User4->y()) };
    auto x = event->x();
    auto y = event->y();

    // 判断鼠标所在区域，产生响应信号
    if (isPointInPolygon(User1, std::move(QPointF(x, y))))
    {
        if(1)//对方接受你的请求,连接到同一个房间
        {
            //OpenFightWin();
            emit clicked(1);
        }
        else {//弹窗提示拒接

        }
    }
    else if (isPointInPolygon(User2, std::move(QPointF(x, y))))
        qDebug() << "222";
    else if (isPointInPolygon(User3, std::move(QPointF(x, y))))
        qDebug() << "3";
    else if (isPointInPolygon(User4, std::move(QPointF(x, y))))
        qDebug() << "4";
}
void GameLobby::reshow(){
    qDebug() << "reshow";
    this->show();
}
GameLobby::~GameLobby()
{
    delete ui;
}

void GameLobby::on_rankButton_clicked()
{
    emit clicked(2);
}
