#include "database.h"

DataBase::DataBase()
{

}

void DataBase::connect_to_database()
{
    if (QFile(QCoreApplication::applicationDirPath() + "/" DATABASE_NAME).exists())
    {
        this->open_database();
    } else
    {
        open_database();
        create_tables();
    }
}

bool DataBase::open_database()
{
    my_db = QSqlDatabase::addDatabase("QSQLITE");
    my_db.setDatabaseName(QCoreApplication::applicationDirPath() + "/" DATABASE_NAME);

    if (my_db.open())
    {
        return true;
    } else
    {
        return false;
    }
}

bool DataBase::create_tables()
{
    QSqlQuery create_users;
    QSqlQuery create_sections;
    QSqlQuery create_tasks;

    my_db.exec("PRAGMA foreign_keys = ON");

    if (!create_users.exec("CREATE TABLE Users ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "login VARCHAR(20) NOT NULL, "
                           "password VARCHAR(20) NOT NULL)"))
    {
        qDebug() << "error create table users";
        qDebug() << create_users.lastError().text();
        return false;
    }

    if (!create_sections.exec("CREATE TABLE Sections "
                              "("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "value VARCHAR(20) NOT NULL"
                              ")"))
    {
        qDebug() << "error create table sections";
        qDebug() << create_sections.lastError().text();
        return false;
    }

    if (!create_tasks.exec("CREATE TABLE Tasks ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                          "current_spot INTEGER, "
                          "value VARCHAR(255) NOT NULL, "
                          "user_id INTEGER NOT NULL, "
                          "section_id INTEGER, "
                          "is_done INTEGER, "
                          "time VARCHAR(10), "
                          "FOREIGN KEY (user_id) REFERENCES Users (id), "
                          "FOREIGN KEY (section_id) REFERENCES Sections (id))"))
    {
        qDebug() << "error create table tasks";
        qDebug() << create_users.lastError().text();
        return false;
    }

    return true;
}

User* DataBase::get_user(QString login)
{
    QSqlQuery get_user;
    get_user.prepare("SELECT * FROM Users WHERE login = (:log)");
    get_user.addBindValue(login);

    if (get_user.exec())
    {
        if (get_user.next())
        {
            User* user = new User();
            user->set_login(get_user.value(1).toString());
            user->set_password(get_user.value(2).toString());
            return user;
        }
    }
}

int DataBase::get_user_id(QString login)
{
    QSqlQuery get_user_id;
    get_user_id.prepare("SELECT id FROM Users WHERE login = (:log)");
    get_user_id.addBindValue(login);

    if (get_user_id.exec())
    {
        if (get_user_id.next())
        {
            return get_user_id.value(0).toInt();
        }

        return 0;
    }
}

std::vector<Task> DataBase::get_tasks(QString login, int is_done)
{
    QSqlQuery get_tasks;
    std::vector<Task> tasks;
    get_tasks.prepare("SELECT * FROM Tasks WHERE user_id = (:us_id) AND is_done = (:isd)");
    get_tasks.addBindValue(get_user_id(login));
    get_tasks.addBindValue(is_done);

    if (get_tasks.exec())
    {
        while (get_tasks.next())
        {
            Task task = Task();
            task.set_id(get_tasks.value(0).toInt());
            task.set_current_spot(get_tasks.value(1).toInt());
            task.set_value(get_tasks.value(2).toString());
            task.set_user_id(get_tasks.value(3).toInt());
            task.set_section_id(get_tasks.value(4).toInt());
            task.set_is_done(get_tasks.value(5).toInt());
            tasks.push_back(task);
        }

        return tasks;
    }
}

QString DataBase::get_task_time(int id)
{
    QSqlQuery get_task_time;
    get_task_time.prepare("SELECT time FROM Tasks WHERE id = (:id)");
    get_task_time.addBindValue(id);

    if (get_task_time.exec())
    {
        if (get_task_time.next())
        {
            return get_task_time.value(0).toString();
        }
    }
}

int DataBase::get_active_tasks_count(int user_id, int is_done)
{
    QSqlQuery get_tasks_count;
    get_tasks_count.prepare("SELECT COUNT(*) FROM Tasks WHERE user_id = (:ui) AND is_done = (:isd)");
    get_tasks_count.addBindValue(user_id);
    get_tasks_count.addBindValue(is_done);

    if (get_tasks_count.exec())
    {
        if (get_tasks_count.next())
        {
            return get_tasks_count.value(0).toInt();
        }
    }
    else
    {
        qDebug() << "error get tasks count";
        qDebug() << get_tasks_count.lastError().text();
    }
}

std::vector<QString> DataBase::get_sections()
{
    QSqlQuery get_sections;
    std::vector<QString> sections;
    get_sections.prepare("SELECT value FROM Sections");

    if (get_sections.exec())
    {
        while (get_sections.next())
        {
            sections.push_back(get_sections.value(0).toString());
        }

        return sections;
    }
    else
    {
        qDebug() << "error get sections";
        qDebug() << get_sections.lastError().text();
    }
}

