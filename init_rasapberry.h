#ifndef INIT_RASAPBERRY_H
#define INIT_RASAPBERRY_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>
#include <QDebug>
#include <QTime>
#include <QCryptographicHash>
#include "device_class.h"
//#include <wiringPi.h>
//#include <device_class.h>

class init_rasapberry_control : public QObject
{
    Q_OBJECT
public:
    explicit init_rasapberry_control(QObject *parent = 0);
    virtual ~init_rasapberry_control      ();
    virtual int init_raspberry__          () = 0;
    virtual int check_raspberry_device__  () = 0;
    virtual int add_raspberry_device__    () = 0;
    virtual int remove_raspberry_device__ () = 0;
    virtual int show_device_list__        () = 0;
    virtual int show_device_info__        () = 0;


};

class raspberry_control : public init_rasapberry_control
{
    Q_OBJECT
public:
    explicit raspberry_control(QObject *parent = 0);
    virtual ~raspberry_control            ();
    virtual int init_raspberry__          ();
    virtual int check_raspberry_device__  ();
    virtual int add_raspberry_device__    ();
    virtual int remove_raspberry_device__ ();
    virtual int show_device_list__        ();
    virtual int show_device_info__        ();

private:
    QString Create_pid();
    bool remove_device(int pid);
    int update_device(int pid, const QJsonObject obj);

    //디바이스 타입별로 테이블에 저장
    int add_device_type_table(QString d_name, QString Type, int gpio_number, QString pid);

public slots:

    //클라이언트에서 원격으로 라즈베리파이의 디바이스를 추가 이벤트시 slot
    int add_raspberry_device(QString d_name, QString Type, QString Device_ownder_number, int gpio_number);
    int remove_raspberry_device (QString pid);
    int update_rapsberry_device (QString pid, const QJsonObject obj);


private:
    QSqlDatabase db;
    QString table_name;
    bool is_table_exsist;
    int device_list_size;

    //디바이스 타입 리스트
    QList<QString> type_list_;


};

#endif // INIT_RASAPBERRY_H
