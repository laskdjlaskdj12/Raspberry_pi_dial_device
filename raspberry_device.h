#ifndef RASPBERRY_DEVICE_H
#define RASPBERRY_DEVICE_H

#include <QObject>
#include <QDebug>
#include <wiringPi.h>
#include <softPwm.h>

class Raspberry_device : public QObject
{
    Q_OBJECT
public:
    explicit Raspberry_device(QObject *parent = 0);
    ~Raspberry_device ();
    bool init_gpio ();
    bool set_gpio (int gpio_pine);
    void set_position (int position);
    int get_position ();

signals:

public slots:

private:
    //position은 반드시 4로 초기화가 진행되어야함
    int position;

    //gpio pin_number
    int gpio_pin_number;

    //position_base_number
    int base_number;

    //position_radiaction
    int radiaction;
};

#endif // RASPBERRY_DEVICE_H
