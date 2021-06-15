#ifndef AUTHORIZE_H
#define AUTHORIZE_H

#include <QMainWindow>
#include "head.h"

namespace Ui {
class Authorize;
}

class Authorize : public QDialog
{
    Q_OBJECT

public:
    explicit Authorize(QDialog * parent = 0);
    ~Authorize();
signals:
    void do_auth(QString, QString);
private slots:
    void on_lineEdit_2_returnPressed();
private:
    void closeEvent(QCloseEvent *close);
private:
    QString         _login;
    QString         _pass;

    Ui::Authorize*  ui;

};

#endif // AUTHORIZE_H
