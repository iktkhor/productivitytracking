#ifndef TASK_H
#define TASK_H

#include <QString>

class Task
{
public:
    Task();
    QString get_value();
    QString get_time();
    int get_id();
    int get_current_spot();
    int get_user_id();
    int get_section_id();
    bool get_is_done();

    void set_id(int id);
    void set_current_spot(int current_spot);
    void set_value(QString value);
    void set_time(QString time);
    void set_user_id(int user_id);
    void set_section_id(int section_id);
    void set_is_done(bool b);
private:
    QString value;
    QString time;
    int id;
    int current_spot;
    int user_id;
    int section_id;
    bool is_done;
};

#endif // TASK_H
