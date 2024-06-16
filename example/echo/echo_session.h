#pragma once

#include "asiomp.h"

class echo_session
  : public session
{
public:
    explicit echo_session(asio::ip::tcp::socket socket);

    ~echo_session();

    void start() override;

private:
    void do_read();

    void do_write(std::size_t length);

    enum { max_length = 1024 };
    char data_[max_length];

    asio::ip::tcp::socket socket_;
};