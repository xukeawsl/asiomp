#pragma once

#include "common.h"

class session
  : public std::enable_shared_from_this<session>
{
public:
    explicit session(asio::ip::tcp::socket socket);

    virtual ~session() = default;

    virtual void start();

protected:
    asio::ip::tcp::socket socket_;
};