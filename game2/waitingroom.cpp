#include "waitingroom.h"
#include "ui_waitingroom.h"

WaitingRoom::WaitingRoom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitingRoom)
{
    ui->setupUi(this);
    setFixedSize(1280, 720);
}

WaitingRoom::~WaitingRoom()
{
    delete ui;
}
