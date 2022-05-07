#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->table_active_tasks->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table_active_tasks->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->stackedWidget->setCurrentIndex(1);
    ui->change_table->setText("Завершенные задачи");

    ui->table_complete_tasks->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table_complete_tasks->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    my_db.connect_to_database();

    log_in_menu = new LogIn(this);
    manage_section = new ManageSection(this);
    add_task = new AddTask(this);
    timer = new QTimer(this);

    sec = 0;
    min = 0;
    flag = false;

    ui->pause_execution->setEnabled(false);
    ui->end_execution->setEnabled(false);

    connect(this, &MainWindow::create_db, log_in_menu, &LogIn::create_db);
    connect(this, &MainWindow::create_db, add_task, &AddTask::create_db);
    connect(this, &MainWindow::create_db, manage_section, &ManageSection::create_db);
    connect(this, &MainWindow::send_user, add_task, &AddTask::recieve_user);
    connect(log_in_menu, &LogIn::send_user, this, &MainWindow::recieve_user);
    connect(log_in_menu, &LogIn::enable_buttons, this, &MainWindow::set_enabled_buttons);   
    connect(add_task, &AddTask::update, this, &MainWindow::refresh_active_tasks_table);
    connect(timer, &QTimer::timeout, this, &MainWindow::timer_slot);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh_active_tasks_table()
{
    active_tasks = my_db.get_tasks(current_user->get_login(), 0);
    sort_tasks();
    ui->table_active_tasks->setRowCount(0);

    for (auto task : active_tasks)
    {
        int ind = ui->table_active_tasks->rowCount();

        ui->table_active_tasks->insertRow(ind);
        ui->table_active_tasks->setItem(ind, 0, new QTableWidgetItem(my_db.get_section(task.get_section_id())));
        ui->table_active_tasks->setItem(ind, 1, new QTableWidgetItem(task.get_value()));
    }

    refresh_current_task();
}

void MainWindow::refresh_complete_tasks_table()
{
    complete_tasks = my_db.get_tasks(current_user->get_login(), 1);
    ui->table_complete_tasks->setRowCount(0);

    for (auto task : complete_tasks)
    {
        int ind = ui->table_complete_tasks->rowCount();

        ui->table_complete_tasks->insertRow(ind);
        ui->table_complete_tasks->setItem(ind, 0, new QTableWidgetItem(my_db.get_section(task.get_section_id())));
        ui->table_complete_tasks->setItem(ind, 1, new QTableWidgetItem(task.get_value()));
        ui->table_complete_tasks->setItem(ind, 2, new QTableWidgetItem(my_db.get_task_time(task.get_id())));
    }
}

void MainWindow::refresh_current_task()
{
    ui->task_label->setText(my_db.get_section(active_tasks[0].get_section_id()) + ": " + active_tasks[0].get_value());
}

void MainWindow::on_delete_task_clicked()
{
    if (!is_log_in())
    {
        return;
    }

    if (ui->stackedWidget->currentIndex() == 1)
    {
        QList<QTableWidgetItem*> selectedItems = ui->table_active_tasks->selectedItems();
        std::set<int> selected_rows;

        for (auto item : selectedItems) {
            selected_rows.insert(item->row());
        }

        for (auto row : selected_rows) {
            auto id = active_tasks[row].get_id();
            my_db.remove_task(id);
        }

        refresh_active_tasks_table();
    }
    else
    {
        QList<QTableWidgetItem*> selectedItems = ui->table_complete_tasks->selectedItems();
        std::set<int> selected_rows;

        for (auto item : selectedItems) {
            selected_rows.insert(item->row());
        }

        for (auto row : selected_rows) {
            auto id = complete_tasks[row].get_id();
            my_db.remove_task(id);
        }

        refresh_complete_tasks_table();
    }
}

void MainWindow::on_add_task_clicked()
{
    if (is_log_in())
    {
        emit create_db();
        emit send_user(current_user);
        add_task->show();
    }
}

void MainWindow::on_manage_sections_clicked()
{
    if (is_log_in())
    {
        emit create_db();
        manage_section->show();
    }
}

void MainWindow::on_start_execution_clicked()
{
    if (!is_log_in())
    {
        return;
    }

    if (!flag)
    {
        ui->pause_execution->setEnabled(true);
        ui->end_execution->setEnabled(true);
        ui->start_execution->setEnabled(false);

        flag = true;
        timer->start(1000);
    }
}

void MainWindow::on_pause_execution_clicked()
{
    if (!is_log_in())
    {
        return;
    }

    if (flag)
    {
        ui->start_execution->setEnabled(true);
        ui->pause_execution->setEnabled(false);
        ui->start_execution->setText("Продолжить");
        flag = false;
        timer->stop();
    }

}

