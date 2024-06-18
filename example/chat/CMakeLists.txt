cmake_minimum_required(VERSION 3.10)

project(chat_server)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb -Wall")

include_directories(
    ../../include
    ../../third-party/asio/asio/include
)

link_directories(
    ../../build
)

add_executable(chat_client chat_client.cpp)
target_link_libraries(chat_client PUBLIC asiomp pthread)

add_executable(chat_server chat_server.cpp chat_session.cpp)
target_link_libraries(chat_server PUBLIC asiomp pthread)

add_executable(chat_room chat_room.cpp)
target_link_libraries(chat_room PUBLIC pthread)