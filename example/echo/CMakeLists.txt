cmake_minimum_required(VERSION 3.10)

project(echo_server)

include_directories(
    ../../src
    ../../third-party/asio/asio/include
)

link_directories(
    ../../build
)

add_executable(${PROJECT_NAME} main.cpp echo_session.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC asiomp)