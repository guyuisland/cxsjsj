#ifndef WAITINGROOM_H
#define WAITINGROOM_H

#include <QWidget>
#include "clientsocket.h"

namespace Ui {
class WaitingRoom;
}

class WaitingRoom : public QWidget
{
    Q_OBJECT

public:
    explicit WaitingRoom(ClientSocket *, QWidget *parent = nullptr);
    ~WaitingRoom();

private:

    Ui::WaitingRoom *ui;
    ClientSocket *_client;//与服务器连接的socket
};

#endif // WAITINGROOM_H
