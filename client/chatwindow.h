#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include "head.h"
#include "client.h"
#include "settings.h"
#include "authorize.h"
#include "adduser.h"
#include "list_item.h"
#include "addgroup.h"
#include "letters.h"

namespace Ui {
class ChatWindow;
}

class Authorize;
class client;
class Letters;

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(const std::string ip, const std::string port, QWidget *parent = 0);
    ~ChatWindow();
    QStringList get_friends() const;
public slots:
    void error(QString);
    void do_auth(QString, QString);
    void is_auth_success(bool);
    void group(QString, QString);
 signals:
    void choose_room(std::string, std::string, std::string);
private slots:
    void on_actionAdd_triggered();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_lineEdit_2_returnPressed();
    void display_groups(QList<QString>);
    void send_msg(QString, QString);
    void display_msg(std::string, std::string, std::string, int);
    void participants(std::string,std::list<std::string>);
    void on_actionfriends_triggered();
    void on_lineEdit_returnPressed();
    void friends(int,std::string);
    void on_pushButton_6_clicked();
    void delete_friend();
    void start_message();
protected:
    void closeEvent(QCloseEvent *event);
    void contextMenuEvent( QContextMenuEvent* e);
private:
    void makeItem( QListWidget* lstWgt, QString i);
    void conv_msg(QString str) const;

private:
    Ui::ChatWindow*                                 ui;

    Authorize*                                      _auth;
    AddUser*                                        _add_user;
    AddGroup*                                       _add_group;
    QMenu*                                          _menu;
    QLayout*                                        _l;
    std::map<QString, QListWidgetItem *>            _items;
    std::map<QString, Letters *>                    _chats;

    std::set<QString>                               _friends;

    boost::asio::io_service                         _io_service;
    std::unique_ptr<boost::asio::io_service::work>  _work;
    std::thread                                     _io_service_runner;
    client                                          _client;

    bool                                            _done;
};

#endif // CHATWINDOW_H
