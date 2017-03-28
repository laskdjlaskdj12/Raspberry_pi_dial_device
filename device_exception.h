#ifndef DEVICE_EXCEPTION_H
#define DEVICE_EXCEPTION_H

#include <QObject>

class Device_Exception : public QObject
{
    Q_OBJECT
public:
    explicit Device_Exception(QObject *parent = 0);

signals:

public slots:
};

#endif // DEVICE_EXCEPTION_H