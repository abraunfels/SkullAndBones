#ifndef ADDUSER_H
#define ADDUSER_H

#include "head.h"

namespace Ui {
class AddUser;
}

class AddUser : public QDialog
{
    Q_OBJECT

public:
    explicit AddUser(QWidget *parent = 0);
    ~AddUser();
signals:
    void add_new(std::string, std::string);
private slots:
    void on_lineEdit_returnPressed();

    void on_pushButton_6_clicked();

private:
    Ui::AddUser *ui;
};

#endif // ADDUSER_H
