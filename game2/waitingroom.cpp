#include "waitingroom.h"
#include "ui_waitingroom.h"

WaitingRoom::WaitingRoom(ClientSocket *client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitingRoom),
    _client(client)
{
    ui->setupUi(this);
    setFixedSize(1280, 720);
}

WaitingRoom::~WaitingRoom()
{
    delete ui;
}


