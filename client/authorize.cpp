#include "authorize.h"
#include "ui_authorize.h"

Authorize::Authorize(QDialog * parent) ://
    QDialog(parent),
    ui(new Ui::Authorize)
{
    ui->setupUi(this);
    this->setModal(true);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
}

Authorize::~Authorize()
{
    delete ui;
}

void Authorize::on_lineEdit_2_returnPressed()
{
    _login = ui->lineEdit->text();
    _pass = ui->lineEdit_2->text();
    if ((_login.length() != 0) && (_pass.length() != 0))
    {
        emit do_auth(_login, _pass);
        this->close();
    }
}
void Authorize::closeEvent(QCloseEvent *close)
{
    //_main->close();
    close->accept();
}
