#include <QCoreApplication>
#include "iot_access_server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    IOT_Access_Server server;

    server.open_server (43100);

    return a.exec();
}
