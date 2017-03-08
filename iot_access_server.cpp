#include "iot_access_server.h"

IOT_Access_Server::IOT_Access_Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer;

    //만약 연결 요청 시그널이 왔을 경우 connect
    connect(server,SIGNAL(newConnection()), this, SLOT(connect_socket ()));
}

IOT_Access_Server::~IOT_Access_Server()
{
    qDebug()<<"[Info] : Close Server";
    server->close ();
}

void IOT_Access_Server::open_server(int port)
{
    qDebug()<<"[Info] : OpenServer ";
    qDebug()<<"[Info] : port :"<<port;

    if(server->listen (QHostAddress::Any, port) != true){
        qDebug()<<"[Info] : can't open server listen : "<<server->errorString ();
        return;
    }
    qDebug()<<"[Info] : open Server listen";

}

void IOT_Access_Server::set_room_tempture(int temp)
{

}

void IOT_Access_Server::set_bathroom_tempture(int temp)
{

}


//## slot area ##
void IOT_Access_Server::connect_socket()
{
    try{

        //소켓을 현재 클라이언트로부터 읽어들임
        Socket_Access access;

        QTcpSocket* sock = server->nextPendingConnection ();
        qDebug()<<"[Info] : Client is Request connect";

        //소켓을 server->nextpending으로 변경
        access.set_socket (sock);
        qDebug()<<"[Info] : set_socket clear";

        //access 프로토콜을 받음
        if(access.connect_clear () != true){
            qDebug()<<"[Error] : Error of accessd";
            return;
        }
        qDebug()<<"[Info] : set_socket clear";

    }catch(QAbstractSocket::SocketError e){
        qDebug()<<"[Error] : server error "<<server->errorString ();
    }
}

