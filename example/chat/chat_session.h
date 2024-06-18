#pragma once

#include "asiomp.h"
#include "chat_message.hpp"

class chat_session : public session {
public:
    explicit chat_session(asio::ip::tcp::socket socket);

    void start() override;

private:
    void stop();

    bool join_room();

    void client_to_room();

    void room_to_client();

    void do_read_header_from_client();

    void do_read_body_from_client();

    void send_to_room();

    void do_read_header_from_room();

    void do_read_body_from_room();

    void send_to_client();

private:
    asio::ip::tcp::socket socket_;
    asio::local::stream_protocol::socket room_socket_;
    chat_message read_msg_;
    chat_message write_msg_;
};