# asiomp 简介

asiomp 能够帮助你构建基于 c++ asio 的多进程 tcp 服务器

## 前提

* 需要支持 C++11 的编译器

* 只支持 Linux 平台 (使用 `fork` 系统调用)

## 特性

* 高可用: 采用类似 `nginx` 的 `master-worker` 工作模式, 当 worker 进程挂掉后可以自动拉起来, 保证正常提供服务

* 高并发: 使用多进程能够很好的利用多核能力, 提高整体处理能力, 推荐可以在工作进程使用 asio 协程, 进一步提高 IO 处理的效率

* 日志: 采用 `spdlog` 第三方日志库, 其中每个进程都有各自对应的日志, 已经设置好了默认的日志记录器, 直接使用即可

## 使用

### 1. 克隆源代码

```bash
git clone --recurse-submodules https://github.com/xukeawsl/asiomp.git
```

### 2. 添加业务代码

* 为 `session` 类添加具体处理代码

```cpp
#include "session.h"

session::session(asio::ip::tcp::socket socket) : socket_(std::move(socket)) {}

session::~session() {}

void session::start() {
    // add your code
}
```

* 根据需求配置监听地址和工作进程数, 修改 `main.cpp` 中的参数即可

```cpp
#include "asiomp.h"

int main(int argc, char *argv[]) {
    // asiomp_server("127.0.0.1", 5555).run(); // 单进程模式
    asiomp_server("127.0.0.1", 5555, 2).run();    // 多进程模式
    return 0;
}
```

### 3. 构建运行

```bash
mkdir build && cd build

# 默认是 Debug 模式
cmake -DCMAKE_BUILD_TYPE=Release ..

# 可以在编译时调整日志级别, 如果没有设置则 Release 构建使用 Info 级别
# Debug 构建使用 Debug 级别
cmake -DLOG_LEVEL=Trace ..

# 编译二进制可执行文件
cmake --build .

# 运行 asiomp
./asiomp
```