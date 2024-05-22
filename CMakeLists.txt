cmake_minimum_required(VERSION 3.10)

project(asiomp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose Release or Debug" FORCE)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wno-unused")

add_subdirectory(third-party/spdlog)

include(cmake/settings.cmake)

file(GLOB_RECURSE srcs ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE hrds ${PROJECT_SOURCE_DIR}/src/*.h)

include_directories(
    src
    third-party/asio/asio/include
)

add_executable(${PROJECT_NAME} main.cpp ${srcs})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    ASIOMP_PROC_NAME="${PROJECT_NAME}"
)

target_link_libraries(${PROJECT_NAME} PUBLIC pthread spdlog::spdlog)

#
# Clang-Format
#
find_program(CLANG_FORMAT_EXECUTABLE
    NAME "clang-format"
    PATHS "/usr/bin" "/usr/local/bin"
)

if (CLANG_FORMAT_EXECUTABLE)
    message(STATUS "Successfully find program `clang-format`")
    message(STATUS "You can use the `make clang-format` command to automatically format the code style")
    add_custom_target(clang-format ALL
    COMMAND
        ${CLANG_FORMAT_EXECUTABLE} --style=file -i ${hdrs};${srcs};${PROJECT_SOURCE_DIR}/main.cpp
    COMMENT
        "Automatically format the code style"
    )
endif()

#
# Valgrind
#
find_program(VALGRIND_EXECUTABLE
    NAMES "valgrind"
    PATHS "/usr/bin" "/usr/local/bin"
)

if (VALGRIND_EXECUTABLE)
    message(STATUS "Successfully find program `valgrind`")
    message(STATUS "You can use the `make valgrind` command to perform memory leak detection")
    add_custom_target(valgrind
    COMMAND
        ${VALGRIND_EXECUTABLE} --log-file=memcheck.log --leak-check=full --trace-children=yes --track-origins=yes $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT
        "Perform memory leak detection, end with `Ctrl + C`"
    )
endif()