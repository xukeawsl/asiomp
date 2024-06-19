cmake_minimum_required(VERSION 3.10)

project(asiomp)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose Release or Debug" FORCE)
endif()

option(SPDLOG_INSTALL "Generate the install target" ON)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wno-unused")

add_subdirectory(third-party/spdlog)

include(cmake/settings.cmake)

file(GLOB_RECURSE srcs ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE hrds ${PROJECT_SOURCE_DIR}/src/*.h)

add_library(${PROJECT_NAME} SHARED ${srcs})

target_include_directories(${PROJECT_NAME} PUBLIC
    ./include
    third-party/asio/asio/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC pthread spdlog::spdlog)

if (PROC_NAME)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        ASIOMP_PROC_NAME="${PROC_NAME}"
    )
endif()

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
        ${CLANG_FORMAT_EXECUTABLE} --style=file -i ${hdrs};${srcs}
    COMMENT
        "Automatically format the code style"
    )
endif()

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    LIBRARY
        DESTINATION lib
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/third-party/asio/asio/include/ DESTINATION include/${PROJECT_NAME})