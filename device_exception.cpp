#include "device_exception.h"

Device_Exception::Device_Exception(const QString &s, int _Line_, Error_type type ): type__(type)\
  ,err_str__(s)\
  ,line__(_Line_){}

QString Device_Exception::get_error_string()
{
    switch(type__){
    case Error:
        qDebug()<<"[Error] : "<<err_str__ << " : "<<line__;
        break;
    case Warning:
        qDebug()<<"[Warning] : "<<err_str__ << " : "<<line__;
        break;
    case Critical:
        qDebug()<<"[Critical] : "<<err_str__ << " : "<<line__;
        break;
    case None:
        qDebug()<<"[None] : "<<err_str__ << " : "<<line__;
        break;
    }
}

int Device_Exception::insert_db_error()
{
    try{
        bool table_exsist = false;
        QSqlDatabase db = db.addDatabase ("QSQLITE");

        if (db.isValid () == false){    throw db.lastError ();}

        db.setDatabaseName ("Error_Log");

        if (db.open () == false){   throw db.lastError ();}

        QSqlQuery error_query(db);

        QStringList table_list = db.tables ();

        foreach (const QString &str, table_list) {

            if ("Error_Log" ==  str){   table_exsist = true;}
        }

        if (!table_exsist){

            if ( error_query.exec ("CREATE TABLE `Error_Log` ("
                                  "`Error_type`  INTEGER,"
                                  "`Error_Line`	INTEGER,"
                                  "`Error_name`	TEXT);") != true){  throw error_query.lastError ();}

        }


        error_query.prepare ("INSERT  INTO `error`(`Error_Line`,`Error_Name`,`Error_Type`)"
                             "VALUES (:line,:name,:type)");

        error_query.bindValue (":line", line__);
        error_query.bindValue (":name", err_str__);
        error_query.bindValue (":type", type__);

        if (error_query.exec () != true){   throw error_query.lastError ();}

        return 0;


    }catch(QSqlError& e){
        qDebug()<<"[Error] : Device_Exception : "<<e.text ();
        return -1;
    }
}

