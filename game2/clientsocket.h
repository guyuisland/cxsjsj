#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include"socket.h"
#include"protocol.h"
#include"json.hpp"
#include<QDebug>

using json=nlohmann::json;


class ClientSocket:public Socket
{
public:
    ClientSocket(string name="unknow");
    ~ClientSocket();
    int Connect();
    string Send_Recv(string sendInfo);
    void single_send(string sendInfo);
    string single_recv();
    string get_user_name();
private:
    //当前与服务器通信的socket
    SOCKET _connectSocket;
    //服务器地址
    SOCKADDR_IN _serverAddr;
    //用户名
    string userName;
};

#endif // CLIENTSOCKET_H
