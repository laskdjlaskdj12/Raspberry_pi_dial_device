#include "raspberry_protocol.h"

#include "raspberry_protocol.h"

Raspberry_Protocol::Raspberry_Protocol(QObject *parent) : QObject(parent),
    tempture__(0),
    connect__(false),
    message__("NULL"),
    device__("NULL")

{

}

Raspberry_Protocol::~Raspberry_Protocol(){
    device__.clear ();
    message__.clear ();
}

void Raspberry_Protocol::set_Device(QString s)
{
    device__ = s;
}

void Raspberry_Protocol::set_Message(QString s)
{
    message__ = s;
}

void Raspberry_Protocol::set_Tempreture(int i)
{
    tempture__ = i;
}

void Raspberry_Protocol::set_Connect(bool a)
{
    connect__ = a;
}

QJsonObject Raspberry_Protocol::get_Json()
{
    obj__["device"] = device__;
    obj__["message"] = message__;
    obj__["tempture"] = tempture__;
    obj__["connect"] = connect__;
    return obj__;
}

QJsonObject Raspberry_Protocol::get_Json_Object()
{
    return obj__;
}
QByteArray Raspberry_Protocol::get_Json_bytearray()
{
    QJsonDocument doc_(obj__);

    return doc_.toBinaryData ();
}

QByteArray Raspberry_Protocol::set_ByteArray(QByteArray a)
{
    QJsonDocument doc;
    doc.fromBinaryData (a);

   obj__ = doc.object ();
}
