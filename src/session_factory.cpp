#include "session_factory.h"

session_factory* session_factory::getInstance() {
    static session_factory factory;
    return &factory;
}

std::shared_ptr<session> session_factory::default_creator(asio::ip::tcp::socket socket) {
    return std::make_shared<session>(std::move(socket));
}

void session_factory::register_session(const std::string& session_name, session_creator creator) {
    this->creators_[session_name] = creator;
}

std::shared_ptr<session> session_factory::create_session(const std::string& session_name, asio::ip::tcp::socket socket) {
    auto it = this->creators_.find(session_name);
    if (it != this->creators_.end()) {
        return it->second(std::move(socket));
    }
    return default_creator(std::move(socket));
}