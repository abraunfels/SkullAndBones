#include "chat_session.h"
#include "chat_server.h"

chat_session::chat_session(connection_ptr tcp_connection, chat_room_ptr room) : _tcp_connection(tcp_connection),
	_room(room)
{
	std::cout << "New chat_session ! " << std::endl;
}

chat_session::~chat_session()
{
	close();
}

void chat_session::close()
{
    //_room.lock()->leave(shared_from_this());
	_tcp_connection->socket().close();
	std::cout << "Session destruct" << std::endl;
}

void chat_session::authorize()
{
	_tcp_connection->async_read(_message,
		boost::bind(&chat_session::handle_auth, shared_from_this(),
			boost::asio::placeholders::error)
		);
}

void chat_session::handle_auth(const boost::system::error_code &error)
{
	if (!error)
	{
		auto room = _room.lock();
        if (room->check(std::pair<std::string, std::string> (_message.login, _message.message)))
		{
			_user.first = _message.login;
			_user.second = _message.message;
			chat_message e;
			e.type = chat_message::AUTENTIFICATION;
			e.message = "Good";
            deliver(e, chat_message::AUTENTIFICATION);
            std::cout << "Authorize access!\n";
            auto room = _room.lock();
            room->join(shared_from_this());
		}
		else
		{
            _user.first = _message.login;
            _user.second = _message.message;
            chat_message e;
            e.type = chat_message::AUTENTIFICATION;
            e.message = "Bad";
            deliver(e, -1);
            std::cout << "Authorize doesn't access!\n";
            authorize();
		}
	}
	else
	{
		boost::mutex::scoped_lock lk(debug_mutex);
		debug_global << __FUNCTION__ << " " << error << ", " << error.message() << std::endl;
	}
}

void chat_session::handle_write(const boost::system::error_code &error, int parametr)
{
	if (error) // (room && error && (!is_leaving))
	{
		boost::mutex::scoped_lock lk(debug_mutex);
		debug_global << __FUNCTION__ << " " << error << ", " << error.message() << std::endl;
		//is_leaving = true;
        //room->leave(shared_from_this());boost::shared_ptr<chat_room> room = _room.lock();
    }
	else
	{

	}
}

void chat_session::wait_for_data() 
{
	_tcp_connection->async_read(_message,
		boost::bind(&chat_session::handle_read, shared_from_this(),
			boost::asio::placeholders::error)
		);
}

void chat_session::handle_read(const boost::system::error_code &error) 
{
    boost::shared_ptr<chat_room> room = _room.lock();
    if (!error)
		{
            if (_message.type == chat_message::NEW_USER)
			{
                room->add_user(_message.login, _message.message);
			}
            if (_message.type == chat_message::TABLE)
            {
                if (_message.message == "Delete")
                {
                    for(const auto & x: _message.list_string)
                        room->delete_group(x);
                }
                else if(_message.message == "Exit")
                {
                    for(const auto & x: _message.list_string)
                        room->exit_group(_message.login, x);
                }
                else if (_message.message == "Participants")
                {
                   for(const auto & x: _message.list_string)
                   {
                       std::list<std::string> list = room->get_participants(x, _message.login);
                       chat_message e;
                       e.type = chat_message::TABLE;
                       e.message = "Participants";
                       e.list_string = list;
                       e.list_string.push_front(x);
                       deliver(e, chat_message::TABLE);
                       std::cout << "Participants list sent\n";
                   }
                }
                else if (_message.message == "Add")
                {
                    room->add_to_group(_message.login, _message.list_string.front());
                }
                else
                    room->choose_table(shared_from_this(), _message);
            }
            if (_message.type == chat_message::NEW_MSG)
            {
                for (const auto & x: _message.list_string)
                    room->deliver(_message, x);
            }
            if (_message.type == chat_message::FRIEND)
            {
                if (_message.message == "Request")
                    room->friends(1, _message.login, _message.list_string.front());
                if(_message.list_string.front() == "Good")
                    room->friends(11, _message.login, _message.message);
                if (_message.list_string.front() == "Delete")
                    room->friends(-1, _message.login, _message.message);
                if (_message.message == "Get")
                    room->friends(0, _message.login, "");

            }
            if(_message.type == chat_message::PERSON_CONNECTED)
            {
                room->deliver(_message, "all");
            }
            if(_message.type == chat_message::PERSON_LEFT)
            {
                //room->leave(_message.login);
                room->deliver(_message, "all");
            }
            wait_for_data();
		}
    else
    {
        room->leave(shared_from_this());
    }
}

std::string chat_session::get_login() const
{
	return _user.first;
}

std::string chat_session::get_pass() const
{
	return _user.second;
}

void chat_session::deliver(const chat_message & msg, int type) 
{
	_tcp_connection->async_write(msg,
		boost::bind(&chat_session::handle_write, shared_from_this(),
            boost::asio::placeholders::error, type)
		);
}



