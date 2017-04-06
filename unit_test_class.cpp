
#include <QtTest/QtTest>
#include <QTest>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <iostream>
#include "device_class.h"
#include "init_rasapberry.h"



class Unit_test_class:public QObject
{
    Q_OBJECT
public:
    Unit_test_class(QObject* parent = 0);
    ~Unit_test_class();

private slots:
    void device_class_get_name_test();
    void device_class_getdevice_type();
    void device_get_device_gpio();
    void device_get_device_pid();
    void device_get_identify_mobile_number();
    void device_set_moter_position();
    void device_get_moter_position();


    void init_raspberry_control_test();
    void init_check_raspberry_device_test();
    //void add_raspberry_device_test();
    void remove_raspberry_device_test();
    void show_device_list();
    void show_device_info();

private:
    Moter* test_class;
    raspberry_control* control;

};


Unit_test_class::Unit_test_class(QObject *parent)
{
    test_class = new Moter;
    control = new raspberry_control;


}

Unit_test_class::~Unit_test_class()
{
    delete test_class;
    delete control;

}

void Unit_test_class::device_class_get_name_test ()
{
    //QTest::addColumn<QString>("input_string");

    test_class->set_device_name ("Test_name");
    QCOMPARE(test_class->get_device_name (), QString("Test_name"));

}

void Unit_test_class::device_class_getdevice_type()
{
    test_class->set_device_name ("Moter");
    QCOMPARE(test_class->get_device_name (), QString("Moter"));
}

void Unit_test_class::device_get_device_gpio()
{
    test_class->set_device_gpio (10);
    QCOMPARE(test_class->get_device_gpio (), uint(10));

}

void Unit_test_class::device_get_device_pid()
{
    test_class->set_device_pid ("710");
    QCOMPARE(test_class->get_device_pid (), QString("710"));

}

void Unit_test_class::device_get_identify_mobile_number()
{
    test_class->set_identify_mobile_number ("9999-9999");
    QCOMPARE(test_class->get_identify_mobile_number (), QString("9999-9999"));

}

void Unit_test_class::device_set_moter_position()
{
    QCOMPARE(test_class->start_active (), true);
    QCOMPARE(test_class->set_moter_position (20), true);
    QCOMPARE(test_class->stop_active (), true);
}

void Unit_test_class::device_get_moter_position()
{
    QCOMPARE(test_class->start_active (), true);
    QCOMPARE(test_class->set_moter_position (20), true);
    QCOMPARE(test_class->get_moter_position (), 20);
    QCOMPARE(test_class->stop_active (), true);
}

void Unit_test_class::init_raspberry_control_test()
{
    QCOMPARE(control->init_raspberry__ (), 0);
}

void Unit_test_class::init_check_raspberry_device_test()
{
    QSqlDatabase db;
    if (QSqlDatabase::contains ("Device_List_Connection")){
        db = QSqlDatabase::database ("Device_List_Connection");
    }
    else{
        db.addDatabase ("QSQLITE");
        db.setDatabaseName ("Device_list.db");
    }

    if (db.open () != true){ qDebug()<<db.lastError ();}

    QCOMPARE(db.open (), true);
    QSqlQuery query(db);

    QCOMPARE(query.exec ("SELECT * FROM `Device_list`;"), true);

    if (query.record ().count () == 0 ){
        QCOMPARE(query.exec ("INSERT INTO `Device_list`(`device_type`,`device_name`,`device_pid`,`device_hash`,`access_mobile_number`,`device_active`)"
                             "VALUES(\"Moter \", \"helloMoter1 \",\"2\",\"h1234\",0,1);"), true);
    }

    QCOMPARE(control->check_raspberry_device__ (), 0);
}

/*void Unit_test_class::add_raspberry_device_test()
{
    QCOMPARE(control->add_raspberry_device__ (), 0);
    //std::cin.read ("Moter",strlen("Moter"));
    //std::cin.read ("Moter_device",strlen("Moter_device"));
    //std::cin.read ("17",strlen("17"));

}*/

void Unit_test_class::remove_raspberry_device_test()
{
    QSqlDatabase db;
    if (QSqlDatabase::contains ("Device_List_Connection")){
        db = QSqlDatabase::database ("Device_List_Connection");
    }
    else{
        db.addDatabase ("QSQLITE", "Device_List_Connection");
        db.setDatabaseName ("Device_list.db");
    }


    QCOMPARE(db.open (), true);
    QSqlQuery query(db);

    QCOMPARE(query.exec ("SELECT * FROM `Device_list`"), true);

    query.last ();

    if ((query.at () + 1) == 0){
        QCOMPARE(query.exec ("INSERT INTO `Device_list`(`device_type`,`device_name`,`device_pid`,`device_hash`,`access_mobile_number`,`device_active`)"
                             "VALUES(\"Moter \", \"helloMoter1 \",\"2\",\"h1234\",0,1);"), true);
    }

    QCOMPARE(control->remove_raspberry_device__ (), 0);
}

void Unit_test_class::show_device_list()
{

    QCOMPARE(control->show_device_list__ (), 0);
}

void Unit_test_class::show_device_info()
{
    QCOMPARE(control->show_device_info__ (), 0);
}

QTEST_MAIN(Unit_test_class)
#include "unit_test_class.moc"
