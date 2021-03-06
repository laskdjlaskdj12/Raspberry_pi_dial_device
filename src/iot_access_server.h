#ifndef IOT_ACCESS_SERVER_H
#define IOT_ACCESS_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "init_rasapberry.h"
#include "qt_json_socket_lib.h"
#include "device_class.h"
#include "device_exception.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

#include "raspberry_device.h"
#include "qt_json_socket_lib.h"


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
    void open_server (int port);

private:
<<<<<<< HEAD
    void set_room_tempture (int temp);
    void set_bathroom_tempture (int temp);
    void set_gpio (int gpio_pine);
    void set_position (int position);
    int get_position ();


public slots:
    //만약 connection이 왔을때
    void disconnect_from_client ();
    void connect_socket ();
=======
    void set_room_tempture(QString pid, int temp);
    void set_bathroom_tempture(QString pid, int temp);
    QJsonObject current_device_list();

signals:
    int add_raspberry_device(QString d_name, QString Type, QString Device_ownder_number, int gpio_number, QString pid);
    int remove_raspberry_device (QString pid);
    int update_raspberry_devcie (QString pid, const QJsonObject obj);

public slots:
    void disconnect_from_client();
    void connect_socket();
    void open_server();

>>>>>>> refs/heads/Re_Factoring_Branch
private:
    QTcpServer* server;

    //만약 소켓이 accept를 할경우
    Qt_Json_Socket_Lib lib;

    QMap<QString,int> device_list;

<<<<<<< HEAD
    Raspberry_device dev;
=======
    //디바이스 저장 db
    QSqlDatabase db;

    Device_class* adjust_device;

>>>>>>> refs/heads/Re_Factoring_Branch

};

#endif // IOT_ACCESS_SERVER_H
