#pragma once

#include <string>
#include <memory>
#include <functional>

#include <unistd.h>
#include <sys/wait.h>

#include "asio.hpp"

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"