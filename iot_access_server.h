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
#include "qt_json_socket_lib.h"
#include "device_class.h"

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
    QJsonObject current_device_list();

signals:
    int add_raspberry_device(QString d_name, QString Type, QString Device_ownder_number, int gpio_number);
    int remove_raspberry_device (QString pid);
    int update_raspberry_devcie (QString pid, const QJsonObject obj);

public slots:
    void disconnect_from_client();
    void connect_socket();


private:
    QTcpServer* server;

    //만약 소켓이 accept를 할경우
    Qt_Json_Socket_Lib lib;

    QMap<QString,int> device_list;

    //디바이스 저장 db
    QSqlDatabase db;

    Device_class* adjust_device;

};

#endif // IOT_ACCESS_SERVER_H
