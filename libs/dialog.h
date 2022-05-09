#pragma once

#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QVariant>
#include "database.h"

class Dialog{
    private:
        Database database;
        Database::Session session;

        QString input(FILE* istream=stdin,FILE* ostream=stdout,bool acceptEmpty = false){
            // обьявляем потоки ввода и вывода в консоль
            // initialization input and output console stream
            QTextStream cin(istream);
            QTextStream cout(ostream);

            QString value; 

            while (value.isEmpty()){
                cout << "> "; cout.flush();
                value = cin.readLine();

                if (acceptEmpty == true){
                     break;
                }
            }
            
            return value;
        
        }
    
    public:
        QVariant get_data(QString data){
            QVariant value;
            
            if (data == "group"){
                value = QVariant(group_selected());
            }
            else if (data == "course"){
                value = QVariant(course_selected());
            }
            else if (data == "date"){
                value = QVariant(date_selected());
            }        
            else if (data == "name"){
                value = QVariant(custom_input("--> Ввведите имя студента:"));        
            }
            else {
                value = QVariant(custom_input("--> Ввведите необходимые данные для аргумента "+data));    
            }
            //qDebug() << QVariant(123);
            
            return value;
        }

        QString custom_input(QString msg = "--> Введите необходимые данные:"){
            qInfo().noquote() << msg; 
            return input();
        }

        bool choice_variant(QString msg = "--> Вы хотите совершить действие",QMap <QString,bool> variant_dict = {{"Y",true},{"n",false}}){
            QString variant;
            qInfo().noquote() << msg << variant_dict.keys() << ":";

            while (true)
            {
                variant = input();
                
                if (variant_dict[variant] == true || variant_dict[variant] == false){
                    break;
                }
            }

            return variant_dict[variant];
            

        }

        QString course_selected(){
            QStringList listdir = QDir(database.COURSES_PATH).entryList(QDir::Files);

            if (listdir.isEmpty()){
                return "no databases found";
            }
            
            qInfo().nospace() << "--> " << "Выберите курс " << "(1-" << listdir.size() << "):";
            
            for (int i = 0;i<listdir.size();i++){
                    qDebug().noquote() << i+1 << "-" << QFileInfo(listdir[i]).baseName();
            }
            
            return QFileInfo(listdir[input(stdin,stdout,false).toInt()-1]).baseName();
        }

        QString group_selected(){
            QStringList listdir = QDir(database.GROUPS_PATH).entryList(QDir::Files);
            
            if (listdir.isEmpty()){
                return "no databases found";
            }

            qInfo().nospace() << "--> " << "Выберите группу " << "(1-" << listdir.size() << "):";

            for (int i = 0;i<listdir.size();i++){
                qDebug().noquote() << i+1 << "-" << QFileInfo(listdir[i]).baseName();
                }
            
            return QFileInfo(listdir[input().toInt()-1]).baseName();

        }

        QList <int> student_selected(QString filename){
            session = database.connect(filename,database.GROUPS_PATH,"json");
            QJsonArray student_list = session.get_students();

            qInfo().nospace() << "--> " << "Выберите студентов через пробел " << "(1-" << student_list.size() << "):";

            
            for (int i = 0;i<student_list.size();i++){
                qDebug().noquote() << i+1 << "-" << student_list[i].toObject().value("name").toString();
            }

            QList <int> selected_students_id;
            QStringList selected_students = input(stdin,stdout,false).split(" ");
            
            for (int i = 0;i<student_list.size();i++){
                selected_students_id << selected_students[i].toInt();
            }

            return selected_students_id;

        }

        QDate date_selected(){
            qInfo().nospace() << "--> " << "Введите дату (пустая строка,если текущая дата):";
            
            QString date_string = input(stdin,stdout,true);

            if (date_string.isEmpty()){
                return QDate::currentDate();
            }

            return QDate::fromString(date_string,"dd/MM/yyyy");

            }









};
