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

        //각 타임아웃을 세팅
        lib.set_connect_timeout (3000);
        lib.set_recv_timeout (3000);
        lib.set_send_timeout (3000);


        //만약 connect이 되었을 경우 소켓을 먼저 세팅함
        lib.set_socket (server->nextPendingConnection ());
        qDebug()<<"[Info] : Setting the socket to send_Library";

        //먼저 recv를 통해 Json_protocol를 받아들임
        QJsonDocument doc = lib.recv_Json ();
        QJsonObject obj = doc.object ();

        //만약 doc가 recv받을때 null일 경우
        if(doc.isNull ()){
            qDebug()<<"[Error] : can't convert QJsonDocument";
            obj["connect"] = false;
        }

        //만약 connect가 false일 경우
        if(obj["connect"].isBool () != true){
            qDebug()<<"[Info] : Error of receving Protocol";
            obj["connect"] = false;
        }

        //만약 json send가 false일 경우
        if(lib.send_Json (obj) != true){
            qDebug()<<"[Error] : send_Json is fail";
            throw lib.get_socket ()->error ();
        }

        qDebug()<<"[Info] : send Json object";

    }catch(QAbstractSocket::SocketError e){
        qDebug()<<"[Error] : server error "<<lib.get_socket ()->errorString ();
    }
}

