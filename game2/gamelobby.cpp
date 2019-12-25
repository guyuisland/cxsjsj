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


GameLobby::GameLobby(ClientSocket *client, json & recvInfo, FightUI *fightui, QWidget *parent) :
    QDialog(parent),
    _fight(fightui),
    _client(client),
    ui(new Ui::GameLobby)
{
    ui->setupUi(this);
    //设置图标
    //setWindowIcon(QIcon(":/Image/ball.png"));
    //设置标题
    setWindowTitle("游戏大厅");
    SetUserList();
    InitUi();
    QFont ft("Microsoft YaHei", 10);
    //ft.setPointSize(11);
    ui->name->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#ff4757"));
    ui->name->setPalette(pa);

    //刷新用户名、waiting列表
    myName = QString::fromStdString(recvInfo["myName"].get<string>());
    myLv = recvInfo["myLv"].get<int>();
    QString str = "Username: " + myName;
    ui->level->setText("Lv:" + QString(myLv));

    wList = recvInfo["wList"].get<std::vector<std::string>>();
    int size = wList.size(), i = 0;
    if(i < size)
    {ui->User1->setText(QString::fromStdString(wList[i++])); userExist[0] = 1;}
    else
        ui->User1->clear();
    if(i < size)
    {ui->User2->setText(QString::fromStdString(wList[i++])); userExist[1] = 1;}
    else
        ui->User2->clear();
    if(i < size)
    {ui->User3->setText(QString::fromStdString(wList[i++])); userExist[2] = 1;}
    else
        ui->User3->clear();
    if(i < size)
    {ui->User4->setText(QString::fromStdString(wList[i++])); userExist[3] = 1;}
    else
        ui->User4->clear();

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
        if(userExist[0] == 1)
        {
            send_invitation(0);
        }
    }
    else if (isPointInPolygon(User2, std::move(QPointF(x, y))))
    {
        if(userExist[1] == 1)
        {
            send_invitation(1);
        }
    }
    else if (isPointInPolygon(User3, std::move(QPointF(x, y))))
    {
        if(userExist[2] == 1)
        {
            send_invitation(2);
        }
    }
    else if (isPointInPolygon(User4, std::move(QPointF(x, y))))
    {
        if(userExist[3] == 1)
        {
            send_invitation(3);
        }
    }
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

void GameLobby::on_waitButton_clicked()
{
    json sendInfo;
    sendInfo["define"] = READY;
    sendInfo["myName"] = myName.toStdString();
    emit clicked(3);

    std::string (ClientSocket::*p)(std::string) = &ClientSocket::Send_Recv;
    strWatcherPtr = new QFutureWatcher<std::string>;
    QFuture<std::string> ret = QtConcurrent::run(_client, p, sendInfo.dump());
    connect(strWatcherPtr, SIGNAL(finished()), this, SLOT(invite_handle()));
    strWatcherPtr->setFuture(ret);

}

void GameLobby::invite_handle()
{
    json recvInfo = json::parse(strWatcherPtr->result());
    disconnect(strWatcherPtr, SIGNAL(finished()), this, SLOT(invite_handle()));
    delete strWatcherPtr;
    if(recvInfo["define"].get<int>() == INVITE)
    {
        //qDebug() <<"here3";
        QString oppName = QString::fromStdString(recvInfo["myName"].get<std::string>());
        QMessageBox msg(this);
        msg.setWindowTitle("Invitation");
        msg.setText("Player " + oppName + " is inviting you to play!");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        json sendInfo;
        sendInfo["define"] = REPLY;
        sendInfo["myName"] = myName.toStdString();
        sendInfo["opponent"] = recvInfo["myName"];
        if(msg.exec() == QMessageBox::Yes)
        {
            sendInfo["option"] = ACCEPT;
            _fight->init_player(myName, invitedName, myLv,1);
            emit clicked(1);

        }
        else if(msg.exec() == QMessageBox::No){
            emit clicked(0);
            sendInfo["option"] = REFUSE;
        }
        std::string (ClientSocket::*p)(std::string) = &ClientSocket::Send_Recv;
        strWatcherPtr = new QFutureWatcher<std::string>;
        QFuture<std::string> ret = QtConcurrent::run(_client, p, sendInfo.dump());
        connect(strWatcherPtr, SIGNAL(finished()), this, SLOT(send_handle()));
        strWatcherPtr->setFuture(ret);
    }

}

