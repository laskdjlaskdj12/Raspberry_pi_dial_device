#include "init_rasapberry.h"

init_rasapberry_control::init_rasapberry_control(QObject *parent) : QObject(parent){}

init_rasapberry_control::~init_rasapberry_control(){}


raspberry_control::raspberry_control(QObject *parent): init_rasapberry_control(parent)
{
    //db를 초기화하여 Device_list 테이블이 존재하는지 확인
    try{

        db = QSqlDatabase::addDatabase ("QSQLITE");
        db.setDatabaseName ("Device_list");

        if (db.open () == false){ throw db.lastError (); }

        QStringList device_table = db.tables ();

        foreach (const QString &str, device_table) {

            if ("Component_list" ==  str){   is_table_exsist = true;}
        }

        QSqlQuery db_query(db);

        if (!is_table_exsist){
            if (db_query.exec ("CREATE TABLE `Device_list` ("
                               "`device_type`	TEXT NOT NULL,"
                               "`device_name`	TEXT NOT NULL,"
                               "`device_pid`	TEXT NOT NULL,"
                               "`device_hash`	TEXT NOT NULL,"
                               "`identify_mobile_number`	TEXT,"
                               "`current_range`	INTEGER,"
                               "`device_gpio`	INTEGER,"
                               "`min_range`	INTEGER,"
                               "`max_range`	INTEGER,"
                               "`access_mobile_number`	INTEGER NOT NULL,"
                               "`device_active`	INTEGER NOT NULL,"
                               "PRIMARY KEY(`device_pid`));") != true){ throw db_query.lastError (); }


        }
        db_query.clear ();

    }catch(const QSqlError& e){
        qDebug()<<"[Error] : raspberry_control exception : "<<e.text ();
    }
}

raspberry_control::~raspberry_control()
{

}


int raspberry_control::init_raspberry ()
{
    try{

        QSqlQuery db_query(db);
        qDebug()<<"===================== RASPBERRY Device_states =====================";

        //if (wiringPiSetup () == -1){ throw "wiringPI_Setup is fail"; }

        if (db_query.exec ("SELECT * FROM `Device_list`;")!= true){     throw db_query.lastError (); }

        while(db_query.next ()){

            qDebug()<<"[Device] : "<< db_query.value(1).toString ()<<" : "<<db_query.value (2).toString () <<" : GPIO : "<<db_query.value(5);

            //pinMode (db_query.value (5).toInt (), INPUT);

        }

        qDebug()<<"================== success init_raspberry ==================";
        return 0;

    }catch(const QSqlError& e){
        qDebug()<<"[Error] : raspberry_control exception : "<<e.text ();
        qDebug()<<"================== fail init_raspberry ==================";

        return -1;

    }catch(const QString& e){
        qDebug()<<"[Error] : raspberry_control exception : "<<e;
        qDebug()<<"================== fail init_raspberry ==================";

        return -1;
    }
}

int raspberry_control::check_raspberry_device()
{
    try{

        QSqlQuery db_query(db);
        qDebug()<<"================== check_raspberry_device ==================";
        //라즈베리파이에 등록된 디바이스들의 갯수를 db에 찾아서 출력함

        if (db_query.exec ("SELECT COUNT(*) FROM `Device_list`;") != true){     throw db_query.lastError (); }

        device_list_size = db_query.value (0).toInt ();

        db_query.clear ();

        if (db_query.exec ("SELECT * FROM `Device_list`;")!= true){     throw db_query.lastError (); }

        while(db_query.next ()){
            qDebug()<<"==========================================";
            qDebug()<<"[Device_Name] : "<<db_query.value(1).toString ();
            qDebug()<<"[Device_Type] : "<<db_query.value (0).toString ();
            qDebug()<<"[Device_pid ] : "<<db_query.value (2).toString ();
            qDebug()<<"[Device_hash] : "<<db_query.value (3).toString ();
            qDebug()<<"[Device_owner_number] : "<<db_query.value (4).toString ();
            qDebug()<<"\n\n\n";
        }

        db_query.clear ();

        return 0;

    }catch(const QSqlError& e){
        qDebug()<<"[Error] : raspberry_control exception : "<<e.text ();
        return -1;
    }
}

