#include "asiomp.h"

int main(int argc, char *argv[]) {
    // asiomp_server(argv, "127.0.0.1", 5555, false).run();    // 单进程模式
    asiomp_server(argv, "127.0.0.1", 5555, 2, true).run();    // 多进程模式
    return 0;
}