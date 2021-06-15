#include "client.h"

client::client(boost::asio::io_service& io_service,
               const std::string& host, const std::string& service)
    : QObject(),
      _connection(io_service)
{
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(host, service);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    qRegisterMetaType<QList<QString>> ("QList<QString>");
    qRegisterMetaType<std::string> ("std::string");
    qRegisterMetaType<std::list<std::string>> ("std::list<std::string>");

    boost::asio::async_connect(_connection.socket(),
                               endpoint_iterator,
                               [this](boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator){
                                       if (ec)
                                       {
                                            emit error(QString("Not connected"));
                                       }
                                   });
}

client::~client()
{
  _connection.socket().close();
}

void client::do_login(std::string login, std::string pass)
{
    _login = login;
    chat_message out_message;
    out_message.login = login;
    out_message.message = pass;
    out_message.type = chat_message::AUTENTIFICATION;
   _connection.async_write(out_message,
                            boost::bind(&client::handle_login,
                                        this,
                                        boost::asio::placeholders::error));
}

void client::handle_login(const boost::system::error_code& e)
{
    if (e)
    {
        emit error(QString("Written error"));
    }
    else
    {
        do_read();
    }
}

void client::do_read()
{
    _connection.async_read(_message,
                        boost::bind(&client::handle_read,
                                    this,
                                    boost::asio::placeholders::error));
}

void client::handle_read(const boost::system::error_code& e)
{
    if (!e)
    {
        if (_message.type == chat_message::AUTENTIFICATION)
        {
            if (_message.message == "Good")
            {
               emit auth_complete(true);
               do_read();
            }
            else
            {
               emit auth_complete(false);
            }
        }
        if (_message.type == chat_message::TABLE)
        {
            if (_message.message  == "New")
            {
                QList<QString> new_groups;
                for (const auto & x:_message.list_string)
                    new_groups.push_back(QString::fromStdString(x));
                emit groups(new_groups);
            }
            if (_message.message == "Existing")
            {
                QList<QString> existing_groups;
                for (const auto & x:_message.list_string)
                    existing_groups.push_back(QString::fromStdString(x));
                emit groups(existing_groups);
            }
            if (_message.message == "Delete")
            {
                for (const auto & x: _message.list_string)
                emit group(QString::fromStdString(x), QString("delete"));
            }
            if (_message.message == "Exit")
            {
                if (_message.login != _login)
                    for (const auto & x:_message.list_string)
                        emit new_msg(x, _message.login, " exit", 10);
            }
            if (_message.message == "Participants")
            {
                std::string group_name = _message.list_string.front();
                emit participants(group_name, _message.list_string);
            }
            if (_message.message == "Add")
            {
                if(_message.login != _login)
                {
                for (const auto & x:_message.list_string)
                    emit new_msg(x, _message.login, " enter", 10);
                }
                if (_message.login == _login)
                {
                    QList<QString> new_groups;
                    for (const auto & x:_message.list_string)
                        new_groups.push_back(QString::fromStdString(x));
                    emit groups(new_groups);
                }
            }
            do_read();
        }
        if (_message.type == chat_message::NEW_MSG)
        {
           if (_message.login != _login)
            {
                for (const auto & x:_message.list_string)
                    emit new_msg(x, _message.login, _message.message, 1);

            }
           do_read();
        }
        if (_message.type == chat_message::FRIEND)
        {
            if(_message.message == "Request")
            {
                emit friends(1, _message.login);
            }
            if (_message.message == "Good")
                emit friends(11, _message.login);
            if (_message.message == "List")
            {
                for (const auto & x:_message.list_string)
                     emit friends(0, x);
            }
            do_read();
        }
        if (_message.type == chat_message::PERSON_CONNECTED)
        {
            emit friends(111,"1,"+_message.login);
            for (const auto & x:_message.list_string)
                emit new_msg(x, _message.login, " is online", 10);
            do_read();
        }
        if (_message.type == chat_message::PERSON_LEFT)
        {
            emit friends(111,"0,"+ _message.login);
            for (const auto & x:_message.list_string)
                emit new_msg(x, _message.login, " is offline", 10);
           do_read();
        }
    }
    else
    {
        emit error("Read Error");
    }
}

void client::do_write(chat_message & msg)
{
    _connection.async_write(msg,
            boost::bind(&client::handle_write,
                this,
                boost::asio::placeholders::error));
}

void client::choose_room(std::string type, std::string name, std::string participant)
{
    chat_message e;
    std::string choice;
    choice.append("Type:");
    choice.append(type);
    if (type == "new")
    {
        choice.append("Name:");
        choice.append(name);
        choice.append("Participants:");
        choice.append(participant);
        choice.append(",");
        e.type = chat_message::TABLE;
        e.message = choice;
        do_write(e);
    }
}

void client::handle_write(const boost::system::error_code& e)
{
    if (e)
    {
        emit error("Written Error");
    }
}

void client::add_user(std::string login, std::string pass)
{
    chat_message msg;
    msg.login = login;
    msg.message = pass;
    msg.type = chat_message::NEW_USER;
    do_write(msg);
}

std::string client::get_login() const
{
    return _login;
}

void client::get_participants(QString group)
{
    chat_message e;
    e.login = _login;
    e.message = "Participants";
    e.type = chat_message::TABLE;
    e.list_string.push_back(group.toStdString());
    do_write(e);
}

void client::add_friend (std::string name)
{
    chat_message e;
    e.type = chat_message::FRIEND;
    e.message = "Request";
    e.login = _login;
    e.list_string.push_back(name);
    do_write(e);
}

void client::add_to_group(QString name, QString group)
{
    chat_message e;
    e.type = chat_message::TABLE;
    e.message = "Add";
    e.login = name.toStdString();
    e.list_string.push_back(group.toStdString());
    do_write(e);
}
