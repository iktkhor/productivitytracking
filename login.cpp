#include "login.h"
#include "ui_login.h"

LogIn::LogIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogIn)
{
    ui->setupUi(this);
}

LogIn::~LogIn()
{
    delete ui;
}

void LogIn::on_log_in_clicked()
{

    if (my_db.is_login_data_right(ui->lineEdit_log->text(), ui->lineEdit_pas->text()) == true)
    {
        emit send_user(my_db.get_user(ui->lineEdit_log->text()));
        emit enable_buttons(true);
        this->close();
    }
    else
    {
        QMessageBox::information(this, "Ошибка", "Пароль или логин неправильный");
    }

}

void LogIn::on_registrate_clicked()
{
    if (check_fields())
    {
        if (my_db.is_login_busy(ui->lineEdit_log->text()) == false)
        {
            User *new_user = new User();
            new_user->set_login(ui->lineEdit_log->text());
            new_user->set_password(ui->lineEdit_pas->text());
            my_db.add_user(new_user);

            this->close();
        }
        else
        {
            QMessageBox::information(this, "Ошибка", "Пользователь с таким логином уже существует");
        }
    }
}

bool LogIn::check_fields()
{
    if (ui->lineEdit_log->text().size() < 3 || ui->lineEdit_log->text().size() > 20)
    {
        QMessageBox::information(this, "Ошибка", "Логин должен быть размера 3-20 символов");
        return false;
    }
    if (ui->lineEdit_pas->text().size() < 5 || ui->lineEdit_pas->text().size() > 20)
    {
        QMessageBox::information(this, "Ошибка", "Пароль должен быть размера 5-20 символов");
        return false;
    }
    return true;
}

void LogIn::create_db()
{
    my_db.connect_to_database();
}