int raspberry_control::add_rasbperry_device()
{
    try{

        QTextStream cin(stdin);
        QTextStream cout(stdin);
        QString Device_type;
        bool        loop = false;

        QSqlQuery db_query(db);
        qDebug()<<"===================== RASPBERRY add_Devcie =====================";
        qDebug()<<"Before edit device module please sure check GPIO pine is clearly connected by gpio";
        qDebug()<<"And check GPIO pine number is wiringPI pine";
        qDebug()<<"\n\n\n\n";


        //Device_type
        while(loop){

            qDebug()<<"[Device_Type] ";
            Device_type.clear ();
            cin >> Device_type;

            //디바이스 타입에 따라 메모리 할당됨
            //현재는 모터 종류만 됨
            if (Device_type != "Moter"){
                qDebug()<<"[Warning] : only Moter Device can add raspberry pi device";
                qDebug()<<"\n\n\n\n\n\n";
                cin.flush ();
                loop = true;
            }

            //만약 Moter때는 false로 해서 루프를 false문으로 함
            else{
                loop = false;
            }

        }

        //device_class에 Moter를 할당하여 moter를 가동함
        Device_class* device_class = new Moter;
        device_class->set_device_type (Device_type);


        //Device_name
        QString device_name;
        cin.flush ();
        qDebug()<<"[Device_Name]";
        cin>> device_name;
        device_class->set_device_name (device_name);


        //Device_gpio
        int device_gpio = 0;
        cin.flush ();
        qDebug()<<"[Devcie_gpio]";
        cin>> device_gpio;
        device_class->set_device_gpio (device_gpio);


        //Device_pid

        //pid는 Create_pid() 로 직접 생성
        QString pid = Create_pid ();
        device_class->set_device_pid (pid);

        //sql Device_hash
        QCryptographicHash Device_hash_gen(QCryptographicHash::Algorithm::Sha512);
        Device_hash_gen.addData (device_class->get_device_pid ().toUtf8 ());
        QByteArray device_hash_res = Device_hash_gen.result ();

        //input device_info into class
        db_query.clear ();

        db_query.prepare ("INSERT INTO `Device_list`(`device_type`,`device_name`,`device_pid`,`device_hash`,`identify_mobile_number`,`current_range`,`device_gpio`,`min_range`,`max_range`,`access_mobile_number`,`device_active`)"
                          "VALUES (:type, :name, :pid, :hash :identify_mobile, :current_range, :access_mobile, :device_active);");
        db_query.bindValue (":type", device_class->get_device_type ());
        db_query.bindValue (":name", device_class->get_device_name ());
        db_query.bindValue (":pid", device_class->get_device_pid ());
        db_query.bindValue (":hash", device_hash_res);
        db_query.bindValue (":access_mobile", 0);
        db_query.bindValue (":device_active",(int)true);

        if (db_query.exec () != true){   throw db_query.lastError (); }


        qDebug()<<"===================== Congraturation  =====================";
        qDebug()<<"Raspberry device added successfuly";
        qDebug()<<"\n\n\n\n";

        delete device_class;

        return 0;



    }catch(const QSqlError& e){

        qDebug()<<"[Error] : add_raspberry_device exception : "<<e.text ();
        return -1;

    }
}

int raspberry_control::remove_raspberry_device()
{
    try{
        QTextStream cin(stdin);
        QTextStream cout(stdin);

        qDebug()<<"===================== RASPBERRY remove_Device =====================";
        qDebug()<<"Before edit device module please sure check GPIO pine is clearly connected by gpio";
        qDebug()<<"And check GPIO pine number is wiringPI pine";
        qDebug()<<"\n\n\n\n";

        do{
            qDebug()<<"Device_pid ";
            qDebug()<<"[Getout press C]";
            QString pid;
            cin.flush ();
            cin>>pid;
            if (pid == "c"){     break;}

            if (this->remove_device (pid.toInt ())){ break;}

        }while(true);

        return 0;

    }catch(const QSqlError& e){
        qDebug()<<"[Error] : add_raspberry_device exception : "<<e.text ();
        return -1;
    }
}

