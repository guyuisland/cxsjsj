#include "stackwidget.h"
#include "ui_stackwidget.h"
#include <QGraphicsDropShadowEffect>

StackWidget::StackWidget(ClientSocket *client, json& recvInfo, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StackWidget),
    _client(client),
    _gameLobby(new GameLobby(client, recvInfo)),
    _fightRoom(new FightUI(client)),
    _rankingRoom(new RankingRoom(client)),
    _waitingRoom(new WaitingRoom(client))
{
    ui->setupUi(this);
    InitUi();

    InitConnect();
}

void StackWidget::InitUi()
{
    //setFixedSize(1280, 720);
    //setWindowFlags(Qt::FramelessWindowHint);

    // 加载stacklayout的各个分视图
    _stackLayout = new QStackedLayout(this);

    _stackLayout->addWidget(_gameLobby);
    _stackLayout->addWidget(_fightRoom);
    _stackLayout->addWidget(_rankingRoom);
    _stackLayout->addWidget(_waitingRoom);
//    _stackLayout->addWidget(_userListWidget);
//    _stackLayout->addWidget(_scratchWidget);
//    _stackLayout->addWidget(_bagWidget);
//    _stackLayout->addWidget(_fightWidget);
    _stackLayout->setCurrentIndex(0);
}

void StackWidget::InitConnect()
{
    // 连接各个分视图的信号
    connect(_gameLobby, SIGNAL(clicked(int)), this, SLOT(SetCurrentIndex(int)));

    connect(_fightRoom, SIGNAL(clicked(int)), this, SLOT(SetCurrentIndex(int)));
    //connect(_rankingRoom, SIGNAL(clicked(int)), this, SLOT(SetCurrentIndex(int)));
    connect(_rankingRoom, SIGNAL(back()), this, SLOT(backToLobby()));
//    connect(_userListWidget, SIGNAL(back()), this, SLOT(BackToLobby()));
//    connect(_bagWidget, SIGNAL(back()), this, SLOT(BackToLobby()));
//    connect(_fightWidget, SIGNAL(back()), this, SLOT(BackToLobby()));
//    connect(_scratchWidget, SIGNAL(back()), this, SLOT(BackToLobby()));

    // 点击的时候刷新相应界面内容
//    connect(this, SIGNAL(refreshUserList()), _userListWidget, SLOT(SetUserList()));
//    connect(this, SIGNAL(refreshBag()), _bagWidget, SLOT(SetBag()));
//    connect(this, SIGNAL(refreshFight()), _fightWidget, SLOT(SetBag()));

    // 关闭所有视图
    connect(_gameLobby, SIGNAL(closeAll()), this, SLOT(close()));
}

StackWidget::~StackWidget()
{
    delete ui;
    delete _gameLobby;
    delete _fightRoom;
    delete _waitingRoom;
    delete _rankingRoom;
//    delete _fightWidget;
//    delete _scratchWidget;
    delete _client;
}

void StackWidget::SetCurrentIndex(int index)
{
    // 根据index，切换各个视图
//    if (index == 1)
//        emit refreshUserList();
//    else if (index == 3)
//        emit refreshBag();
//    else if (index == 4)
//        emit refreshFight();
    if(index == 0)
        _gameLobby->on_updateButton_clicked();
    if(index == 2)
        _rankingRoom->on_update_ranking();
    _stackLayout->setCurrentIndex(index);

}

void StackWidget::backToLobby()
{
    // 设置当前视图为主界面
    _stackLayout->setCurrentIndex(0);
}