void GameLobby::send_handle()
{
    disconnect(strWatcherPtr, SIGNAL(finished()), this, SLOT(send_handle()));
    delete strWatcherPtr;
}


void GameLobby::on_updateButton_clicked()
{
    json sendInfo;
    sendInfo["define"] = GET_WAITING_LIST;

    std::string (ClientSocket::*p)(std::string) = &ClientSocket::Send_Recv;
    strWatcherPtr = new QFutureWatcher<std::string>;
    QFuture<std::string> ret = QtConcurrent::run(_client, p, sendInfo.dump());
    connect(strWatcherPtr, SIGNAL(finished()), this, SLOT(update_handle()));
    strWatcherPtr->setFuture(ret);


}

void GameLobby::update_handle()
{
    json recvInfo = json::parse(strWatcherPtr->result());
    disconnect(strWatcherPtr, SIGNAL(finished()), this, SLOT(update_handle()));
    delete strWatcherPtr;
    wList = recvInfo["w_list"].get<std::vector<std::string>>();
    int size = wList.size(), i = 0;
    if(i < size)
    {ui->User1->setText(QString::fromStdString(wList[i++])); userExist[0] = 1;}
    else
    {ui->User1->clear(); userExist[0] = 0;}
    if(i < size)
    {ui->User2->setText(QString::fromStdString(wList[i++])); userExist[1] = 1;}
    else
    {ui->User2->clear(); userExist[1] = 0;}
    if(i < size)
    {ui->User3->setText(QString::fromStdString(wList[i++])); userExist[2] = 1;}
    else
    {ui->User3->clear(); userExist[2] = 0;}
    if(i < size)
    {ui->User4->setText(QString::fromStdString(wList[i++])); userExist[3] = 1;}
    else
    {ui->User4->clear(); userExist[3] = 0;}
}

void GameLobby::send_invitation(int index)
{
    json sendInfo;
    sendInfo["define"] = INVITE;
    sendInfo["myName"] = myName.toStdString();
    sendInfo["opponent"] = wList[index];
    this->invitedName = QString::fromStdString(wList[index]);
    std::string (ClientSocket::*p)(std::string) = &ClientSocket::Send_Recv;
    strWatcherPtr = new QFutureWatcher<std::string>;
    QFuture<std::string> ret = QtConcurrent::run(_client, p, sendInfo.dump());
    connect(strWatcherPtr, SIGNAL(finished()), this, SLOT(reply_handle()));
    strWatcherPtr->setFuture(ret);

}


void GameLobby::reply_handle()
{
    json recvInfo = json::parse(strWatcherPtr->result());
    disconnect(strWatcherPtr, SIGNAL(finished()), this, SLOT(reply_handle()));
    delete strWatcherPtr;
    if(recvInfo["define"] == REPLY)
    {
        if(recvInfo["option"] == ACCEPT)
        {
            QMessageBox msg(this);
            msg.setWindowTitle("Reply");
            msg.setText("Player " + invitedName + " accepted your invitation!");
            msg.setStandardButtons(QMessageBox::Ok);
            if(msg.exec() == QMessageBox::Ok)
            {
                _fight->init_player(myName, invitedName, myLv,1);
                emit clicked(1);

            }
        }
        else if(recvInfo["option"] == REFUSE)
        {
            QMessageBox msg(this);
            msg.setWindowTitle("Reply");
            msg.setText("Player " + invitedName + " refused your invitation!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
        }
    }
}
