#ifndef LETTERS_H
#define LETTERS_H

#include <QWidget>
#include "head.h"
#include "chatwindow.h"

namespace Ui {
class Letters;
}

class ChatWindow;

class Letters : public QWidget
{
    Q_OBJECT

public:
    explicit Letters(QString, ChatWindow*, QWidget *parent = 0);
    ~Letters();
    void publish(std::string, std::string, int);
    void display_particiants(std::list<std::string>);
signals:
    void send_msg(QString, QString);
    void group(QString, QString);
    void participants(QString);
    void add_to_group(QString, QString);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_2_clicked();
    void on_lineEdit_returnPressed();
    void on_pushButton_7_clicked();

private:
    void makePartItem( QListWidget*, QString);
private:
    Ui::Letters *   ui;

    ChatWindow *    _par;

    QString         _group;
};

#endif // LETTERS_H
