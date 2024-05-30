
# 环形缓冲区开源C库

![项目Logo](docs/images/project_logo.png)

![GitHub Top Language](https://img.shields.io/github/languages/top/zifengchenll/circular_buffer.svg)
![GitHub Language Count](https://img.shields.io/github/languages/count/zifengchenll/circular_buffer.svg)
![GitHub Release](https://img.shields.io/github/license/zifengchenll/circular_buffer.svg)
![GitHub Release](https://img.shields.io/github/repo-size/zifengchenll/circular_buffer.svg)
![GitHub Release](https://img.shields.io/github/last-commit/zifengchenll/circular_buffer.svg)
![GitHub Release](https://img.shields.io/github/v/release/zifengchenll/circular_buffer.svg)

[English](readme_en.md) | [中文](./readme.md)

环形缓冲区（Circular Buffer），也称为环形队列（Circular Queue），是一种固定大小的数据结构，它使用单个、固定大小的缓冲区作为一个环形结构。环形缓冲区的固定大小特性使其内存使用非常高效，避免了动态内存分配带来的开销和内存碎片问题，非常适合资源受限的嵌入式系统；它的主要特点是，当缓冲区的末尾被写满后，新的数据将从缓冲区的起始位置开始覆盖旧的数据（本仓库另外还支持多种策略配置）。

环形缓冲区广泛应用于需要高效处理流式数据的场景，如音频流、网络数据包处理、日志记录等。本仓库提供了一个使用C语言编写的、具备高移植性的环形缓冲区库，支持多种硬件平台，包括Linux平台和各类常见的嵌入式平台。该库具有高效的读写操作，通过指针实现，无需复杂的内存管理；

支持多种写入策略配置，包括覆盖旧数据策略和拒绝新数据策略，使用户可以根据具体应用场景选择最合适的写入方式；为了在多线程环境下保证数据的一致性和线程安全，库中提供了锁机制策略配置，用户可以选择启用或禁用锁机制，以满足不同的同步需求；库的设计考虑了跨平台的需求，具备高移植性，可以在不同的硬件平台上无缝运行，包括Linux平台和各种嵌入式平台，方便在不同项目中复用；

------

## 特性规格

| 规格                   | 详细描述                                                     |
| ---------------------- | ------------------------------------------------------------ |
| 支持多种嵌入式硬件平台 | C库支持硬件跨平台，已经适配的系统包括Linux内核、FreeRTOS、裸机等，同时适用与包括STM32、ESP32、ESP8266、BL602、BL616、RTL8720DN、W800等平台 |
| 支持数据写入策略可配置 | 当缓冲区的写入速度大于读取速度时，传统环形缓冲区会出现缓冲区满溢问题。当缓冲区写满时，可选择覆盖旧数据或者拒绝新数据两种策略，仓库默认配置是使用拒绝新数据策略；可以通过配置CIRCULAR_BUFFER_OVERWRITE来调整写入策略 |
| 可配置无锁环形缓冲区   | 环形缓冲区支持无锁工作模式，通过ENABLE_LOCK宏定义配置成0，切换成无锁工作模式，在写满拒绝新数据策略下，并且处于单生产者单消费者模式，推荐使用无锁工作模式，以最低限度降低系统开销；若在写满覆盖旧数据策略下，多线程场景必须使用锁机制，以确保线程安全 |
| 可配置有锁环形缓冲区   | 环形缓冲区支持有锁工作模式，通过ENABLE_LOCK宏定义配置成1，切换成有锁工作模式，在有锁工作模式下，无论是写满覆盖旧数据策略和写满拒绝新数据策略，多生产者和多消费者，多线程场合，都是可以保证线程安全的 |

## 实现原理

讲述这个仓库的实现原理，我这里也是拷贝一段语句填充：环形缓冲区使用一个固定大小的数组和两个指针：一个头指针（head）和一个尾指针（tail）。这两个指针指示缓冲区的起始位置和结束位置。数据从尾指针的位置写入，从头指针的位置读取。当指针到达数组的末端时，它们会环绕回数组的起点，形成一个环形结构。这种设计避免了数据移动的开销，使得缓冲区能够高效地进行读写操作。

## 安装步骤

请按照以下步骤安装和配置项目：

克隆仓库：

```bash
git clone https://github.com/zifengchenll/circular_buffer.git
```

进入目录：

```bash
cd circular_buffer/
```

## 使用说明

编译所有目标，包括可执行文件和静态库：

```c
make
```

仅编译静态库并将其放入 `lib` 目录

```
make lib
```

编译测试文件并运行测试

```
make run_tests && ./run_tests
```

使用指定路径的工具链编译静态库

```
make lib CC=/path/to/riscv64-unknown-elf-gcc AR=/path/to/riscv64-unknown-elf-ar
```

清理生成的文件，但保留 `lib` 目录

```
make clean
```

## 测试说明

编译单元测试用例：

项目采用unity测试框架，测试用例文件文件路径在./test_case/test_circular_buffer.c，可以通过这个测试用例，对环形缓冲区进行全量测试

```bash
make run_tests 
```

运行单元测试程序：

```
cd ./bin && ./run_tests
```

查看测试用例结果：

```
test_case/test_circular_buffer.c:307:test_circular_buffer_init:PASS
test_case/test_circular_buffer.c:308:test_circular_buffer_write_and_read:PASS
test_case/test_circular_buffer.c:309:test_circular_buffer_full:PASS
test_case/test_circular_buffer.c:310:test_circular_buffer_empty:PASS
test_case/test_circular_buffer.c:311:test_circular_buffer_boundary:PASS
test_case/test_circular_buffer.c:312:test_circular_buffer_random_operations:PASS
test_case/test_circular_buffer.c:313:test_circular_buffer_stress_test:PASS
test_case/test_circular_buffer.c:314:test_circular_buffer_concurrent:PASS

-----------------------
8 Tests 0 Failures 0 Ignored 
OK
```

## 项目结构

```bash
.
├── bin
│   ├── circular_buffer_example  // 例程执行文件
│   └── run_tests                // 用例执行文件
├── circular_buffer
│   ├── port                     // 跨平台需要适配的文件
│   └── src                      // 环形缓冲区源码
├── circular_buffer_example
│   ├── example.c                // 使用例程
├── docs
│   ├── api                      // API文档
│   ├── images                   // 图片素材
│   └── reference_file           // 相关资料
├── lib
│   └── libcircular_buffer.a     // 静态库
├── license                      // 许可证明
├── Makefile                     // 工程文件
├── readme_en.md
├── readme.md
├── test_case
│   ├── test_circular_buffer.c   // 测试用例
└── tools                        // 工具目录
    ├── doxygen                  // API文档生成工具
    ├── toolchain                // 交叉编译工具链，用于生成跨平台lib
    └── unity                    // 测试框架

```

## 注意事项

在写入数据策略配置成覆盖旧数据模式下，一定要通过锁进行线程安全保护（裸机除外，因为裸机平台，没有多线程的概念，不需要通过锁进行保护）

## 参考文献

- [内核实现：https://github.com/torvalds/linux](https://github.com/torvalds/linux)
- [知乎平台：https://zhuanlan.zhihu.com/p/534098236](https://zhuanlan.zhihu.com/p/534098236)
- [维基百科：https://en.wikipedia.org/wiki/Circular_buffer](https://en.wikipedia.org/wiki/Circular_buffer)
