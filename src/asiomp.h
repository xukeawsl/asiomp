#pragma once

#include <string>
#include <memory>

#include <unistd.h>
#include <sys/wait.h>

#include "asio.hpp"

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"

class asiomp_server {
public:
    asiomp_server(const std::string& host, uint16_t port);

    asiomp_server(const std::string& host, uint16_t port, uint32_t worker_num);

    ~asiomp_server();

    void run() noexcept;

protected:
    struct process_t {
        pid_t pid;
        bool exited;

        process_t() : pid(-1), exited(true) {}
    };

private:
    void stop_server();

    void init();

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
    asio::io_context io_context;
    asio::signal_set signals;
    asio::ip::tcp::acceptor acceptor;
    asio::ip::tcp::endpoint listen_endpoint;

    bool single_mode;
    bool isworker;
    bool terminate;
    std::vector<process_t> processes;

    std::string log_dir;
};