#include "adduser.h"
#include "ui_adduser.h"

AddUser::AddUser(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::AddUser)
{
    ui->setupUi(this);
}

AddUser::~AddUser()
{
    delete ui;
}

void AddUser::on_lineEdit_returnPressed()
{
    QString qlogin = ui->lineEdit->text();
    if (qlogin.length() !=0)
    {
        std::string login = qlogin.toStdString();
        srand(time(0));
        std::string guest_pass;
        for (unsigned int i = 0; i < 8; ++i)
        {
            guest_pass.push_back((char)(48+rand()%74));
        }
        QString qpass = QString::fromStdString(guest_pass);
        QByteArray hex_pass_ba = qpass.toUtf8();
        QString hex_pass_qs(QCryptographicHash::hash(hex_pass_ba, QCryptographicHash::Sha3_512).toHex());
        guest_pass = hex_pass_qs.toStdString();
        ui->lineEdit_2->setText(qpass);
        emit add_new(login, guest_pass);
    }
}

void AddUser::on_pushButton_6_clicked()
{
    this->close();
}
