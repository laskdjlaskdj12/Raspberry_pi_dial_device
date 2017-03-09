#ifndef RASPBERRY_PROTOCOL_H
#define RASPBERRY_PROTOCOL_H

#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class Raspberry_Protocol : public QObject
{
    Q_OBJECT
public:
    explicit Raspberry_Protocol(QObject *parent = 0);
    ~Raspberry_Protocol();
    void set_Device(QString s);
    void set_Message(QString s);
    void set_Tempreture(int i);
    void set_Connect(bool a);
    void set_ByteArray();

    QJsonObject get_Json();
    QByteArray get_Json_bytearray();
    QJsonObject get_Json_Object();
    QByteArray set_ByteArray(QByteArray a);


private:
    QString device__;
    QString message__;
    int     tempture__;
    bool    connect__;

    QJsonObject obj__;
};

#endif // RASPBERRY_PROTOCOL_H
