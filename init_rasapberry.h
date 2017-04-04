#ifndef INIT_RASAPBERRY_H
#define INIT_RASAPBERRY_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>
#include <QDebug>
#include <QCryptographicHash>
#include <wiringPi.h>
#include <device_class.h>

class init_rasapberry_control : public QObject
{
    Q_OBJECT
public:
    explicit init_rasapberry_control(QObject *parent = 0);
    virtual ~init_rasapberry_control();
    virtual int init_raspberry          () = 0;
    virtual int check_raspberry_device  () = 0;
    virtual int add_rasbperry_device    () = 0;
    virtual int remove_raspberry_device () = 0;
    virtual int show_device_list        () = 0;
    virtual int show_device_info        () = 0;


};

class raspberry_control : public init_rasapberry_control
{
    Q_OBJECT
public:
    explicit raspberry_control(QObject *parent = 0);
    virtual ~raspberry_control          ();
    virtual int init_raspberry          ();
    virtual int check_raspberry_device  ();
    virtual int add_rasbperry_device    ();
    virtual int remove_raspberry_device ();
    virtual int show_device_list        ();
    virtual int show_device_info        ();

private:
    QString Create_pid();
    bool remove_device(int pid);

public slots:

    //클라이언트에서 원격으로 라즈베리파이의 디바이스를 추가 이벤트시 slot
    int add_raspberry_device(QString d_name, QString Type, QString Device_ownder_number, int gpio_number);
    int remove_raspberry_device (QString pid);

private:
    QSqlDatabase db;
    QString table_name;
    bool is_table_exsist;
    int device_list_size;

};

#endif // INIT_RASAPBERRY_H
