#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>
#include "head.h"
#include "authorize.h"
#include "chatwindow.h"

namespace Ui {
class Settings;
}

class Settings : public QMainWindow
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

signals:
    void create_client(std::string ip, std::string port);

private slots:
    void on_spinBox_editingFinished();

    void on_spinBox_2_editingFinished();

    void on_spinBox_3_editingFinished();

    void on_spinBox_4_editingFinished();

    void on_spinBox_5_editingFinished();

    void on_pushButton_clicked();

private:
    void set();

private:
    Ui::Settings *ui;

    int _ip1;
    int _ip2;
    int _ip3;
    int _ip4;
    int _port;
};

#endif // SETTINGS_H
