#include "asiomp.h"
#include "echo_session.h"

std::shared_ptr<session> echo_session_creator(asio::ip::tcp::socket socket) {
    auto sptr = std::make_shared<echo_session>(std::move(socket));
    return std::dynamic_pointer_cast<session>(sptr);
}

int main(int argc, char *argv[]) {
    session_factory::getInstance()->register_session("echo_session", echo_session_creator);
    asiomp_server(argv, "127.0.0.1", 5555, 2, false, "echo_session").run();
    return 0;
}