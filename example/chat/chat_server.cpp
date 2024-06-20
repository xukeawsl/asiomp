#include "asiomp.h"
#include "chat_session.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: chat_server <port> [<port> ...]\n";
        return 1;
    }

    asiomp_server::register_session<chat_session>("chat_session");
    asiomp_server(argv, "127.0.0.1", std::atoi(argv[1]), 2, false,
                  "chat_session")
        .run();
    return 0;
}