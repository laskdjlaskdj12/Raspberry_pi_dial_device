QT += core network sql testlib
QT -= gui

CONFIG += c++11

TARGET = IOT_Device
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/iot_access_server.cpp \
    src/raspberry_device.cpp \
    src/softPwm.c \
    src/wiringPi.c \
    src/device_class.cpp \
    src/device_exception.cpp \
    src/init_rasapberry.cpp \
    src/qt_json_socket_lib.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    src/iot_access_server.h \
    src/qt_json_socket_lib_global.h \
    src/qt_json_socket_lib.h \
    src/softPwm.h \
    src/wiringPi.h \
    src/raspberry_device.h
    src/device_class.h \
    src/device_exception.h \
    src/init_rasapberry.h

macx: LIBS += -L$$PWD/../build-Qt_Json_Socket_Lib-Desktop_Qt_5_8_0_clang_64bit-Debug/ -lQt_Json_Socket_Lib.1.0.0

INCLUDEPATH += $$PWD/../build-Qt_Json_Socket_Lib-Desktop_Qt_5_8_0_clang_64bit-Debug
DEPENDPATH += $$PWD/../build-Qt_Json_Socket_Lib-Desktop_Qt_5_8_0_clang_64bit-Debug

INCLUDEPATH += $$PWD/../../wiringpi/wiringPi/wiringPi


# COPYWRITE BY laskdjlaskdj12 by inhoeKu
# This Copywrite only used by non commersial license and none ravinuew license
# ALL Soures is free
