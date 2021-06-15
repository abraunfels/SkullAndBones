#ifndef CHAT_SESSION_H
#define CHAT_SESSION_H

#include "Header.h"
#include "chat_server.h"
#include "chat_room.h"
#include "tcp_connection.h"

using boost::asio::ip::tcp;

class chat_server;

class chat_session : public boost::enable_shared_from_this<chat_session>
{
public:
    void close();
    ~chat_session();
    static boost::shared_ptr<chat_session> create(connection_ptr tcp_connection, chat_room_ptr room)
    {
        boost::shared_ptr<chat_session> session(new chat_session(tcp_connection, room));
        session->authorize();
        return session;
    }
    std::string get_login() const;
    std::string get_pass() const;
    void deliver(const chat_message & msg, int type);
    void wait_for_data();
private:
    chat_session(connection_ptr tcp_connection, chat_room_ptr room);
    void authorize();

    void handle_auth(const boost::system::error_code &error);
    void handle_write(const boost::system::error_code& error, int);
    void handle_read(const boost::system::error_code& error);

private:
    std::pair<std::string, std::string>	_user;
    boost::shared_ptr<tcp_connection>	_tcp_connection;
    boost::weak_ptr<chat_room>			_room;

    chat_message						_message;

    bool								_auth;
    bool								is_leaving;
};

typedef boost::shared_ptr<chat_session> chat_session_ptr;

#endif // CHAT_SESSION_H
