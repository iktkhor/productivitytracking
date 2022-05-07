#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include "database.h"
#include "login.h"
#include "managesection.h"
#include "addtask.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void create_db();
    void update_ui();
    void send_user(User*);

private slots:
    void on_delete_task_clicked();

    void on_add_task_clicked();

    void on_start_execution_clicked();

    void on_end_execution_clicked();

    void on_change_table_clicked();

    void on_log_in_clicked();

    void on_manage_sections_clicked();

    void recieve_user(User* user);

    void timer_slot();

    void on_up_clicked();

    void on_down_clicked();

    void on_pause_execution_clicked();

    void on_table_active_tasks_cellClicked(int row, int column);

    void on_table_complete_tasks_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    DataBase my_db;
    User *current_user{ nullptr };
    LogIn *log_in_menu;
    AddTask *add_task;
    ManageSection *manage_section;
    QTimer *timer;
    int sec;
    int min;
    bool flag;
    std::vector<Task> active_tasks;
    std::vector<Task> complete_tasks;

    void refresh_active_tasks_table();
    void refresh_complete_tasks_table();
    void refresh_current_task();
    void set_enabled_buttons(bool b);
    void sort_tasks();
    bool is_log_in();
};
#endif // MAINWINDOW_H
