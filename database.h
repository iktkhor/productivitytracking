#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <iostream>
#include <vector>
#include "user.h"
#include "task.h"

#define DATABASE_NAME "ProdTracking.sqlite3"

class DataBase
{
public:
    DataBase();
    void connect_to_database();

    User* get_user(QString login);
    int get_user_id(QString login);
    std::vector<Task> get_tasks(QString login, int is_done);
    QString get_task_time(int id);
    int get_active_tasks_count(int user_id, int is_done);
    std::vector<QString> get_sections();
    int get_section(QString value);
    QString get_section(int id);
    void add_user(User* user);
    void add_section(QString value);
    void add_task(Task* task);
    void remove_section(QString value);
    void remove_task(int id);
    void change_current_spot(int id, int spot);
    void change_is_done(int id, int is_done);
    void change_time(int id, QString time);
    bool is_login_busy(QString login);
    bool is_login_data_right(QString login, QString password);
    bool is_section_busy(QString value);

private:
    QSqlDatabase my_db;

private:
    bool open_database();
    bool create_tables();
};



#endif // DATABASE_H
