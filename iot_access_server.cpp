#include "iot_access_server.h"

IOT_Access_Server::IOT_Access_Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer;
    device_list["Livingroom"] = 0;
    device_list["Bathroom"] =   1;

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
    qDebug()<<"[Info] : set romm tempture : "<<temp;
}

void IOT_Access_Server::set_bathroom_tempture(int temp)
{
    qDebug()<<"[Info] : set bathroom tempture : "<<temp;
}






//## slot area ##

void IOT_Access_Server::disconnect_from_client()
{
    //만약 클라이언트에서 disconnect시그널이 왔을시에 라이브러리에서 disconnect함
    lib.disconnect_socket ();

    //QTcpSocket의 객체를 delete함
    lib.delete_QTcpSocket ();
}

/*
 * 1.connect시 프로토콜을 connect 프로시저 내에 읽고 disconnect할수 있도록 함
 * 2. 기존의
 *
 *
 * */
void IOT_Access_Server::connect_socket()
{
    try{
        /*
         * 1.connect 반응이 오면 connectaccept함
         * 2.Qt_Json_Socket_Lib::disconnect_socket()이 오면 바로
         * 3.
         * */

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

        //send Json object으로 전송함
        qDebug()<<"[Info] : send Json object";



        //device조작 명령이 올경우
        if(obj["device"].isNull () != true){

            //recv으로 조정을 함
            doc = lib.recv_Json ();

            //만약 doc가 읽을수 없을경우
            if(doc.isNull ()){
                qDebug()<<"[Error] : recv_Json protocol is fail";
                throw lib.get_socket ()->error ();
            }

            //수신받은 프로토콜을 QJsonObjectfh qusghks
            obj = doc.object ();

            switch(device_list[obj["device"].toString ()]){

            case 0:
                qDebug()<<"[Info] : set_tempture of LivingRoom tempture : "<<obj["tempture"].toInt ();
                set_room_tempture (obj["tempture"].toInt ());
                break;
            case 1:
                qDebug()<<"[Info] : set_tempture of BathRoom tempture : "<<obj["tempture"].toInt ();
                set_room_tempture (obj["tempture"].toInt ());
                break;


            }

            //프로토콜 obj["connect"]로 설정
            obj["connect"] = true;


            //클라이언트로 전송
            if(lib.send_Json (obj) != true){
                qDebug()<<"[Info] : send setting";
            }

            //클라이언트 disconnect
            qDebug()<<"[Info] : disconnect client";

        }

        //클라이언트 disconnect를 함
        lib.disconnect_socket ();


    }catch(QAbstractSocket::SocketError e){

        //에러를 발생함
        qDebug()<<"[Error] : server error "<<lib.get_socket ()->errorString ();

        //클라이언트에 전송이 가능할경우
        if(lib.get_socket ()->isWritable ()){
            QJsonObject obj;
            obj["Message"] = "Protocol isn't readable";

            if(lib.send_Json (obj) != true){
                qDebug()<<"[Error] : send_error log is fail";
            }
        }
        //클라이언트를 disconnect함
        lib.disconnect_socket ();
    }
}

