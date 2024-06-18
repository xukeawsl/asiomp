# asiomp 简介

[![License](https://img.shields.io/npm/l/mithril.svg)](https://github.com/xukeawsl/asiomp/blob/master/LICENSE)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/23ca8b88cf7f4fe2bc3a4e870d5f795f)](https://app.codacy.com/gh/xukeawsl/asiomp/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Build status](https://ci.appveyor.com/api/projects/status/5epf5t4t8c1wqax4?svg=true)](https://ci.appveyor.com/project/xukeawsl/asiomp)

asiomp 能够帮助你构建基于 c++ asio 的多进程 tcp 服务器

## 前提

* 需要支持 C++11 的编译器

* 只支持 Linux 平台 (使用 `fork` 系统调用)

## 特性

* 高可用: 采用类似 `nginx` 的 `master-worker` 工作模式, 当 worker 进程挂掉后可以自动拉起来, 保证正常提供服务, 支持守护进程模式工作

* 高并发: 使用多进程能够很好的利用多核能力, 提高整体处理能力, 推荐可以在工作进程使用 asio 协程, 进一步提高 IO 处理的效率

* 日志: 采用 `spdlog` 第三方日志库, 其中每个进程都有各自对应的日志, 已经设置好了默认的日志记录器, 直接使用即可

## 建议

* 为了更好的发挥多进程的优势, 建议在会话可以独立工作的场景下使用, 例如在项目 [coro_socks](https://github.com/xukeawsl/coro_socks) 中使用多进程加协程实现了一个高性能的 socks5 代理服务器

* 如果会话间需要通信交互等, 建议使用 asio 封装的 unix 域套接字或者 tcp、udp 套接字, 可以很好地融入 asio 的上下文, 并且不需要引入锁

* 如果需要实现类似聊天室这种需要频繁转发消息的功能, 建议使用单进程模式即可, 如果要使用多进程, 则需要进程间通信来向所有连接分发消息, 示例 [chat](https://github.com/xukeawsl/asiomp/tree/master/example/chat) 中使用 unix 域套接字实现了一个简单的聊天室以供参考

## 使用

### 1. 作为子模块引入项目

```bash
# 对于一个新项目
git init
mkdir third-party

# 添加子模块
git submodule add https://github.com/xukeawsl/asiomp.git third-party/asiomp

# 递归拉取所有子模块代码
git submodule update --init --recursive
```

### 2. 在 CMakeLists 中添加子目录并链接动态库

```cmake
add_subdirectory(third-party/asiomp)

target_link_libraries(${PROJECT_NAME} PUBLIC asiomp)
```

### 3. 添加业务代码

* 继承 `session` 类并实现其 `start` 方法

```cpp
#include "asiomp.h"

class your_session : public session {
public:
    explicit your_session(asio::ip::tcp::socket socket) : socket_(std::move(socket)) {
        // 其它初始化操作
    }

    void start() override {
        // 注意如果使用 asio 的异步操作, 需要将智能指针提升
        // 为 std::shared_ptr<your_session> 才行, 可以使用 session 中
        // 定义的 getDerivedSharedPtr<your_session> 模板方法
        auto self = getDeriveSharedPtr<your_session>();
        self->handle_session();

        // 如果不使用异步方法直接调用即可
        // this->handle_session();
    }

private:
    void handle_session() {
        // 会话处理
    }

private:
    asio::ip::tcp::socket socket_;
};
```

* 将自己实现的会话类注册到会话工厂中, 需要提供一个会话生成器, 最后在创建服务器时提供会话名称即可

```cpp
#include "asiomp.h"
#include "your_session.h"

std::shared_ptr<session> your_session_creator(asio::ip::tcp::socket socket) {
    auto sptr = std::make_shared<your_session>(std::move(socket));
    return std::dynamic_pointer_cast<session>(sptr);
}

/* asiomp 参数解释
   param1: argv 用于修改运行后的进程名称, 可以使用 cmake -DPROC_NAME=your_proc_name 设置
   param2: 服务器监听的 ip 地址, 支持 IPv6
   param3: 服务器监听端口
   param4: worker 进程个数, 如果为 0 则取 CPU 核心数(单进程模式无这个参数)
   param5: 是否作为守护进程运行
   param6: 会话名称(如果对应会话没有注册, 则使用默认会话)
*/
int main(int argc, char *argv[]) {
    session_factory::getInstance()->register_session("your_session", your_session_creator);
    asiomp_server(argv, "127.0.0.1", 80, 2, false, "your_session").run();
    return 0;
}
```

### 4. 构建运行

```bash
mkdir build && cd build

# 默认是 Debug 模式
cmake -DCMAKE_BUILD_TYPE=Release ..

# 可以在编译时调整日志级别, 如果没有设置则 Release 构建使用 Info 级别
# Debug 构建使用 Debug 级别
cmake -DLOG_LEVEL=Trace ..

# 可以在构建时调整可执行文件运行后的名称, 默认是 asiomp
cmake -DPROC_NAME=echo_server ..

# 编译二进制可执行文件
cmake --build .

# 假如你的可执行文件名是 echo
./echo
```

### 5. 安装

```cmake
# 安装 asiomp 以及依赖的 spdlog、asio
# 如果不需要安装 spdlog 则不设置 SDPLOG_INSTALL 选项即可
cmake -DSPDLOG_INSTALL=ON ..

# 安装到默认路径
cmake --install .

# 安装到指定目录开头的路径
cmake --install . --prefix /usr/local/test

# 安装完之后注意一下头文件路径和库文件路径是否能在环境变量中找到, 否则需要再添加一下环境变量
```