int DataBase::get_section(QString value)
{
    QSqlQuery get_section;
    get_section.prepare("SELECT id FROM Sections WHERE value = (:vl)");
    get_section.addBindValue(value);

    if (get_section.exec())
    {
        if (get_section.next())
        {
            return get_section.value(0).toInt();
        }
    }
}

QString DataBase::get_section(int id)
{
    QSqlQuery get_section;
    get_section.prepare("SELECT value FROM Sections WHERE id = (:id)");
    get_section.addBindValue(id);

    if (get_section.exec())
    {
        if (get_section.next())
        {
            return get_section.value(0).toString();
        }
    }
}

void DataBase::add_user(User *user)
{
    QSqlQuery add_user;
    add_user.prepare("INSERT INTO Users(login, password) VALUES(:log, :pas)");
    add_user.addBindValue(user->get_login());
    add_user.addBindValue(user->get_password());

    if (!add_user.exec())
    {
        qDebug() << "error add user";
        qDebug() << add_user.lastError().text();
    }
}

void DataBase::add_section(QString value)
{
    QSqlQuery add_section;
    add_section.prepare("INSERT INTO Sections(value) VALUES(:vl)");
    add_section.addBindValue(value);

    if (!add_section.exec())
    {
        qDebug() << "error add section";
        qDebug() << add_section.lastError().text();
    }
}

void DataBase::add_task(Task *task)
{
    QSqlQuery add_task;
    int user_id = task->get_user_id();
    int section_id = task->get_section_id();
    add_task.prepare("INSERT INTO Tasks(value, current_spot, user_id, section_id, is_done) VALUES(:vl, :cs, :us_id, :sec_id, :isd)");
    add_task.addBindValue(task->get_value());
    add_task.addBindValue(task->get_current_spot());
    add_task.addBindValue(user_id == 0 ? NULL : user_id);
    add_task.addBindValue(section_id == 0 ? NULL : section_id);
    add_task.addBindValue(0);

    if (!add_task.exec())
    {
        qDebug() << "error add task";
        qDebug() << add_task.lastError().text();
    }
}

void DataBase::remove_section(QString value)
{
    QSqlQuery rem_sec;
    rem_sec.prepare("DELETE FROM Sections WHERE value = (:vl)");
    rem_sec.addBindValue(value);

    if (!rem_sec.exec())
    {
        qDebug() << "error section is not deleted";
        qDebug() << rem_sec.lastError().text();
    }
}

void DataBase::remove_task(int id)
{
    QSqlQuery rem_task;
    rem_task.prepare("DELETE FROM Tasks WHERE id = (:id)");
    rem_task.addBindValue(id);

    if (!rem_task.exec())
    {
        qDebug() << "error task is not deleted";
        qDebug() << rem_task.lastError().text();
    }
}

void DataBase::change_current_spot(int id, int spot)
{
    QSqlQuery change_spot;
    change_spot.prepare("UPDATE Tasks "
                        "SET current_spot = (:cs) "
                        "WHERE id = (:id)");

    change_spot.addBindValue(spot);
    change_spot.addBindValue(id);

    if (!change_spot.exec())
    {
        qDebug() << "error task is not deleted";
        qDebug() << change_spot.lastError().text();
    }
}

void DataBase::change_is_done(int id, int is_done)
{
    QSqlQuery change_is_done;
    change_is_done.prepare("UPDATE Tasks "
                           "SET is_done = (:isd) "
                           "WHERE id = (:id)");
    change_is_done.addBindValue(is_done);
    change_is_done.addBindValue(id);

    if (!change_is_done.exec())
    {
        qDebug() << "error change task is done";
        qDebug() << change_is_done.lastError().text();
    }
}

void DataBase::change_time(int id, QString time)
{
    QSqlQuery change_time;
    change_time.prepare("UPDATE Tasks "
                        "SET time = (:tm) "
                        "WHERE id = (:id)");
    change_time.addBindValue(time);
    change_time.addBindValue(id);

    if (!change_time.exec())
    {
        qDebug() << "error change task time";
        qDebug() << change_time.lastError().text();
    }
}

bool DataBase::is_login_busy(QString login)
{
    QSqlQuery is_busy;
    is_busy.prepare("SELECT * FROM Users WHERE login = (:log)");
    is_busy.addBindValue(login);

    if (is_busy.exec())
    {
        if (is_busy.next())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool DataBase::is_login_data_right(QString login, QString password)
{
    QSqlQuery is_right;
    is_right.prepare("SELECT * FROM Users WHERE login = (:log) AND password = (:pas)");
    is_right.addBindValue(login);
    is_right.addBindValue(password);

    if (is_right.exec())
    {
        if (is_right.next())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool DataBase::is_section_busy(QString value)
{
    QSqlQuery is_busy;
    is_busy.prepare("SELECT * FROM Sections WHERE value = (:vl)");
    is_busy.addBindValue(value);

    if (is_busy.exec())
    {
        if (is_busy.next())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
