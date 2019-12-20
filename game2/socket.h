#ifndef SOCKET_H
#define SOCKET_H

//#define WIN32_LEAN_AND_MEAN

#include<winsock2.h>
#include<ws2tcpip.h>

#include<iostream>
#include<QString>
#include<string>
#include<list>
#include<vector>
#include<unordered_map>

#include<QtDebug>

#include<thread>
#include<mutex>


#pragma comment(lib,"Ws2_32.lib")

#define DEFAULT_BUFLEN 10000        // Socket传输缓冲区大小

#define CLIENT_PORT 4567            // 客户端连接端口
#define SERVER_PORT 4567            // 服务器监听端口

                                    // 单机模式下都指向localhost
#define CLIENT_ADDR "127.0.0.1"     // 客户端IP地址
#define SERVER_ADDR "127.0.0.1"     // 服务器IP地址

using namespace std;

class Socket{
public:
    Socket(){
        // WSAStartup初始化WS2_32.dll
        int success=WSAStartup(MAKEWORD(2, 2), &wsaData);
        if(success!=0){
            qDebug()<<"WSA start fail"<<endl;
        }
        else{
            qDebug()<<"WSA start WSAsucess"<<endl;
        }
    }
    ~Socket(){
        //释放WSA资源
        WSACleanup();
    }
private:
    //WSA数据信息
     WSADATA wsaData;
};

#endif // SOCKET_H
