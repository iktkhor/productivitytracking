#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QMessageBox>
#include "database.h"

namespace Ui {
class LogIn;
}

class LogIn : public QDialog
{
    Q_OBJECT

public:
    explicit LogIn(QWidget *parent = nullptr);
    ~LogIn();

signals:
    void send_user(User*);
    void enable_buttons(bool);

public slots:
    void create_db();

private slots:
    void on_log_in_clicked();

    void on_registrate_clicked();

private:
    Ui::LogIn *ui;
    DataBase my_db;

private:
    bool check_fields();
};

#endif // LOGIN_H
