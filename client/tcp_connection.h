﻿#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "head.h"

class tcp_connection
{
public:
    tcp_connection(boost::asio::io_service& io_service)
        : _socket(io_service)
    {
    }

    boost::asio::ip::tcp::socket& socket()
    {
        return _socket;
    }


    template <typename T, typename Handler>
    void async_write(const T& t, Handler handler)
    {
        std::ostringstream archive_stream;
        std::ostream archive_stream_b(archive_stream.rdbuf());
        boost::archive::text_oarchive archive(archive_stream_b);
        archive << t;
        _outbound_data = archive_stream.str();


        std::ostringstream header_stream;
        std::ostream header_stream_b(header_stream.rdbuf());
        header_stream << std::setw(header_length)
            << std::hex << _outbound_data.size();
        if (!header_stream || header_stream.str().size() != header_length)
        {

            boost::system::error_code error(boost::asio::error::invalid_argument);
            (_socket.get_io_service()).post(boost::bind(handler, error));
            return;
        }
        _outbound_header = header_stream.str();


        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::buffer(_outbound_header));
        buffers.push_back(boost::asio::buffer(_outbound_data));
        boost::asio::async_write(_socket, buffers, handler);
    }


    template <typename T, typename Handler>
    void async_read(T& t, Handler handler)
    {

        void (tcp_connection::*f)(const boost::system::error_code&, T&, boost::tuple<Handler>) = &tcp_connection::handle_read_header<T, Handler>; //ÃÂ·ÃÂ°ÃÂ´ÃÂ°ÃÂµÃÂ¼ ÃÂ½ÃÂ¾ÃÂ²Ã‘Æ’Ã‘Å½ Ã‘â€žÃ‘Æ’ÃÂ½ÃÂºÃ‘â€ ÃÂ¸Ã‘Å½
        boost::asio::async_read(_socket, boost::asio::buffer(_inbound_header),
            boost::bind(f,
                this, boost::asio::placeholders::error, boost::ref(t),
                boost::make_tuple(handler)));
    }

    template <typename T, typename Handler>
    void handle_read_header(const boost::system::error_code& e,
        T& t, boost::tuple<Handler> handler)
    {
        if (e)
        {
            boost::get<0>(handler)(e);
        }
        else
        {
            std::istringstream is(std::string(_inbound_header, header_length));
            std::size_t _inbound_datasize = 0;
            if (!(is >> std::hex >> _inbound_datasize))
            {
                boost::system::error_code error(boost::asio::error::invalid_argument);
                boost::get<0>(handler)(error);
                return;
            }
            _inbound_data.resize(_inbound_datasize);
            void (tcp_connection::*f)(const boost::system::error_code&, T&, boost::tuple<Handler>)
                = &tcp_connection::handle_read_data<T, Handler>;

            boost::asio::async_read(_socket, boost::asio::buffer(_inbound_data),
                boost::bind(f, this,
                    boost::asio::placeholders::error, boost::ref(t), handler));
        }
    }

    template <typename T, typename Handler>
    void handle_read_data(const boost::system::error_code& e,
        T& t, boost::tuple<Handler> handler)
    {
        if (e)
        {
            boost::get<0>(handler)(e);
        }
        else
        {
            try
            {
                std::string archive_data(&_inbound_data[0], _inbound_data.size());
                std::istringstream archive_stream(archive_data);
                std::istream archive_stream_b(archive_stream.rdbuf());
                boost::archive::text_iarchive archive(archive_stream_b);
                archive >> t;
            }
            catch (std::exception& e)
            {
                boost::system::error_code error(boost::asio::error::invalid_argument);
                boost::get<0>(handler)(error);
                return;
            }
            boost::get<0>(handler)(e);
        }
    }

private:

    boost::asio::ip::tcp::socket _socket;


    enum { header_length = 8 };

    std::string			_outbound_header;
    std::string			_outbound_data;
    char				_inbound_header[header_length];
    std::vector<char>	_inbound_data;
};

typedef boost::shared_ptr<tcp_connection> connection_ptr;

#endif
