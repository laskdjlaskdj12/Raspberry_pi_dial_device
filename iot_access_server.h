#ifndef IOT_ACCESS_SERVER_H
#define IOT_ACCESS_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include "qt_json_socket_lib.h"
#include <QMap>

/* 1. IOTDevcie는 Socket_Lib_test를 import를 함
 * 2. 만약 socket_Lib_test가 임포트가 안됬을 경우 QNetwrokAccessManager로 해결을 함
 *
 *
 * */

class IOT_Access_Server : public QObject
{
    Q_OBJECT
public:
    explicit IOT_Access_Server(QObject *parent = 0);
    ~IOT_Access_Server();
    void open_server(int port);

private:
    void set_room_tempture(int temp);
    void set_bathroom_tempture(int temp);

public slots:
    //만약 connection이 왔을때
    void disconnect_from_client();
    void connect_socket();
private:
    QTcpServer* server;
    QThread     sock_thread;

    //만약 소켓이 accept를 할경우
    Qt_Json_Socket_Lib lib;

    QMap<QString,int> device_list;


};

#endif // IOT_ACCESS_SERVER_H
