#include "chat_room.h"
#include "chat_session.h"
#include "chat_server.h"

chat_room::chat_room(chat_server& server)
	:_server(server)
{
	std::cout << "New room" << std::endl;
}

bool chat_room::check(const std::pair<std::string, std::string> & log_pas)
{
	if (_participants.size() == 0)
    {
        user user_new;
        user_new.pass = log_pas.second;
        _participants.insert(std::pair<std::string, user>(log_pas.first, user_new));
		return true;
    }
	else
	{
        for (const auto & x : _participants)
            if (log_pas.first == x.first)
                if (log_pas.second == x.second.pass)
					return true;
	}
	return false;
}

void chat_room::join(boost::shared_ptr<chat_session> participant)
{
    auto part = _participants.find(participant->get_login());
    part->second.participant = participant;
    part->second.online = true;

    chat_message e;
    for (const auto & x : _groups)
        if (x.second.find(participant->get_login()) != x.second.end())
                e.list_string.push_back(x.first);
    e.message = "Existing";
    e.type = chat_message::TABLE;
    participant->deliver(e, chat_message::TABLE);
    std::cout << "Groups' data sent\n";

    chat_message e2;
    e2.type = chat_message::PERSON_CONNECTED;
    e2.login = participant->get_login();
    for (const auto & x: _groups)
        if (x.second.find(e2.login) != x.second.end())
        {
            e.list_string.push_back(x.first);
            deliver(e2, x.first);
        }
    participant->wait_for_data();
}

void chat_room::add_user(const std::string & name, const std::string & pass)
{
    user user_new;
    user_new.pass = pass;
    user_new.online = false;
    _participants.insert(std::pair<std::string, user>(name, user_new));
    std::cout << "User " << name << " add.\n";
}

void chat_room::choose_table(boost::shared_ptr<chat_session> participant, const chat_message & msg)
{
    std::string const utype = "Type:";
	std::string const uname = "Name:";
	std::string const uparticipants = "Participants:";

	std::string type;
	std::string name;

    std::cout << type;

	boost::smatch match;
    if (boost::regex_search(msg.message, match, boost::regex(utype + "(.*?)" + uname + "(.*?)" + ",")))
	{
        type = match[1].str();
        name = match[2].str();
    }
	if (type == "new") 
    {
        name.erase(name.find(uparticipants));
        std::string participants_names(msg.message.begin() + msg.message.find(uparticipants) + uparticipants.size(), msg.message.end());
        if (_groups.find(name) != _groups.end())
		{
            chat_message e;
            e.type = chat_message::SERVICE;
            e.message = "This table " + name + " has already exist.";
            participant->deliver(e,chat_message::SERVICE);
		}
		else
		{	
            std::string participants_names(msg.message.begin() + msg.message.find(uparticipants) + uparticipants.size(), msg.message.end());
            std::set<std::string> new_set;
            new_set.insert(participant->get_login());
			while (participants_names.size() != 0)
			{
				bool founded = false;

				std::string current_name(participants_names.begin(), participants_names.begin() + participants_names.find(','));
				for (const auto & x : _participants)
				{
                    if (current_name == x.first)
					{
                        new_set.insert(current_name);
						founded = true;
					}
				}
				if (!founded)
				{
                    chat_message e;
                    e.type = chat_message::SERVICE;
                    e.message = "This user " + current_name + " is not registry.";
                    participant->deliver(e,chat_message::SERVICE);
				}
				founded = false;
				participants_names.erase(participants_names.find(current_name), current_name.length() + 1);
			}
            _groups.insert(std::pair<std::string, std::set<std::string>>(name, new_set));

            std::cout << "Create room " << name << std::endl;

            chat_message e;
            e.type = chat_message::TABLE;
            e.message = "New";
            e.list_string.push_back(name);
            deliver(e, name);
		}
	}
}

void chat_room::deliver(const chat_message& msg, const std::string & group_name)
{
    if (group_name == "all")
    {
        for (const auto & x: _participants)
        {
            if(x.second.online)
                x.second.participant->deliver(msg, msg.type);
        }
    }
    else
    {
        auto current_table = _groups.find(group_name)->second;
        for (const auto & x: current_table)
        {
            auto p = _participants.find(x);
            if (p!=_participants.end())
                if(p->second.online)
                    p->second.participant->deliver(msg, msg.type);

        }
    }
}

