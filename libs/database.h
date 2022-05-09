#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSharedPointer>

#pragma once


class Database{
    public:
        class Session{
             private:
                 
                QFile* dbFile = new QFile();   
                
                // низокуровневые функции чтения и записи
                
                QJsonArray write(QJsonDocument document){
                    dbFile -> setFileName(dbFullPath);
                    
                    if(!dbFile -> open(QIODevice::WriteOnly))
                    {
                        return QJsonArray({false,dbFile -> error(),dbFile -> errorString()});
                    }
                    
                    dbFile -> write(document.toJson(QJsonDocument::Indented));
                    
                    dbFile -> close();
                    
                    return QJsonArray({true,0,"succesfull writable"});
                }

                QJsonDocument read(){
                    dbFile -> setFileName(dbFullPath);

                    if(dbFile -> open(QIODevice::ReadOnly))
                            {
                                QJsonDocument jsonDoc = QJsonDocument::fromJson(dbFile -> readAll());
                                dbFile -> close();
                                return jsonDoc;
                            }
                }
                
                // низкоуровневая функция создания сложных json структур

             public:
                QString dbName;
                QString dbDir;
                QString dbFormat;
                QString dbFullPath;
                                    
                void print_session_info(){  
                        qDebug() << "----------------------------------------------";
                        qDebug() << "[*]" << "database path:" << dbName;
                        qDebug() << "[*]" << "database path:" << dbFullPath;
                        qDebug() << "[*]" << "database format:" << dbFormat;
                        qDebug() << "----------------------------------------------";
                }


                QJsonArray add_progress(int id,QString course_name,QString date,QJsonObject data){
                    
                    QString strnum = QString::number(id);

                    QJsonDocument dbFileJson = read();                    
                    QJsonObject object{dbFileJson.object()};

                    object.insert("group",course_name);
                    
                    QJsonObject progress = object["progress"].toObject();
                    QJsonObject idobj = QJsonObject();

                    idobj.insert(date,data);
                    progress.insert(strnum,idobj);
                    object.insert("progress",progress);

                    dbFileJson.setObject(object);
                   
                    QJsonArray result = write(dbFileJson);
                    return result;
                    }
                    

                QJsonArray add_course(int id,QString course_name){
                    
                    QString strnum = QString::number(id);
                    
                    QJsonDocument dbFileJson = read();                    
                    QJsonObject object{dbFileJson.object()};

                    object.insert("group",course_name);

                    QJsonObject progress = object["progress"].toObject();
                    QJsonObject idobj = QJsonObject();

                    progress.insert(strnum,idobj);
                    
                    object.insert("progress",progress);

                    dbFileJson.setObject(object);

                    QJsonArray result = write(dbFileJson);
                    return result;   
                    
                    }

                QJsonArray add_student(int id,QString name){          
                    
                    QJsonDocument dbFileJson = read();                    
                    QJsonObject object{dbFileJson.object()};                                        
                    QJsonArray studentarr = object["students"].toArray();

                    // отлавливаем не уникальные id

                    for (int i=0;i<studentarr.size();i++){
                            if (studentarr[i].toObject().value("id").toInt()==id){                                
                                return QJsonArray({false,0,"student with the specified id already exists"});
                            }
                        }
                    
                    studentarr.push_back(QJsonObject{{"id",id},{"name",name}});
                    
                    object.insert("students",studentarr);
                    
                    dbFileJson.setObject(object);

                    QJsonArray result = write(dbFileJson);
                    return result;   
                                       
                }

                QJsonArray remove_student(int id){
                    int index = id-1;
                    
                    QJsonDocument dbFileJson = read();                    
                    QJsonObject object{dbFileJson.object()};                                        
                    QJsonArray studentarr = object["students"].toArray();

                    studentarr.removeAt(index);
                    
                    object.insert("students",studentarr);

                    dbFileJson.setObject(object);
                    QJsonArray result = write(dbFileJson);
                    return result;
                }

                QJsonArray get_students(){
                    QJsonDocument dbFileJson = read();                    
                    QJsonObject object{dbFileJson.object()};                                        
                    QJsonArray student_list = object["students"].toArray();
                    return student_list;


                }

                int get_next_id(){
                    QJsonDocument dbFileJson = read();                    
                    QJsonObject object{dbFileJson.object()};                                        
                    QJsonValue arr = object["students"];
                    
                    if (arr.isNull()){
                        QJsonValue arr = object["progress"];

                        if (arr.isNull()){
                            return 1;
                        } 
                        return arr.toObject().count()+1;           
                    }

                    return arr.toArray().count()+1;
                
                }
            
            
            };

        // установливаем стандартные пути для хранения данных        
        const QString HOME_PATH = getenv("HOME");
        const QString DATA_PATH = HOME_PATH+"/.students-rating/";
        const QString GROUPS_PATH = DATA_PATH+"/groups/";
        const QString COURSES_PATH = DATA_PATH+"/course/";

        // подключаемся к установленной базе данных

        Session connect(QString name,QString dir,QString format="json"){
            Session session;
            
            // принудительно создаем базу данных для избежания ошибок
            create_database(name,dir,format);

            session.dbName = name;
            session.dbDir = dir;
            session.dbFormat = format;
            session.dbFullPath = dir+name+"."+format;
        
            return session;

        }

        bool is_database_exists(QString name,QString dir,QString format="json"){
            QFile database_file(dir+name+"."+format);

            return database_file.exists();
        }
        
        // создаем пустую базу данных
        void create_database(QString name,QString dir,QString format="json"){
         
            if (!QDir().exists(dir)){
                  QDir().mkdir(dir);}
            
             QFile database_file(dir+name+"."+format);

             if (!database_file.exists()){
                    database_file.open(QIODevice::WriteOnly);
                    database_file.close();
                         
            }

        }
        void mkdir(QString dir){
               if (!QDir().exists(dir)){
                   QDir().mkdir(dir);}

        }
};


        

