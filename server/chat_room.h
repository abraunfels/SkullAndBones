#pragma once

#include "Header.h"
#include "chat_message.h"

class chat_session;
class chat_server;
typedef boost::shared_ptr<chat_session> chat_session_ptr;

class chat_room
{
public:
	chat_room(chat_server& server); 

    bool	check(const std::pair<std::string, std::string> &);
	void	join(boost::shared_ptr<chat_session> participant);
	void	choose_table(boost::shared_ptr<chat_session> participant, const chat_message & msg);
    void    add_user(const std::string & name, const std::string & pass);
    void	deliver(const chat_message & msg, const std::string & table_name = "all");
    void    delete_group(std::string);
    void    exit_group(std::string, std::string);
    std::list<std::string> get_participants(std::string, std::string);
    void    friends(int, std::string, std::string);
    void    add_to_group(std::string, std::string);
    void	leave(chat_session_ptr participant);
    void    leave(std::string name);

private:
    struct user //информация о пользователе
    {
        std::set<std::string>               friends;
        boost::shared_ptr<chat_session> participant;
        std::string                            pass;
        bool                                 online;
    };
    std::map<std::string, std::set<std::string>>             _groups; //конференции
    std::map<std::string, user>                        _participants; //все пользователи чата
    chat_server&                                             _server;
};

typedef boost::shared_ptr<chat_room>	chat_room_ptr;
typedef boost::weak_ptr<chat_room>		chat_room_wptr;
