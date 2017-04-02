#include <QCoreApplication>
#include "iot_access_server.h"
#include "init_rasapberry.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QThread     sock_thread;

    raspberry_control ras_control;

    //서버는 컨트롤 패널없이 계속 돌아감
    IOT_Access_Server* server = new IOT_Access_Server;
    server->moveToThread (&sock_thread);
    server->open_server (43100);

    //

    return a.exec();
}
