// 

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QDir>
#include <QDate>

#include "libs/dialog.h"
#include "libs/database.h"

int main(int argc,char *argv[]){

    Database database;
    Dialog dialog;

    Database::Session session;
                    
    QCoreApplication app(argc, argv);
    
    // конфигурация парсинга аргументов коммандной строки
    QCommandLineParser parser;
    parser.addPositionalArgument("command","Execute the command to work with the journal","command [options]");
    parser.addHelpOption();
    
    parser.parse(QCoreApplication::arguments());
    
    // получаем первый позиционный аргумент
    const QStringList args = parser.positionalArguments();
    const QString command = args.isEmpty() ? QString() : args.first();
    
    // устанавливаем опции для позиционных аргументов
    if (command == "report"){

    }
    else if (command == "add-progress" || command == "apr"){
          parser.clearPositionalArguments();
          QList <QCommandLineOption> options = {QCommandLineOption(QStringList() << "c" << "course","Selected course.","course"),
                                                QCommandLineOption(QStringList() << "d" << "date","Selected date","date"),
                                                QCommandLineOption(QStringList() << "n" << "name","Selected student name","student name"),
                                                QCommandLineOption(QStringList() << "cr" << "criteria","Selected criteria","<attendance, plus, test>")
                                };
          
          parser.addOptions(options);
          
          if (parser.parse(QCoreApplication::arguments()) == false){
                   qInfo() << "usage:" << argv[0] << "[command][options]";
                   qInfo() << argv[0] << "error:" << parser.errorText().toLower();
                   exit(1);
          }

          QStringList ignored_argv = {"name"};
          QMap <QString,QString> argv;

            for (int i = 0;i<options.size();i++){
                if (parser.value(options[i]).isEmpty() && ignored_argv.contains(options[i].names()[1]) != true){
                        QVariant value = dialog.get_data(options[i].names()[1]);
                        argv[options[i].names()[1]] = value.toString();
                        }
                else {   
                    argv[options[i].names()[1]] = parser.value(options[i]);
                }
            }

            session = database.connect(argv.value("course"),database.COURSES_PATH,"json");

            if (argv.value("name").isEmpty()){
                QList <int> student_ids = dialog.student_selected(dialog.group_selected());
                
                for (int i = 0;i<student_ids.size();i++){
                    qDebug() << "[+]" << "writable progress:" << session.add_progress(student_ids[i],argv.value("course"),argv.value("date"),{{"ll","kek"}})[2].toString();
            }
            }
            }
                                                                                                                        
    else if (command == "add-student" || command == "ast"){
         
         parser.clearPositionalArguments();
         QList <QCommandLineOption> options = {QCommandLineOption(QStringList() << "c" << "course","Selected course.","course"),
                                               QCommandLineOption(QStringList() << "g" << "group","Selected course.","group name"),
                                               QCommandLineOption(QStringList() << "n" << "name","Selected student name","student name")
         };
         
         parser.addOptions(options);

         if (parser.parse(QCoreApplication::arguments()) == false){

                  qInfo() << "usage:" << argv[0] << "[command][options]";
                  qInfo() << argv[0] << "error:" << parser.errorText().toLower();
                  exit(1);
                  }

         QMap <QString,QString> argv;

         for (int i = 0;i<options.size();i++){
             if (parser.value(options[i]).isEmpty()){
                    QVariant value = dialog.get_data(options[i].names()[1]);
                    argv[options[i].names()[1]] = value.toString();
             }
             else{
                 argv[options[i].names()[1]] = parser.value(options[i]);
             }
         };

         session = database.connect(argv.value("course"),database.COURSES_PATH,"json");
         qDebug() << "[+]" << "writable course:" << session.add_course(session.get_next_id(),argv.value("group"))[2].toString();
         
         session = database.connect(argv.value("group"),database.GROUPS_PATH,"json");
         qDebug() << "[+]" << "writable student:" << session.add_student(session.get_next_id(),argv.value("name"))[2].toString();
                  
    }
    else if (command == "add-course"){
        parser.clearPositionalArguments();
        QList <QCommandLineOption> options = {QCommandLineOption(QStringList() << "c" << "course","Selected course.","course")};
        
        parser.addOptions(options);
        
        if (parser.parse(QCoreApplication::arguments()) == false){
                  qInfo() << "usage:" << argv[0] << "[command][options]";
                  qInfo() << argv[0] << "error:" << parser.errorText().toLower();
                  exit(1);
        }

        QMap <QString,QString> argv;

        if (parser.value("course").isEmpty()){
            argv["course"] = dialog.custom_input("--> Введите имя для нового курса:");
        }
        else{
            argv["course"] = parser.value("course");
        }
        
        if (database.is_database_exists(argv.value("course"),database.COURSES_PATH,"json")){
            qInfo() << "--> заданная база данных уже существует";
            return 1;
        }

        // Попытка подключится к базе данных курса
        // Примечание: при отсуствии базы данных, она будет создана автоматический  
        Database::Session course_session = database.connect(argv.value("course"),database.COURSES_PATH,"json");
    
        if (dialog.choice_variant("--> Желатете добавить новую группу?")){
            argv["group"] = dialog.custom_input("--> введите имя группы");
            session = database.connect(argv.value("group"),database.GROUPS_PATH,"json");
        }
        else {
            qInfo() << "--> Cохранено";
            return 0;
        }

        if (dialog.choice_variant("--> Желатете добавить студентов?")){
            while (true)
            {
                 qDebug() << "-->" << "результат записи на курс:" << course_session.add_course(course_session.get_next_id(),argv.value("group"))[2].toString();
                 qDebug() << "-->" << "результат добавления студента:" << session.add_student(session.get_next_id(),dialog.custom_input("--> Ввведите имя студента:"))[2].toString();
                 
                 if (dialog.choice_variant("--> Желатете добавить еще студентов?") == false){
                     break;
                 }
            }
        
        qInfo() << "--> Cохранено";
            
    }}
    else if (commnad == "remove-student" || "rst"){
         parser.clearPositionalArguments();
         QList <QCommandLineOption> options = {QCommandLineOption(QStringList() << "c" << "course","Selected course.","course"),
                                               QCommandLineOption(QStringList() << "g" << "group","Selected course.","group name"),
                                               QCommandLineOption(QStringList() << "n" << "name","Selected student name","student name")
         };
         

    }
    else {
        parser.showHelp();
    }
    
    return 0;
}
