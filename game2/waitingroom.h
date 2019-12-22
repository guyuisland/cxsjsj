#ifndef WAITINGROOM_H
#define WAITINGROOM_H

#include <QWidget>
#include "clientsocket.h"
#include <QMainWindow>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QMediaPlaylist>
#include<QBoxLayout>

namespace Ui {
class WaitingRoom;
}

class WaitingRoom : public QWidget
{
    Q_OBJECT

public:
    explicit WaitingRoom(ClientSocket *, QWidget *parent = nullptr);
    ~WaitingRoom();

    void addToPlaylist(const QStringList& fileNames);
private:
    ClientSocket *_client;
    Ui::WaitingRoom *ui;
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QMediaPlaylist *Playlist;
    QString m_fileName;
    QMediaPlayer::State  m_playerState;


};

#endif // WAITINGROOM_H
