#include "waitingroom.h"
#include "ui_waitingroom.h"

WaitingRoom::WaitingRoom(ClientSocket *client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitingRoom),
    _client(client)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout;

    QMediaPlayer* player = new QMediaPlayer;
    QVideoWidget* videoWidget = new QVideoWidget;
    QMediaPlaylist* playlist=new QMediaPlaylist;

    videoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
    videoWidget->setFullScreen(true);

    layout->addWidget(videoWidget);
    ui->widget->setLayout(layout);

    playlist->clear();
    playlist->addMedia(QUrl::fromLocalFile("H:\\Users\\Myth\\source\\repos\\cxsjsj\\game2\\adv.avi"));
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    player->setPlaylist(playlist);


    player->setVideoOutput(videoWidget);
    player->setPlaylist(playlist);

    ui->widget->showFullScreen();
    player->play();

}

WaitingRoom::~WaitingRoom()
{
    player->stop();
    delete ui;
}


