#include "session.h"

session::session(asio::ip::tcp::socket socket) : socket_(std::move(socket)) {}

session::~session() {}

void session::start() {
    // add your code
}
