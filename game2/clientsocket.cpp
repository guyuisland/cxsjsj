#include "clientsocket.h"

ClientSocket::ClientSocket(string name):userName(name)
{
    qDebug()<<"初始化成功\n";
    _connectSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(_connectSocket==INVALID_SOCKET){
        qDebug()<<"创建socket失败\n";
    }
    else{
        qDebug()<<"创建socket成功\n";
    }
    _serverAddr.sin_family=AF_INET;
    _serverAddr.sin_port=htons(SERVER_PORT);
    _serverAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
    //设置时限为1000
    int Timeout=1000;
    setsockopt(_connectSocket,SOL_SOCKET,SO_SNDTIMEO,(char*)&Timeout,sizeof (int));
    qDebug()<<"初始化成功\n";
}

ClientSocket::~ClientSocket(){
    shutdown(_connectSocket,SD_BOTH);
    closesocket(_connectSocket);
    qDebug() << "Clientsocket closed.\n";
}

int ClientSocket::Connect(){
    int trys=3;
    bool success=false;
    for (;trys!=0 && success==false;trys--) {
        if(connect(_connectSocket,reinterpret_cast<SOCKADDR *>(&_serverAddr),sizeof (_serverAddr))==-1){
            qDebug()<<"连接失败!10s后开始"<<"第"<< 4-trys<<"次尝试\n";
            Sleep(1000);
        }
        else{
             qDebug()<<"连接成功!\n";
             success=true;
             return 1;
        }
    }
    if(!success){
        closesocket(_connectSocket);
        qDebug()<<"无法连接至服务端\n";
        return 0;
    }


}
string ClientSocket::Send(string sendInfo){
    char sendBuf[DEFAULT_BUFLEN];
    strcpy(sendBuf, sendInfo.c_str());
    if(send(_connectSocket,sendBuf,strlen(sendBuf)+1,0)==SOCKET_ERROR){
        closesocket(_connectSocket);
        qDebug()<<"发送失败\n";
    }
    else{
        qDebug()<<"发送完成\n";
    }

    char recvBuf[DEFAULT_BUFLEN];
    if(recv(_connectSocket,recvBuf,DEFAULT_BUFLEN,0)<=0){
        closesocket(_connectSocket);
        qDebug()<<"接受失败\n";
    }
    else{
        qDebug()<<"接收到"<<recvBuf<<endl;
        return recvBuf;
    }




}






string ClientSocket::get_user_name(){
    return userName;
}
