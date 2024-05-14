#pragma once

#include "asiomp.h"

class session
  : public std::enable_shared_from_this<session>
{
public:
    session(asio::ip::tcp::socket socket);

    ~session();

    void start();

private:
    void do_read();

    void do_write(std::size_t length);

    asio::ip::tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};