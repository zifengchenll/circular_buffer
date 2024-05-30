
# Circular Buffer Open Source C Library

![Project Logo](docs/images/project_logo.png)

![GitHub Top Language](https://img.shields.io/github/languages/top/zifengchenll/circular_buffer.svg)
![GitHub Language Count](https://img.shields.io/github/languages/count/zifengchenll/circular_buffer.svg)
![GitHub Release](https://img.shields.io/github/license/zifengchenll/circular_buffer.svg)
![GitHub Release](https://img.shields.io/github/repo-size/zifengchenll/circular_buffer.svg)
![GitHub Release](https://img.shields.io/github/last-commit/zifengchenll/circular_buffer.svg)
![GitHub Release](https://img.shields.io/github/v/release/zifengchenll/circular_buffer.svg)

[English](readme_en.md) | [中文](./readme.md)

Circular Buffer, also known as Circular Queue, is a fixed-size data structure that uses a single, fixed-size buffer as a circular structure. The fixed-size nature of the circular buffer makes its memory usage very efficient, avoiding the overhead and fragmentation issues of dynamic memory allocation, making it very suitable for resource-constrained embedded systems; its main feature is that when the end of the buffer is written full, new data will start overwriting old data from the beginning of the buffer (this repository also supports various strategy configurations).

Circular buffers are widely used in scenarios where efficient handling of streaming data is required, such as audio streams, network packet processing, logging, etc. This repository provides a highly portable circular buffer library written in C, supporting multiple hardware platforms, including Linux and various common embedded platforms. The library features efficient read and write operations, implemented through pointers, without complex memory management;

Supports multiple write strategy configurations, including overwriting old data and rejecting new data, allowing users to choose the most suitable writing method according to specific application scenarios; to ensure data consistency and thread safety in a multi-threaded environment, the library provides a lock mechanism strategy configuration, allowing users to enable or disable the lock mechanism to meet different synchronization needs; the library is designed with cross-platform requirements in mind, with high portability, and can run seamlessly on different hardware platforms, including Linux and various embedded platforms, making it easy to reuse in different projects;

------

## Feature Specifications

| Specification              | Description                                                         |
| -------------------------- | ------------------------------------------------------------------- |
| Supports multiple embedded hardware platforms | The C library supports cross-platform hardware, including Linux kernel, FreeRTOS, bare metal, etc., and is suitable for platforms including STM32, ESP32, ESP8266, BL602, BL616, RTL8720DN, W800, etc. |
| Configurable data write strategy | When the write speed of the buffer is greater than the read speed, the traditional circular buffer will overflow. When the buffer is full, you can choose between overwriting old data or rejecting new data. The default configuration of the repository is to reject new data; you can adjust the write strategy by configuring CIRCULAR_BUFFER_OVERWRITE |
| Configurable lock-free circular buffer | The circular buffer supports lock-free operation mode, configured by setting ENABLE_LOCK macro to 0, switching to lock-free mode. In single producer single consumer mode under the write-full reject new data strategy, it is recommended to use lock-free mode to minimize system overhead; in multi-threaded scenarios under the write-full overwrite old data strategy, locks must be used to ensure thread safety |
| Configurable locked circular buffer | The circular buffer supports locked operation mode, configured by setting ENABLE_LOCK macro to 1, switching to locked mode. In locked mode, regardless of the write-full overwrite old data strategy or write-full reject new data strategy, it can ensure thread safety in multi-producer and multi-consumer multi-threaded scenarios |

## Implementation Principle

The circular buffer uses a fixed-size array and two pointers: a head pointer and a tail pointer. These two pointers indicate the head and tail positions of the data in the buffer. The head pointer points to the position where the next read will occur, and the tail pointer points to the position where the next write will occur. When data is written, the tail pointer moves forward, and when data is read, the head pointer moves forward. When the tail pointer reaches the end of the buffer, it wraps around to the beginning, forming a circular structure. When the head pointer catches up with the tail pointer, it means the buffer is empty, and when the tail pointer catches up with the head pointer, it means the buffer is full.

## Usage

### Compilation and Build

Run the following command to compile the project:

```
make
```

### Example Program

Run the example program:

```
cd ./bin && ./circular_buffer_example
```

### Unit Tests

Run the following command to compile the unit test program:

```
make run_tests
```

Run the unit test program:

```
cd ./bin && ./run_tests
```

View the test case results:

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

## Project Structure

```bash
.
├── bin
│   ├── circular_buffer_example  // Example executable
│   └── run_tests                // Test executable
├── circular_buffer
│   ├── port                     // Platform-specific adaptation files
│   └── src                      // Circular buffer source code
├── circular_buffer_example
│   └── example.c                // Example usage
├── docs
│   ├── api                      // API documentation
│   ├── images                   // Image resources
│   └── reference_file           // Reference materials
├── lib
│   └── libcircular_buffer.a     // Static library
├── license                      // License
├── Makefile                     // Build file
├── readme_en.md
├── readme.md
├── test_case
│   └── test_circular_buffer.c   // Test cases
└── tools                        // Tools
    ├── doxygen                  // API documentation generator
    ├── toolchain                // Cross-compilation toolchain for generating cross-platform lib
    └── unity                    // Testing framework
```

## Notes

When the data write strategy is configured to overwrite old data mode, you must use locks for thread safety protection (except for bare metal, because there is no concept of multi-threading on bare metal platforms, so there is no need for lock protection).

## References

- [Kernel implementation: https://github.com/torvalds/linux](https://github.com/torvalds/linux)
- [Zhihu platform: https://zhuanlan.zhihu.com/p/534098236](https://zhuanlan.zhihu.com/p/534098236)
- [Wikipedia: https://en.wikipedia.org/wiki/Circular_buffer](https://en.wikipedia.org/wiki/Circular_buffer)