void MainWindow::on_end_execution_clicked()
{
    if (!is_log_in())
    {
        return;
    }

    if (flag)
    {
        timer->stop();
        QString time = QString::number(min) + " : " + QString::number(sec);
        sec = 0;
        min = 0;

        ui->min_label->setText(QString::number(min) + " ");
        ui->sec_label->setText(": " + QString::number(sec));
        ui->start_execution->setEnabled(true);
        ui->start_execution->setText("Начать");

        my_db.change_is_done(active_tasks[0].get_id(), 1);
        my_db.change_time(active_tasks[0].get_id(), time);
        refresh_active_tasks_table();
    }
}

void MainWindow::on_change_table_clicked()
{
    if (!is_log_in())
    {
        return;
    }

    QString s;
    bool b;
    int curr_ind(0);

    if (ui->stackedWidget->currentIndex() == 1)
    {
        s = "Активные задачи";
        curr_ind = 0;
        b = false;
        refresh_complete_tasks_table();
    }
    else
    {
        s = "Завершенные задачи";
        curr_ind = 1;
        b = true;
    }

    ui->stackedWidget->setCurrentIndex(curr_ind);
    ui->change_table->setText(s);

    ui->up->setEnabled(b);
    ui->down->setEnabled(b);
    ui->start_execution->setEnabled(b);
}

void MainWindow::on_up_clicked()
{
    if (!is_log_in())
    {
        return;
    }

    QList<QTableWidgetItem*> selectedItems = ui->table_active_tasks->selectedItems();
    std::set<int> selected_rows;

    for (auto item : selectedItems) {
        selected_rows.insert(item->row());
    }

    for (auto row : selected_rows) {
        if (row != 0) {
            auto id = active_tasks[row].get_id();
            auto curr_spot = active_tasks[row].get_current_spot();
            my_db.change_current_spot(id, curr_spot - 1);
            my_db.change_current_spot(active_tasks[row - 1].get_id(), curr_spot);
            refresh_active_tasks_table();
        }
        else
        {
            qDebug() << "First item cant up more";
        }
    }

    refresh_active_tasks_table();
}

void MainWindow::on_down_clicked()
{
    if (!is_log_in())
    {
        return;
    }

    QList<QTableWidgetItem*> selectedItems = ui->table_active_tasks->selectedItems();
    std::set<int> selected_rows;

    for (auto item : selectedItems) {
        selected_rows.insert(item->row());
    }

    for (auto row : selected_rows) {
        if (row < active_tasks.size() - 1) {
            auto id = active_tasks[row].get_id();
            auto curr_spot = active_tasks[row].get_current_spot();
            my_db.change_current_spot(id, curr_spot + 1);
            my_db.change_current_spot(active_tasks[row + 1].get_id(), curr_spot);
            refresh_active_tasks_table();
        }
        else
        {
            qDebug() << "Last item cant down more";
        }
    }

    refresh_active_tasks_table();
}

void MainWindow::on_log_in_clicked()
{
    emit create_db();
    log_in_menu->show();
}

void MainWindow::recieve_user(User* user)
{
    current_user = user;
    qDebug() << my_db.get_active_tasks_count(my_db.get_user_id(current_user->get_login()), 0);
    if (my_db.get_active_tasks_count(my_db.get_user_id(current_user->get_login()), 0) > 0)
    {
       refresh_active_tasks_table();
    }
}

bool MainWindow::is_log_in()
{
    if (current_user != nullptr)
    {
        return true;
    }
    else
    {
        QMessageBox::information(this, "Ошибка", "Сначала нужно войти в аккаунт");
        set_enabled_buttons(false);
        return false;
    }
}

void MainWindow::set_enabled_buttons(bool b)
{
    ui->delete_task->setEnabled(b);
    ui->add_task->setEnabled(b);
    ui->manage_sections->setEnabled(b);
    ui->start_execution->setEnabled(b);
    ui->change_table->setEnabled(b);
    ui->up->setEnabled(b);
    ui->down->setEnabled(b);
}

void MainWindow::sort_tasks()
{
    for(int i(0); i < active_tasks.size(); i++)
    {
        for (int j(0); j < active_tasks.size() - 1 - i; j++)
        {
            if (active_tasks[j].get_current_spot() > active_tasks[j + 1].get_current_spot())
            {
                Task temp = active_tasks[j];
                active_tasks[j] = active_tasks[j + 1];
                active_tasks[j + 1] = temp;
            }
        }
    }
}

void MainWindow::timer_slot()
{
    sec++;

    if(sec >=60)
    {
        sec = 0;
        min++;
    }

    ui->min_label->setText(QString::number(min) + " ");
    ui->sec_label->setText(": " + QString::number(sec));
}

void MainWindow::on_table_active_tasks_cellClicked(int row, int column)
{
    ui->table_active_tasks->selectRow(row);
}


void MainWindow::on_table_complete_tasks_cellClicked(int row, int column)
{
    ui->table_complete_tasks->selectRow(row);
}

