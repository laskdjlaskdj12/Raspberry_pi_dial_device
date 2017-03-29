#ifndef DEVICE_EXCEPTION_H
#define DEVICE_EXCEPTION_H

#include <QJsonObject>
#include <QSqlDatabase>

//디바이스 exception 객체 함수로 변환함
class Device_Exception
{
public:
    Device_Exception(const QString& s, int _Line_);
    void operator   ()(int type);
    int             get_error_code();
    QString         get_error_string();
};


#endif // DEVICE_EXCEPTION_H
