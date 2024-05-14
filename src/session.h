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
    asio::ip::tcp::socket socket_;
};