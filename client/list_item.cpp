#include "list_item.h"
#include "ui_list_item.h"

list_item::list_item(QString name, QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint),
    ui(new Ui::list_item),
    _name(name)
{
    ui->setupUi(this);
    ui->label->setText(_name);
}

list_item::~list_item()
{
    delete ui;
}
QSize list_item::size()
{
    return ui->label->size();
}
