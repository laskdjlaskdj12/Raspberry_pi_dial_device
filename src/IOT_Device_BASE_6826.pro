QT += core network
QT -= gui

CONFIG += c++11

TARGET = IOT_Device
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    iot_access_server.cpp

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
    iot_access_server.h \
    qt_json_socket_lib_global.h \
    qt_json_socket_lib.h

macx: LIBS += -L$$PWD/../build-Qt_Json_Socket_Lib-Desktop_Qt_5_8_0_clang_64bit-Debug/ -lQt_Json_Socket_Lib.1.0.0

INCLUDEPATH += $$PWD/../build-Qt_Json_Socket_Lib-Desktop_Qt_5_8_0_clang_64bit-Debug
DEPENDPATH += $$PWD/../build-Qt_Json_Socket_Lib-Desktop_Qt_5_8_0_clang_64bit-Debug
