#ifndef DEVICE_EXCEPTION_H
#define DEVICE_EXCEPTION_H

#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QSqlQuery>
#include <QDebug>

enum Error_type{
    Error,
    Warning,
    Critical,
    None
};

//디바이스 exception 객체 함수로 변환함
class Device_Exception
{
public:
    Device_Exception(const QString& s, int _Line_, Error_type type);
    QString         get_error_string();

private:
    int             insert_db_error();

private:
    Error_type        type__;
    const QString&    err_str__;
    int               line__;

};


#endif // DEVICE_EXCEPTION_H
