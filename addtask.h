#ifndef ADDTASK_H
#define ADDTASK_H

#include <QDialog>
#include <QMessageBox>
#include "database.h"
#include "task.h"
#include "user.h"

namespace Ui {
class AddTask;
}

class AddTask : public QDialog
{
    Q_OBJECT

signals:
    void update_tasks();

public:
    explicit AddTask(QWidget *parent = nullptr);
    ~AddTask();

signals:
    void update();

public slots:
    void create_db();
    void update_ui();
    void recieve_user(User* user);

private slots:
    void on_add_task_clicked();

    void on_back_clicked();

private:
    Ui::AddTask *ui;
    DataBase my_db;
    User* current_user;
};

#endif // ADDTASK_H
