#include <QCoreApplication>
#include "iot_access_server.h"
#include "init_rasapberry.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QThread*     sock_thread = new QThread;

    raspberry_control* ras_control = new raspberry_control;
    IOT_Access_Server* server = new IOT_Access_Server;     //서버는 컨트롤 패널없이 계속 돌아감

    QObject::connect ( server, SIGNAL( add_raspberry_device(QString,QString,QString,int,QString)) , ras_control, SLOT( add_raspberry_device(QString,QString,QString,int,QString)));
    QObject::connect ( server, SIGNAL( remove_raspberry_device(QString)) , ras_control, SLOT( remove_raspberry_device(QString)));
    QObject::connect ( server, SIGNAL (update_raspberry_devcie( QString, QJsonObject ) ), ras_control, SLOT ( update_rapsberry_device( QString, QJsonObject ) ) );

    //쓰레드 start 시그널을 server의 open_server 슬롯으로 바인딩
    QObject::connect (sock_thread, SIGNAL ( started() ), server, SLOT( open_server() ) );


    server->moveToThread (sock_thread);
    //쓰레드 시작
    sock_thread->start ();

    qDebug()<<"===================== RASPBERRY Control_Panel =====================";
    qDebug()<<"[1] check_raspberry_device";
    qDebug()<<"[2] add_rapberry_device";
    qDebug()<<"[3] remove_raspberry_deivce";
    qDebug()<<"[4] show_device_list";
    qDebug()<<"[5] show_device_info";


    return a.exec();
}
