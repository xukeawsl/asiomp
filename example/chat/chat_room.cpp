#include <cstdio>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <set>

#include "asio.hpp"
#include "chat_message.hpp"

typedef std::deque<chat_message> chat_message_queue;

class chat_participant {
public:
    virtual ~chat_participant() {}
    virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

class chat_room {
public:
    void join(chat_participant_ptr participant) {
        participants_.insert(participant);
        for (auto msg : recent_msgs_) participant->deliver(msg);
    }

    void leave(chat_participant_ptr participant) {
        participants_.erase(participant);
    }

    void deliver(const chat_message& msg) {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs) recent_msgs_.pop_front();

        for (auto participant : participants_) participant->deliver(msg);
    }

private:
    std::set<chat_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    chat_message_queue recent_msgs_;
};

class session : public chat_participant, public std::enable_shared_from_this<session> {
public:
    session(asio::local::stream_protocol::socket sock, chat_room& room)
        : socket_(std::move(sock)), room_(room) {}

    void start() {
        room_.join(shared_from_this());
        do_read_header();
    }

    void deliver(const chat_message& msg) {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress) {
            do_write();
        }
    }

private:
    void do_read_header() {
        auto self(shared_from_this());
        asio::async_read(
            socket_,
            asio::buffer(read_msg_.data(), chat_message::header_length),
            [this, self](std::error_code ec, std::size_t /*length*/) {
                if (!ec && read_msg_.decode_header()) {
                    do_read_body();
                } else {
                    room_.leave(shared_from_this());
                }
            });
    }

    void do_read_body() {
        auto self(shared_from_this());
        asio::async_read(
            socket_, asio::buffer(read_msg_.body(), read_msg_.body_length()),
            [this, self](std::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    room_.deliver(read_msg_);
                    do_read_header();
                } else {
                    room_.leave(shared_from_this());
                }
            });
    }

    void do_write() {
        auto self(shared_from_this());
        asio::async_write(
            socket_,
            asio::buffer(write_msgs_.front().data(),
                         write_msgs_.front().length()),
            [this, self](std::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    write_msgs_.pop_front();
                    if (!write_msgs_.empty()) {
                        do_write();
                    }
                } else {
                    room_.leave(shared_from_this());
                }
            });
    }

private:
    asio::local::stream_protocol::socket socket_;
    chat_room& room_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

class chat_center {
public:
    chat_center(asio::io_context& io_context, const std::string& file)
        : acceptor_(io_context, asio::local::stream_protocol::endpoint(file)) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](std::error_code ec,
                   asio::local::stream_protocol::socket socket) {
                if (!ec) {
                    std::make_shared<session>(std::move(socket), room_)
                        ->start();
                } else {
                    std::cout << ec.message() << std::endl;
                }

                do_accept();
            });
    }

private:
    asio::local::stream_protocol::acceptor acceptor_;
    chat_room room_;
};

int main(int argc, char* argv[]) {
    try {
        asio::io_context io_context;

        std::remove("/tmp/chat_room");

        chat_center center(io_context, "/tmp/chat_room");

        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}