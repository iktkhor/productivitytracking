#include "task.h"

Task::Task()
{

}

int Task::get_id()
{
    return id;
}

int Task::get_current_spot()
{
    return current_spot;
}

QString Task::get_value()
{
    return value;
}

QString Task::get_time()
{
    return time;
}

int Task::get_user_id()
{
    return user_id;
}

int Task::get_section_id()
{
    return section_id;
}

bool Task::get_is_done()
{
    return is_done;
}

void Task::set_id(int id)
{
    this->id = id;
}

void Task::set_current_spot(int current_spot)
{
    this->current_spot = current_spot;
}

void Task::set_value(QString value)
{
    this->value = value;
}

void Task::set_time(QString time)
{
    this->time = time;
}

void Task::set_user_id(int user_id)
{
    this->user_id = user_id;
}

void Task::set_section_id(int section_id)
{
    this->section_id = section_id;
}

void Task::set_is_done(bool b)
{
    this->is_done = b;
}
