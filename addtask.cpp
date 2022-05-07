#include "addtask.h"
#include "ui_addtask.h"

AddTask::AddTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTask)
{
    ui->setupUi(this);
}

AddTask::~AddTask()
{
    delete ui;
}

void AddTask::on_add_task_clicked()
{
    if (ui->value->text() != "")
    {
        Task *new_task = new Task();
        int sections_id = my_db.get_section(ui->sections->currentText());
        new_task->set_value(ui->value->text());
        new_task->set_user_id(my_db.get_user_id(current_user->get_login()));
        new_task->set_section_id(ui->sections->currentIndex() == 0 ? 0 : sections_id);
        new_task->set_current_spot(my_db.get_active_tasks_count(my_db.get_user_id(current_user->get_login()), 0));
        new_task->set_is_done(0);
        my_db.add_task(new_task);
    }

    emit update();
    update_ui();
}

void AddTask::on_back_clicked()
{
    this->close();
}

void AddTask::create_db()
{
    my_db.connect_to_database();
    update_ui();
}

void AddTask::update_ui()
{
    ui->sections->clear();
    ui->sections->addItem("");
    ui->value->setText("");

    for (auto section : my_db.get_sections())
    {
        ui->sections->addItem(section);
    }
}

void AddTask::recieve_user(User* user)
{
    current_user = user;
}
