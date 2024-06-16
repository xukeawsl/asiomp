#pragma once

#include "common.h"
#include "session.h"

class session_factory {
public:
    using session_creator = std::function<std::shared_ptr<session>(asio::ip::tcp::socket)>;

    static session_factory* getInstance();

    void register_session(const std::string& session_name, session_creator creator);

    std::shared_ptr<session> create_session(const std::string& session_name, asio::ip::tcp::socket socket);

private:
    std::shared_ptr<session> default_creator(asio::ip::tcp::socket socket);

    std::unordered_map<std::string, session_creator> creators_;
};