#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::set()
{
    _ip1= ui->spinBox->value();
    _ip2= ui->spinBox_2->value();
    _ip3= ui->spinBox_3->value();
    _ip4= ui->spinBox_4->value();
    _port=ui->spinBox_5->value();
}
void Settings::on_spinBox_editingFinished()
{
    set();
}

void Settings::on_spinBox_2_editingFinished()
{
    set();
}

void Settings::on_spinBox_3_editingFinished()
{
    set();
}

void Settings::on_spinBox_4_editingFinished()
{
    set();
}

void Settings::on_spinBox_5_editingFinished()
{
    set();
}

void Settings::on_pushButton_clicked()
{
    std::stringstream ip;
    ip << _ip1 << "." << _ip2 << "." << _ip3 << "." << _ip4;
    std::stringstream port;
    port << _port;
    ChatWindow * chat = new ChatWindow(ip.str(), port.str());
    this->close();
}
