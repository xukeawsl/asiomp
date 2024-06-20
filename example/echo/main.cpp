#include "asiomp.h"
#include "echo_session.h"

int main(int argc, char *argv[]) {
    asiomp_server::register_session<echo_session>("echo_session");
    asiomp_server(argv, "127.0.0.1", 5555, 2, false, "echo_session").run();
    return 0;
}