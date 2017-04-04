#ifndef DEVICE_CLASS_H
#define DEVICE_CLASS_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QtGlobal>
#include <QHash>
#include <QCryptographicHash>
//#include "softPwm.h"
//#include "wiringPi.h"
#include "device_exception.h"


class Device_class : public QObject
{
    Q_OBJECT
public:
    explicit Device_class(QObject *parent = 0);
    virtual ~Device_class();
    virtual QString         get_device_name()                                 = 0;
    virtual QString         get_device_type()                                 = 0;
    virtual uint            get_device_gpio()                                 = 0;
    virtual QString         get_device_pid()                                  = 0;
    virtual QString         get_identify_mobile_number()                      = 0;
    virtual int             get_access_mobile_number()                        = 0;

    virtual void            init_random_number()                              = 0;
    virtual void            set_device_name(QString name)                     = 0;
    virtual void            set_device_type(QString type)                     = 0;
    virtual void            set_device_gpio(int gpio_num)                     = 0;
    virtual void            set_device_pid(QString pid)                       = 0;
    virtual void            set_identify_mobile_number(QString mobile_num)    = 0;
    virtual bool            is_number_match(int p_num)                        = 0;
    virtual bool            is_active()                                       = 0;

signals:

public slots:

};

//모터 클래스
class Moter: public Device_class
{
    Q_OBJECT
public:
    explicit Moter(QObject* parent = 0);
    virtual  ~Moter();

    virtual QString         get_device_name();
    virtual QString         get_device_type();
    virtual uint            get_device_gpio();
    virtual QString         get_device_pid();
    virtual QString         get_identify_mobile_number();
    virtual int             get_access_mobile_number();

    virtual void            set_device_name(QString name);
    virtual void            init_random_number();
    virtual void            set_device_type(QString type);
    virtual void            set_device_gpio(int gpio_num);
    virtual void            set_identify_mobile_number(QString mobile_num);
    virtual void            set_device_pid(QString pid);

    virtual bool            is_number_match(int p_num);
    virtual bool            is_active();

    //디바이스 제어부분
private:
    bool                     init_gpio();
    bool                     init_position();
    bool                     set_position(uint range);
    int                      get_position();
signals:
    void device_error(int error_no);
public slots:

private:
    QString device_type;
    QString device_name;
    QString device_pid;
    QString device_hash;
    QString identify_mobile_number;
    uint    current_range;
    uint    device_gpio;
    uint    random_number;
    uint    min_range;
    uint    max_range;
    int     access_mobile_number;
    bool    device_active;


};

/*//적외선 센서 클래스
class IF_sencer : public Device_class
{
public:
    explicit IF_sencer(QObject* parent = 0);
    ~IF_sencer();

    virtual QString         get_device_name();
    virtual QString         get_device_type();
    virtual uint            get_device_gpio();
    virtual QString         get_identify_mobile_number();
    virtual int             get_access_mobile_number();

    virtual void            init_private_hash();
    virtual void            set_device_type(QString type);
    virtual void            set_device_gpio(int gpio_num);
    virtual void            set_identify_mobile_number();

    virtual bool            is_private_hash_match(int p_num);
    virtual bool            is_active();

private:
    //void set_control(int duration);
signals:

public slots:

private:
    QString device_type;
    QString device_name;
    QString device_pid;
    QString identify_mobile_number;
    uint    device_gpio;
    bool    device_active;
};*/



#endif // DEVICE_CLASS_H
