#include "asiomp.h"
#include "chat_session.h"

std::shared_ptr<session> chat_session_creator(asio::ip::tcp::socket socket) {
    auto sptr = std::make_shared<chat_session>(std::move(socket));
    return std::dynamic_pointer_cast<session>(sptr);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: chat_server <port> [<port> ...]\n";
        return 1;
    }

    session_factory::getInstance()->register_session("chat_session",
                                                     chat_session_creator);
    asiomp_server(argv, "127.0.0.1", std::atoi(argv[1]), 2, false,
                  "chat_session")
        .run();
    return 0;
}