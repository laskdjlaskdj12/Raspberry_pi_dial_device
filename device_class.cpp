#include "device_class.h"

Device_class::Device_class(QObject *parent) : QObject(parent){}

Device_class::~Device_class(){}

Moter::Moter(QObject *parent):Device_class(parent)
{

}

Moter::~Moter()
{

}

QString Moter::get_device_name(){   return device_name; }
QString Moter::get_device_type(){   return device_type;}
uint Moter::get_device_gpio(){  return device_gpio;}
QString Moter::get_identify_mobile_number(){    return identify_mobile_number;}
int Moter::get_access_mobile_number(){  return access_mobile_number;}

void Moter::set_device_type(QString type){  device_type = type;}
void Moter::set_device_gpio(int gpio_num){  device_gpio = gpio_num;}
void Moter::set_identify_mobile_number(QString mobile_num){ identify_mobile_number = mobile_num;}
bool Moter::is_active(){    return device_active;}

bool Moter::is_number_match(int hash)
{
    if(device_hash == hash){
        return true;
    }
    return false;
}


void Moter::init_random_number(){   random_number = (uint)qrand();}

bool Moter::init_gpio()
{

}

bool Moter::init_position()
{

}

bool Moter::set_position(int range)
{

}

int Moter::get_position()
{

}
