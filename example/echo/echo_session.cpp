#include "echo_session.h"

echo_session::echo_session(asio::ip::tcp::socket socket) : socket_(std::move(socket)) {}

echo_session::~echo_session() {}

void echo_session::start() { do_read(); }

void echo_session::do_read() {
    auto self(getDerivedSharedPtr<echo_session>());
    socket_.async_read_some(
        asio::buffer(data_, max_length),
        [this, self](std::error_code ec, std::size_t length) {
            if (!ec) {
                do_write(length);
            }
        });
}

void echo_session::do_write(std::size_t length) {
    auto self(getDerivedSharedPtr<echo_session>());
    asio::async_write(socket_, asio::buffer(data_, length),
                      [this, self](std::error_code ec, std::size_t length) {
                          if (!ec) {
                              do_read();
                          }
                      });
}