int raspberry_control::show_device_list()
{
    return this->init_raspberry ();
}

int raspberry_control::show_device_info()
{
    return this->check_raspberry_device ();
}


QString raspberry_control::Create_pid()
{
    QSqlQuery db_query(db);
    int rand = 0;
    do{
        rand = qrand();

        db_query.prepare ("SELECT * FROM `Device_list` WHERE `device_pid` = :pid ;");
        db_query.bindValue (":pid", QString::number (rand));

        if (db_query.exec () != true){   throw db_query.lastError ();}

        if (db_query.record ().count () != 0){
            continue;
        }

        else{

            break;
        }

    }while(true);

    return QString::number (rand);

}

bool raspberry_control::remove_device(int pid)
{
    try{
        QSqlQuery sql_query(db);

        sql_query.prepare ("SELECT * FROM `Device_list` WHERE `device_pid` = :pid ;");
        sql_query.bindValue (":pid", pid);

        if (sql_query.exec () != true){  throw sql_query.lastError ();}

        if (sql_query.record ().count () == 0){

            qDebug()<<"[Info] : NO SUCH PID";
            return false;
        }

        sql_query.prepare ("DELETE * FROM `Device_list` WHERE `device_pid` = :pid ;");
        sql_query.bindValue (":pid", pid);

        if (sql_query.exec () != true){  throw sql_query.lastError ();}

        qDebug()<<"[Info] : =================== Success delete ["<< pid <<"] ===================";

        return true;;

    }catch(const QSqlError& e){
        qDebug()<<"[Error] : add_raspberry_device exception : "<<e.text ();
        return false;
    }
}

// SLOT AREA
int raspberry_control::add_raspberry_device(QString d_name, QString Type, QString Device_owner_number, int gpio_number)
{
    try{

        QSqlQuery db_query(db);

        if (Type != "Moter"){
            throw "NO_MOTER";
        }

        Device_class* device_class = new Moter;

        device_class->set_device_gpio (gpio_number);
        device_class->set_device_name (d_name);
        device_class->set_device_type ("Moter");
        device_class->set_device_pid (Create_pid ());
        device_class->set_identify_mobile_number (Device_owner_number);


        //sql Device_hash
        QCryptographicHash Device_hash_gen(QCryptographicHash::Algorithm::Sha512);
        Device_hash_gen.addData (device_class->get_device_pid ().toUtf8 ());
        QByteArray device_hash_res = Device_hash_gen.result ();

        //input device_info into class
        db_query.clear ();

        db_query.prepare ("INSERT INTO `Device_list`(`device_type`,`device_name`,`device_pid`,`device_hash`,`identify_mobile_number`,`current_range`,`device_gpio`,`min_range`,`max_range`,`access_mobile_number`,`device_active`)"
                          "VALUES (:type, :name, :pid, :hash :identify_mobile, :current_range, :access_mobile, :device_active);");
        db_query.bindValue (":type", device_class->get_device_type ());
        db_query.bindValue (":name", device_class->get_device_name ());
        db_query.bindValue (":pid", device_class->get_device_pid ());
        db_query.bindValue (":hash", device_hash_res);
        db_query.bindValue (":access_mobile", device_class->get_access_mobile_number ());
        db_query.bindValue (":device_active",(int)true);

        if (db_query.exec () != true){   throw db_query.lastError (); }


        qDebug()<<"===================== Congraturation  =====================";
        qDebug()<<"Raspberry device added successfuly";
        qDebug()<<"\n\n\n\n";

        int pid = device_class->get_device_pid ().toInt ();
        delete device_class;

        return pid;

    }catch(const QSqlError& e){
        qDebug()<<"[Error] : add_raspberry_device exception : "<<e.text ();
        return -1;

    }catch(QString& e){
        QString mes = "[Error] : NO_Device : ";
        mes.append (e);
        return -1;
    }

}

int raspberry_control::remove_raspberry_device(QString pid)
{
    this->remove_device (pid.toInt ());
    return 0;
}
