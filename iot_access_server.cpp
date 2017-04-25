#include "iot_access_server.h"

IOT_Access_Server::IOT_Access_Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer;


    //현재 서포트가 되는 디바이스 리스트 를 매핑함
    device_list["Moter"] = 0;

    //만약 연결 요청 시그널이 왔을 경우 connect
    connect(server,SIGNAL(newConnection()), this, SLOT(connect_socket ()));

    //현재 connect가 되어있는 db의 이름을 찾아서 오픈
    if (QSqlDatabase::contains ("Device_List_Connection") == false){
        qDebug()<<"[Error]: Didn't find QSqlDatabase";
        return;
    }

    db = QSqlDatabase::database ("Device_List_Connection");

    if (db.open () == false){   qDebug()<<"[Error]: Didn't open db"; return; }
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

    if (server->listen (QHostAddress::Any, port) == false){
        qDebug()<<"[Info] : can't open server listen : "<<server->errorString ();
        return;
    }
    qDebug()<<"[Info] : open Server listen";

}

// WiringPi 통해 추가해야할 부분
void IOT_Access_Server::set_room_tempture(QString pid, int temp)
{

    qDebug()<<"[Info] : set romm tempture : "<<temp;
}

void IOT_Access_Server::set_bathroom_tempture(QString pid, int temp)
{
    qDebug()<<"[Info] : set bathroom tempture : "<<temp;
}

