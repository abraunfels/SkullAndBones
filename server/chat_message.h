#pragma once

#include "Header.h"

class chat_message
{

public:

	void reset()
	{
		list_string.clear();
		message.clear();
		login.clear();
	}
	int type;

	std::list<std::string>	list_string;

	std::string	message;
	std::string	login;


	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & type & list_string & message & login;
	}

	enum {
		AUTENTIFICATION = 0,
        TABLE = 1,
		NEW_MSG = 2,
		PERSON_LEFT = 3,
		PERSON_CONNECTED = 4,
		SERVICE = 5,
        NEW_USER = 6,
        FRIEND = 7,
	};
};
