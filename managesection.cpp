#include "managesection.h"
#include "ui_managesection.h"

ManageSection::ManageSection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageSection)
{
    ui->setupUi(this);
}

ManageSection::~ManageSection()
{
    delete ui;
}

void ManageSection::on_add_section_clicked()
{
    if (my_db.is_section_busy(ui->value->text()) == true)
    {
        QMessageBox::information(this, "Ошибка", "Такой раздел уже существует");
    }
    else if (ui->value->text() == "")
    {
        QMessageBox::information(this, "Ошибка", "Вы не заполнили название раздела");
    }
    else
    {
        my_db.add_section(ui->value->text());
        update_ui();
    }
}


void ManageSection::on_del_section_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->list->selectedItems();
    for (auto item : selectedItems) {
        my_db.remove_section(item->text());
    }

    update_ui();
}


void ManageSection::on_back_clicked()
{
    this->close();
}

void ManageSection::create_db()
{
    my_db.connect_to_database();
    update_ui();
}

void ManageSection::update_ui()
{
    ui->list->clear();
    ui->value->setText("");

    for (auto section : my_db.get_sections())
    {
        ui->list->addItem(section);
    }
}
