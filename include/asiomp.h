#pragma once

#include "common.h"
#include "session_factory.h"

#ifndef ASIOMP_PROC_NAME
#define ASIOMP_PROC_NAME "asiomp"
#endif

class asiomp_server {
public:
    asiomp_server(char **argv, const std::string& host, uint16_t port, bool daemon, const std::string& session);

    asiomp_server(char **argv, const std::string& host, uint16_t port, uint32_t worker_num, bool deamon, const std::string& session);

    ~asiomp_server();

    void run() noexcept;

    template <class T>
    static void register_session(const std::string& session_name) {
        session_factory::getInstance()->register_session(session_name,
            [](asio::ip::tcp::socket socket) -> std::shared_ptr<session> {
                return std::dynamic_pointer_cast<session>(std::make_shared<T>(std::move(socket)));
            });
    }

protected:
    struct process_t {
        pid_t pid;
        bool exited;

        process_t() : pid(-1), exited(true) {}
    };

private:
    void stop_server();

    void init();

    bool set_daemon();

    void init_setproctitle();

    void set_proctitle(const std::string& title);

    void set_logger(const std::string& logger_name);

    void signal_handler();

    void worker_process();

    void spawn_process();

    void spawn_process(int respawn);

    void handle_accept();

    void stop_worker();

    void update_status();

    void reap_worker();

private:
    char **os_argv;
    char *os_argv_last;

    std::shared_ptr<char[]> os_environ;

    asio::io_context io_context;
    asio::signal_set signals;
    asio::ip::tcp::acceptor acceptor;
    asio::ip::tcp::endpoint listen_endpoint;

    bool single_mode;
    bool isworker;
    bool terminate;
    bool isdaemon;
    std::string session_name;
    std::vector<process_t> processes;

    std::string log_dir;
};