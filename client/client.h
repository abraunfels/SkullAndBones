#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "head.h"
#include "chat_message.h"
#include "tcp_connection.h"
#include "client.h"

#define max_lenght 9999

class client : public QObject
{
    Q_OBJECT
public slots:
    void add_user(std::string login, std::string pass);
    void choose_room(std::string type, std::string name, std::string participant);
    void get_participants(QString);
    void add_to_group(QString, QString);
public:
    client(boost::asio::io_service& io_service, const std::string& host, const std::string& service);
    ~client();
    void do_login(std::string login, std::string pass);
    std::string get_login() const;
    void do_write(chat_message &);
    void add_friend (std::string);
signals:
    void error(QString);
    void auth_complete(bool);
    void groups(QList<QString>);
    void new_msg(std::string, std::string, std::string, int);
    void group(QString, QString);
    void participants (std::string, std::list<std::string>);
    void friends (int, std::string);
private:
    void handle_login(const boost::system::error_code& e);
    void do_read();
    void handle_read(const boost::system::error_code& e);
    void handle_write(const boost::system::error_code& e);
private:
    tcp_connection	_connection;
    std::string		_login;
    chat_message	_message; //входящее сообщение 
};

#endif //CLIENT_H
