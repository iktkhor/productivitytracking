#ifndef MANAGESECTION_H
#define MANAGESECTION_H

#include <QDialog>
#include <QMessageBox>
#include <set>
#include "database.h"

namespace Ui {
class ManageSection;
}

class ManageSection : public QDialog
{
    Q_OBJECT

public:
    explicit ManageSection(QWidget *parent = nullptr);
    ~ManageSection();

public slots:
    void create_db();
    void update_ui();

private slots:
    void on_add_section_clicked();

    void on_del_section_clicked();

    void on_back_clicked();

private:
    Ui::ManageSection *ui;
    DataBase my_db;
};

#endif // MANAGESECTION_H
