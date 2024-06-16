cmake_minimum_required(VERSION 3.10)

project(asiomp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

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

add_library(${PROJECT_NAME} SHARED ${srcs})

target_link_libraries(${PROJECT_NAME} PUBLIC pthread spdlog::spdlog)

if (PROC_NAME)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        ASIOMP_PROC_NAME="${PROC_NAME}"
    )
endif()