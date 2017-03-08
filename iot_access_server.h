#ifndef IOT_ACCESS_SERVER_H
#define IOT_ACCESS_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
//#include "socket_access.h"
#include <QThread>


/*
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
    void set_room_tempture(int temp);
    void set_bathroom_tempture(int temp);


private:


public slots:
    //만약 connection이 왔을때 socket에 할당을 함

signals:

public slots:
    //만약 connection이 왔을때

    void connect_socket();
private:
    QTcpServer* server;
    //QThread sock_thread;
    QTcpSocket* socket;


};

#endif // IOT_ACCESS_SERVER_H
