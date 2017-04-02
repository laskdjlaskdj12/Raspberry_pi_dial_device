#include "device_class.h"
Device_class::Device_class(QObject *parent) : QObject(parent){}

Device_class::~Device_class(){}

Moter::Moter(QObject *parent):Device_class(parent)\
  ,device_type("NULL")\
  ,device_name("NULL")\
  ,device_pid("NULL")\
  ,device_hash("NULL")\
  ,identify_mobile_number("NULL")\
  ,max_range(0)\
  ,min_range(0)\
  ,current_range(0)\
  ,device_gpio(0)\
  ,random_number(0)\
  ,access_mobile_number(0)\
  ,device_active(false){}

Moter::~Moter(){}

QString Moter::get_device_name(){   return device_name; }
QString Moter::get_device_type(){   return device_type;}
uint Moter::get_device_gpio(){  return device_gpio;}
QString Moter::get_device_pid(){    return device_pid;}
QString Moter::get_identify_mobile_number(){    return identify_mobile_number;}
int Moter::get_access_mobile_number(){  return access_mobile_number;}

void Moter::set_device_name(QString name){  device_name = name;}
void Moter::set_device_type(QString type){  device_type = type;}
void Moter::set_device_gpio(int gpio_num){  device_gpio = gpio_num;}
void Moter::set_identify_mobile_number(QString mobile_num){ identify_mobile_number = mobile_num;}
void Moter::set_device_pid(QString pid){    device_pid = pid;}

bool Moter::is_active(){    return device_active;}
void Moter::init_random_number(){   random_number = (uint)qrand();}

bool Moter::is_number_match(int hash)
{
    if (device_hash == hash){
        return true;
    }
    return false;
}
bool Moter::init_gpio()
{
    try{
        if (wiringPiSetup() == -1){ throw Device_Exception("wiringPiSetup()", __LINE__, Error_type::Error);}

        pinMode(device_gpio, OUTPUT);

        digitalWrite(device_gpio,LOW);

        if (softPwmCreate(device_gpio,0,200) == -1){ throw Device_Exception("softPwmCreate",__LINE__, Error_type::Error);}

        device_active = true;

        return true;

    }catch(Device_Exception& e){
        e.get_error_string ();
        return false;
    }
}

bool Moter::init_position()
{
    try{
        if ( device_active == false ){ throw Device_Exception("init_position : gpio device is not active", __LINE__, Error_type::Warning);}

        softPwmWrite (device_gpio, min_range);

        delay(50);

        return true;

    }catch(Device_Exception& e){
        e.get_error_string ();
        return false;
    }
}

bool Moter::set_position(uint range)
{
    try{

        if ( device_active == false ){ throw Device_Exception("init_position : gpio device is not active", __LINE__, Error_type::Warning);}

        if (range > max_range || range < min_range){

            if (range > max_range){

                current_range = max_range;

                softPwmWrite (device_gpio, current_range);

                delay(50);

            }else{

                current_range = min_range;

                softPwmWrite (device_gpio, current_range);

                delay(50);

            }
            return false;
        }

        current_range = range;

        softPwmWrite (device_gpio, current_range);

        delay(50);

        return true;

    }catch(Device_Exception& e){

        e.get_error_string ();
        return false;
    }
}


int Moter::get_position()
{
    return current_range;
}