//현재 디바이스 정보들을 로딩하여 전송
QJsonObject IOT_Access_Server::current_device_list()
{
    try{
        /*
     * 1. 먼저 Device_List 테이블에 있는 디바이스 정보부터 쿼리를 통해 수집
     * 2. pid를 통해 Type_List 테이블에 있는 디바이스 정보를 수집
     * 3. 그것을 JSON화 팩토링을 통해서 전송
     * 4. 에러가 났을시 NULL을 리턴
     * */

        QJsonObject device_list;
        QJsonArray _device_array_;

        QSqlQuery db_device_list_query(db);
        QSqlQuery db_device_type_list_query(db);

        db_device_list_query.prepare ("SELECT `device_type`, `device_name`, `device_pid`, `device_gpio` FROM `Device_list` ");
        if( db_device_list_query.exec () == false){ throw db_device_list_query.lastError (); }

        while(db_device_list_query.next ()){

            QJsonObject _cur_div_;

            _cur_div_["d_type"] = db_device_list_query.value (0).toString ();
            _cur_div_["d_name"] = db_device_list_query.value (1).toString ();
            _cur_div_["d_pid"]  = db_device_list_query.value (2).toString ();
            _cur_div_["d_gpio"] = db_device_list_query.value (3).toInt ();


            //만약 디바이스 타입 테이블에 대한 정보가 있을경우 디바이스 리스트에서 pid를 통해 검색
            if( db_device_list_query.value (0).toString () == "Moter"){

                db_device_type_list_query.prepare ("SELECT * FROM `Moter_Table` WHERE `device_pid` = (:pid) ;");
                db_device_type_list_query.bindValue (":pid", db_device_list_query.value (2).toString ());

                if( db_device_type_list_query.exec () == false){ throw db_device_type_list_query.lastError (); }

                //next를 통해서 다음 쿼리로 넘김
                db_device_type_list_query.last ();

                if(db_device_type_list_query.at () + 1 < 0){
                    qDebug()<<"[Error] : query size is : "<<db_device_type_list_query.at () + 1 <<" reason : "<<db.lastError ().text ();
                }

                db_device_type_list_query.first ();
                qDebug()<< "==================== DataBase object ===========================";
                qDebug()<<"[d_pid] : "<< db_device_list_query.value (2).toString ();
                qDebug()<<"[d_range] : " << db_device_type_list_query.value (2).toInt ();
                qDebug()<<"[d_max_range] : " << db_device_type_list_query.value (4).toInt ();
                qDebug()<<"[d_min_range] : " << db_device_type_list_query.value (5).toInt ();

                _cur_div_["d_range"] =     db_device_type_list_query.value (2).toInt ();
                _cur_div_["d_max_range"] = db_device_type_list_query.value (4).toInt ();
                _cur_div_["d_min_range"] = db_device_type_list_query.value (5).toInt ();
            }

            QJsonValue test_value = _cur_div_;
            _device_array_.append (test_value);

        }

        device_list["device_list"] = _device_array_;

        //json 파서가 된것을 디버그 창
        qDebug()<<"===================== load QJsonObject =====================";
        qDebug()<<QString (QJsonDocument(device_list).toJson ());

        return device_list;

    }catch(QSqlError& e){

        qDebug()<<"[Error] : " << e.text ();

        QJsonObject err_obj;
        err_obj["Error_String"] = e.text ();

        return err_obj;
    }
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

        QSqlQuery       sql_query(db);
        QString         res_type;
        QJsonObject     res_obj;
        QJsonDocument   doc;
        QJsonObject     obj;


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
        doc = lib.recv_Json ();

        //저장한 QJsonDocument를 QJsonObject로 변환시킴
        obj = doc.object ();


        //======================== Device_Connect_Protocol_Area ========================

        //만약 doc가 recv받을때 null일 경우

        if (doc.isNull () || doc.isEmpty ()){
            qDebug()<<"[Error] : can't convert QJsonDocument";
            res_obj["connect"] = false;
        }

        //만약 connect가 false일 경우

        if (obj["connect"].isNull () == true){
            qDebug()<<"[Info] : Error of receving Protocol";
            res_obj["connect"] = false;
        }

        res_obj["connect"] = true;


        //디바이스 조작 프로토콜일경우
        if(obj["device"].isNull () == false){

            res_obj["connect"] = true;
            res_obj["device"]  = true;

            //만약 json send가 false일 경우
            if (lib.send_Json (res_obj) == false){
                qDebug()<<"[Error] : send_Json is fail";
                throw lib.get_socket ()->error ();
            }

            //send Json object으로 전송함
            qDebug()<<"[Info] : send Json object";



            //======================== Device_Adjust_Protocol_AREA ========================


            //lib.recv_Json을 수신
            doc = lib.recv_Json ();

            //JSON을 판독을 못할경우
            if (doc.isNull () || doc.isEmpty ()){
                qDebug()<<"[Error] : can't convert QJsonDocument";
                obj["connect"] = false;
            }

            //현재 들어온 obj를 현재 들어온 obj으로 변환
            obj = doc.object ();

            if (obj["add_device"].isNull () == false){

                /*
                 * 1. json["add_device"]의 섹션이 체크
                 * 2. json["d_type"].tostring으로 디바이스 타입을 할당
                 * 3. pid를 static Create_pid()으로 pid를 먼저 할당
                 * 4. 디바이스 gpio, name, identify_mobile_number, pid 를 설정
                 * 5. add_raspberry_device의 signal를 emit -> slot에서 디바이스 추가시 타입부분별로 테이블을 만들어서 타입을 추가
                 * 6. add_raspberry_device의 리턴 pid를 JsonObject로 저장해서 전송
                 *
                 * */

                Device_class* add_device_type;

                switch(device_list[obj["d_type"].toString ()]){

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

                QString pid = raspberry_control::Create_pid ();

                emit add_raspberry_device (add_device_type->get_device_name ()\
                                           , add_device_type->get_device_type ()\
                                           , add_device_type->get_identify_mobile_number ()\
                                           , add_device_type->get_device_gpio ()\
                                           , pid
                                           );

                if ( pid < 0 ) {
                    throw QString("Server_Add_Error");
                }

                //클라이언트로 리턴할 pid를 설정
                res_obj["pid"] = pid;

                add_device_type->deleteLater ();


            }

            //디바이스가 remove일 경우
            else if (obj["remove_device"].isNull () == false){

                if (obj["pid"].isNull ()){   throw QString("pid is not valid"); }

                else{

                    if (emit remove_raspberry_device (obj["pid"].toString ()) != 0) { throw QString("remove device is fail");}
                }
            }

            //디바이스가 update일 경우
            else if (obj["update_device"].isNull () == false){

                if (obj["pid"].isNull ()){   throw QString("pid is not valid"); }

                else{

                    if (emit update_raspberry_devcie (obj["pid"].toString (), obj) != 0) { throw QString("remove device is fail");}
                }
            }

            //사용되고있는 디바이스 정보들을 로딩
            else if(obj["load_device_info"].isNull () == false){

                res_obj = current_device_list ();

                //리턴값이 에러일경우
                if(res_obj["Error_String"].isNull () == false){

                    //에러값을 설정후 전달
                    res_obj["Message"] = "Device_Info DataBase_Json_Error";
                }
            }


            //device조작 명령이 올경우
            else if (obj["tempture"].isNull () == false){

                if (obj["d_pid"].isNull () == true){
                    throw QString("No setting Temptreu");
                }

                //pid를 검색
                sql_query.prepare ("SELECT `device_type` FROM `Device_list` WHERE `device_pid` = :pid ;");
                sql_query.bindValue (":pid", obj["d_pid"].toString ());

                if (sql_query.exec () == false){ throw QString("server sql is fail");}

                sql_query.last ();

                //만약 pid결과가 검색이 되지않았을경우 예외처리
                if (sql_query.at () + 1 == 0){ throw QString ("No exsist pid");}

                sql_query.first ();

                res_type = sql_query.value (0).toString ();


                //만약 obj["adjust"] == set일경우
                if(obj["adjust"].toString () == "set"){

                    //해당 디바이스의 타입클래스를 할당후 sql타입테이블로 input하여 조작
                    if( device_list[res_type] == 0 ){

                        Moter* adjust_device_moter = new Moter;

                        adjust_device_moter->set_device_pid (res_type);

                        adjust_device_moter->set_moter_position ((obj["tempture"].toInt () / 100) * 24);

                        res_obj["tempture"] = obj["tempture"].toInt ();

                        adjust_device_moter->deleteLater ();

                    } else {

                        throw QString("No Such Device type");
                    }

                    //만약 obj["adjust"] == get일경우 타입 테이블를 통해서 현재 range를 받음
                }else if( obj["adjust"].toString () == "get"){

                    sql_query.prepare ("SELECT * FROM `" + res_type + "_Table` WHERE `device_pid` = :pid ;");
                    sql_query.bindValue (":pid", obj["d_pid"].toString ());

                    if(sql_query.exec () == false){
                        throw sql_query.lastError ().text ();
                    }

                    sql_query.first ();
                    int device_current_range = sql_query.value (2).toInt ();
                    int device_max_range = sql_query.value (4).toInt ();

                    res_obj["tempture"] = (device_current_range /device_max_range) * 100;

                }

            }
        }
        //클라이언트로 전송
        if (lib.send_Json (res_obj) == false){
            qDebug()<<"[Info] : send setting";
        }

        //클라이언트 disconnect
        qDebug()<<"[Info] : disconnect client";


        //클라이언트 disconnect를 함
        lib.disconnect_socket ();


    }catch(QAbstractSocket::SocketError e){

        //에러를 발생함
        qDebug()<<"[Error] : server error "<<lib.get_socket ()->errorString ();

        //클라이언트에 전송이 가능할경우
        if (lib.get_socket ()->isWritable ()){
            QJsonObject obj;
            obj["Message"] = "Protocol isn't readable";

            if (lib.send_Json (obj) == false){
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

            if (lib.send_Json (obj) == false){
                qDebug()<<"[Error] : send_error log is fail";
            }
        }

        //클라이언트를 disconnect함
        lib.disconnect_socket ();
    }
}

void IOT_Access_Server::open_server()
{
    open_server (43100);
}


