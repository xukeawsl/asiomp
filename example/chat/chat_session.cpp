#include "chat_session.h"

chat_session::chat_session(asio::ip::tcp::socket socket)
    : socket_(std::move(socket)), room_socket_(socket_.get_executor()) {}

void chat_session::start() {
    if (!join_room()) {
        stop();
        return;
    }

    client_to_room();
    room_to_client();
}

void chat_session::stop() {
    asio::error_code ignore_ec;
    socket_.close(ignore_ec);
    room_socket_.close(ignore_ec);
}

bool chat_session::join_room() {
    asio::error_code ec;
    room_socket_.connect(
        asio::local::stream_protocol::endpoint("/tmp/chat_room"), ec);

    if (ec) {
        std::cerr << "failed to join chat room: " << ec.message() << std::endl;
        return false;
    }

    return true;
}

void chat_session::client_to_room() { do_read_header_from_client(); }

void chat_session::room_to_client() { do_read_header_from_room(); }

void chat_session::do_read_header_from_client() {
    auto self(getDerivedSharedPtr<chat_session>());
    asio::async_read(
        socket_, asio::buffer(read_msg_.data(), chat_message::header_length),
        [this, self](std::error_code ec, std::size_t /*length*/) {
            if (!ec && read_msg_.decode_header()) {
                do_read_body_from_client();
            } else {
                stop();
            }
        });
}

void chat_session::do_read_body_from_client() {
    auto self(getDerivedSharedPtr<chat_session>());
    asio::async_read(socket_,
                     asio::buffer(read_msg_.body(), read_msg_.body_length()),
                     [this, self](std::error_code ec, std::size_t /*length*/) {
                         if (!ec) {
                             send_to_room();
                         } else {
                             stop();
                         }
                     });
}

void chat_session::send_to_room() {
    auto self(getDerivedSharedPtr<chat_session>());
    asio::async_write(room_socket_,
                      asio::buffer(read_msg_.data(), read_msg_.length()),
                      [this, self](std::error_code ec, std::size_t /**/) {
                          if (!ec) {
                              client_to_room();
                          } else {
                              stop();
                          }
                      });
}

void chat_session::do_read_header_from_room() {
    auto self(getDerivedSharedPtr<chat_session>());
    asio::async_read(
        room_socket_,
        asio::buffer(write_msg_.data(), chat_message::header_length),
        [this, self](std::error_code ec, std::size_t /*length*/) {
            if (!ec && write_msg_.decode_header()) {
                do_read_body_from_room();
            } else {
                stop();
            }
        });
}

void chat_session::do_read_body_from_room() {
    auto self(getDerivedSharedPtr<chat_session>());
    asio::async_read(room_socket_,
                     asio::buffer(write_msg_.body(), write_msg_.body_length()),
                     [this, self](std::error_code ec, std::size_t /*length*/) {
                         if (!ec) {
                             send_to_client();
                         } else {
                             stop();
                         }
                     });
}

void chat_session::send_to_client() {
    auto self(getDerivedSharedPtr<chat_session>());
    asio::async_write(socket_,
                      asio::buffer(write_msg_.data(), write_msg_.length()),
                      [this, self](std::error_code ec, std::size_t /**/) {
                          if (!ec) {
                              room_to_client();
                          } else {
                              stop();
                          }
                      });
}
