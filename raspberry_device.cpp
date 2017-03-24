#include "raspberry_device.h"

Raspberry_device::Raspberry_device(QObject *parent) : QObject(parent), position (4), gpio_pin_number (0), base_number (0), radiaction (21)
{

}

Raspberry_device::~Raspberry_device()
{

}

bool Raspberry_device::init_gpio()
{
    try{

        if(wiringPiSetup() == -1){
            throw "wiringPi_Setup()_fail";
        }

   //     pinMode(gpio_pine, OUTPUT);

   //     digitalWrite(gpio_pine,LOW);

   //     softPwmCreate(gpio_pine, 0, 200);

    }catch(const QString e){
        qDebug()<<"[Error] : "<< e ;
        return false;
    }
}

bool Raspberry_device::set_gpio(int gpio_pine)
{
    gpio_pin_number = gpio_pine;
}

void Raspberry_device::set_position(int position)
{
    int angle = (position*21) / 100;
  //  softPwmWrite(adjust_gpio, (angle+base_number));

    delay(50);
}

int Raspberry_device::get_position()
{
    return position;
}
