#include "iot_access_server.h"

IOT_Access_Server::IOT_Access_Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer;


    //현재 서포트가 되는 디바이스 리스트 를 매핑함
    device_list["Moter"] = 0;

    //만약 연결 요청 시그널이 왔을 경우 connect
    connect(server,SIGNAL(newConnection()), this, SLOT(connect_socket ()));

    //현재 connect가 되어있는 db의 이름을 찾아서 오픈
    if (QSqlDatabase::contains ("Device_List_Connection") != true){
        qDebug()<<"[Error]: Didn't find QSqlDatabase";
        return;
    }

    db.database ("Device_List_Connection");

    if (db.open () != true){   qDebug()<<"[Error]: Didn't open db"; return; }
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

    if (server->listen (QHostAddress::Any, port) != true){
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
 * */
void IOT_Access_Server::connect_socket()
{
    try {
        /*
         * 1.connect 반응이 오면 connect accept
         * 2.Qt_Json_Socket_Lib::disconnect_socket()
         * */

        //각 타임아웃을 세팅
        lib.set_connect_timeout (3000);
        lib.set_recv_timeout (3000);
        lib.set_send_timeout (3000);

        //만약 connect이 되었을 경우 소켓을 먼저 세팅함
        lib.set_socket (server->nextPendingConnection ());
        qDebug()<<"[Info] : Setting the socket to send_Library";

        //먼저 recv를 통해 Json_protocol를 받아들임
        QJsonDocument   doc = lib.recv_Json ();
        QJsonObject     obj = doc.object ();
        QSqlQuery       sql_query(db);
        QString         res_type;
        QJsonObject     res_obj;

        //만약 doc가 recv받을때 null일 경우
        /*if (doc.isNull ()){
            qDebug()<<"[Error] : can't convert QJsonDocument";
            obj["connect"] = false;
        }

        //만약 connect가 false일 경우
        */if (obj["connect"].isBool () != true){
            qDebug()<<"[Info] : Error of receving Protocol";
            obj["connect"] = false;
        }/*

        //만약 json send가 false일 경우
        if (lib.send_Json (obj) != true){
            qDebug()<<"[Error] : send_Json is fail";
            throw lib.get_socket ()->error ();
        }

        //send Json object으로 전송함
        qDebug()<<"[Info] : send Json object";*/


        //addDevcie 일경우
        if (obj["add_devcie"].isNull () != true){

            Device_class* add_device_type;

            switch(device_list[obj["add_device"].toString ()]){

            case 0:
                add_device_type = new Moter;
                add_device_type->set_device_type ("Moter");
                break;
            default:
                throw QString("This Device not support");
                break;
            }

            add_device_type->set_device_gpio (obj["d_gpio"].toInt ());
            add_device_type->set_device_name (obj["d_name"].toString ());
            add_device_type->set_identify_mobile_number (obj["d_access_number"].toString ());

            if ( emit add_raspberry_device (add_device_type->get_device_name ()\
                                            , add_device_type->get_device_type ()\
                                            , add_device_type->get_identify_mobile_number ()\
                                            , add_device_type->get_device_gpio ()\
                                            ) < 0){
                throw QString("Server_Add_Error");
            }



        }

        //디바이스가 remove일 경우
        else if (obj["remove_device"].isNull () != true){

            if (obj["pid"].isNull ()){   throw QString("pid is not valid"); }

            else{

                if (emit remove_raspberry_device (obj["pid"].toString ()) != 0) { throw QString("remove device is fail");}
            }
        }

        //디바이스가 update일 경우
        else if (obj["update_device"].isNull () != true){
            if (obj["pid"].isNull ()){   throw QString("pid is not valid"); }

            else{

                if (emit update_raspberry_devcie (obj["pid"].toString (), obj) != 0) { throw QString("remove device is fail");}
            }
        }


        //device조작 명령이 올경우
        else if (obj["tempture"].isNull () != true){

            if (obj["d_pid"].isNull () == true){
                throw QString("No setting Temptreu");
            }

            sql_query.prepare ("SELECT `device_type` FROM `Device_list` WHERE `device_pid` = :pid ;");
            sql_query.bindValue (":pid", obj["d_pid"].toString ());

            if (sql_query.exec () != true){ throw QString("server sql is fail");}

            sql_query.last ();

            //만약 pid결과가 검색이 되지않았을경우
            if (sql_query.at () + 1 == 0){ throw QString ("No exsist pid");}

            sql_query.first ();
            res_type = sql_query.value (0).toString ();



            if( device_list[res_type] == 0 ){

                Moter* adjust_device_moter = new Moter;

                adjust_device_moter->set_device_pid (res_type);

                adjust_device_moter->set_moter_position ((obj["tempture"].toInt () / 100) * 24);

                res_obj["tempture"] = obj["tempture"].toInt ();

                adjust_device_moter->deleteLater ();

            } else {
                throw QString("No Such Device type");
            }







            //디바이스 pid를 통해서 조작
            /*
             * 1. 디바이스를 pid로 검색
             * 2. 디바이스 오브젝트 할당후 id를 바인딩함
             * 3. 디바이스 오브젝트에 moter 작동 함수를 실
             */


            /*//recv으로 조정을 함
            doc = lib.recv_Json ();

            //만약 doc가 읽을수 없을경우
            if (doc.isNull ()){
                qDebug()<<"[Error] : recv_Json protocol is fail";
                throw lib.get_socket ()->error ();
            }

            //수신받은 프로토콜을 QJsonObject로 변환함
            obj = doc.object ();*/

            //어떤 디바이스를 조작할지 명령을 정함

            //디바이스 pid를 통해서 조작
            /*
             * 1. 디바이스를 pid로 검색
             * 2. 디바이스 오브젝트 할당후 id를 바인딩함
             * 3. 디바이스 오브젝트에 moter 작동 함수를 실
             */

            /*switch(device_list[obj["device"].toString ()]){

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
            obj["connect"] = true;*/


            //클라이언트로 전송
            if (lib.send_Json (res_obj) != true){
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
        if (lib.get_socket ()->isWritable ()){
            QJsonObject obj;
            obj["Message"] = "Protocol isn't readable";

            if (lib.send_Json (obj) != true){
                qDebug()<<"[Error] : send_error log is fail";
            }
        }
        //클라이언트를 disconnect함
        lib.disconnect_socket ();

    }catch(const QString& e){

        //만약 소켓이 전송이 가능하다면
        if (lib.get_socket ()->isWritable ()){
            QJsonObject obj;
            obj["Message"] = e;

            if (lib.send_Json (obj) != true){
                qDebug()<<"[Error] : send_error log is fail";
            }
        }

        //클라이언트를 disconnect함
        lib.disconnect_socket ();
    }
}