void chat_room::delete_group(std::string group_name)
{
    if (_groups.find(group_name) != _groups.end())
    {
        chat_message e;
        e.type = chat_message::TABLE;
        e.message = "Delete";
        e.list_string.push_back(group_name);
        deliver(e, group_name);
        _groups.erase(group_name);
        std::cout << "Delete group " << group_name << std::endl;
    }
}

void chat_room::exit_group(std::string login, std::string group)
{
    auto p = _groups.find(group);
    if (p != _groups.end())
    {
        p->second.erase(login);
        std::cout << login << " exit group " << group << std::endl;
        if(p->second.size() == 0)
            _groups.erase(group);
        else
        {
            chat_message e;
            e.type = chat_message::TABLE;
            e.login = login;
            e.message = "Exit";
            e.list_string.push_back(group);
            deliver(e, group);
        }
    }
}

std::list<std::string> chat_room::get_participants(std::string group, std::string name)
{
    std::list<std::string> participants;
    auto p = _groups.find(group);
    if (p != _groups.end())
    {
        if (p->second.find(name) != p->second.end())
        {
            for (const auto & x: p->second)
            {
                int pstatus = _participants.find(x)->second.online;
                participants.push_back(std::to_string(pstatus)+','+x);
            }
        }
    }
    return participants;
}

void chat_room::friends(int type, std::string from, std::string to)
{
    if(type == 1)
    {
        chat_message e;
        e.type = chat_message::FRIEND;
        e.login = from;
        e.message = "Request";
        auto p = _participants.find(to);
        if (p!= _participants.end())
            if (p->second.online)
                p->second.participant->deliver(e,chat_message::FRIEND);
    }
    if (type == 11)
    {
        auto p1 = _participants.find(to);
        auto p2 = _participants.find(from);
        if ((p1!= _participants.end())&&(p2!= _participants.end()))
        {
            p2->second.friends.insert(to);
            p1->second.friends.insert(from);
            chat_message e;
            e.type = chat_message::FRIEND;
            e.login = from;
            e.message = "Good";
            if (p1->second.online)
                p1->second.participant->deliver(e,chat_message::FRIEND);
        }
    }
    if (type == -1)
    {
        auto p1 = _participants.find(from); //кто удаляет
        auto p2 = _participants.find(to); //кого удаляет
        if ((p1!= _participants.end())&&(p2!= _participants.end()))
        {
            p1->second.friends.erase(to);
            p2->second.friends.erase(from);
        }
    }
    if (type == 0)
    {
        chat_message e;
        e.type = chat_message::FRIEND;
        e.message = "List";
        auto p = _participants.find(from);
        if (p != _participants.end())
            for (const auto & x:p->second.friends)
            {
                std::cout << "Sent friend info..." << x;
                e.list_string.push_back(std::to_string(p->second.online)+','+x);
                p->second.participant->deliver(e, chat_message::FRIEND);
            }
    }
}

void chat_room::add_to_group(std::string login, std::string  group)
{
    if (_participants.find(login) != _participants.end())
    {
        auto p = _groups.find(group);
        if (p != _groups.end())
        {
            chat_message e;
            e.type = chat_message::TABLE;
            e.login = login;
            e.message = "Add";
            e.list_string.push_back(group);
            p->second.insert(login);
            std::cout << login << " add to group " << group << std::endl;
            deliver(e, group);
        }
    }
}

void chat_room::leave(chat_session_ptr participant)
{
    auto part = _participants.find(participant->get_login());
    part->second.online = false;
    chat_message e;
    e.type = chat_message::PERSON_LEFT;
    e.login = participant->get_login();
    for (const auto & x: _groups)
        if (x.second.find(e.login) != x.second.end())
        {
            e.list_string.push_back(x.first);
            deliver(e, x.first);
        }
}

void chat_room::leave(std::string name)
{
    auto part = _participants.find(name);
    part->second.online = false;
    chat_message e;
    e.type = chat_message::PERSON_LEFT;
    e.login = name;
    for (const auto & x: _groups)
        if (x.second.find(e.login) != x.second.end())
        {
            e.list_string.push_back(x.first);
            deliver(e, x.first);
        }
}

