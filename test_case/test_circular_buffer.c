#include "unity.h"
#include "circular_buffer.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>

// 定义宏以启用或禁用日志
// #define ENABLE_LOGGING

#ifdef ENABLE_LOGGING
#define LOG(fmt, ...)                printf(fmt, ##__VA_ARGS__)
#define PRINT_BUFFER(buffer, length) print_buffer(buffer, length)
#else
#define LOG(fmt, ...)                                                                                                                                \
    do                                                                                                                                               \
    {                                                                                                                                                \
    } while (0)
#define PRINT_BUFFER(buffer, length)                                                                                                                 \
    do                                                                                                                                               \
    {                                                                                                                                                \
    } while (0)
#endif

// 初始化测试环境，每个测试函数运行前都会调用这个函数
void setUp(void)
{
    // 可以在这里进行初始化操作
}

// 清理测试环境，每个测试函数运行后都会调用这个函数
void tearDown(void)
{
    // 可以在这里进行清理操作
}

// 打印缓冲区内容，供调试使用
void print_buffer(const char *buffer, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        printf("%02X ", (unsigned char)buffer[i]);
    }
    printf("\n");
}

// 测试环形缓冲区初始化
void test_circular_buffer_init(void)
{
    circular_buffer cb;
    size_t buffer_size = 16;
    // 验证初始化是否成功
    TEST_ASSERT_TRUE(circular_buffer_init(&cb, buffer_size));
    // 释放缓冲区
    circular_buffer_free(&cb);
}

// 测试环形缓冲区的写入和读取功能
void test_circular_buffer_write_and_read(void)
{
    circular_buffer cb;
    size_t buffer_size = 16;
    char write_data[] = "1234567890";
    char read_data[buffer_size];

    // 初始化缓冲区
    TEST_ASSERT_TRUE(circular_buffer_init(&cb, buffer_size));

    // 写入数据
    TEST_ASSERT_TRUE(circular_buffer_write(&cb, write_data, strlen(write_data)));

    // 读取数据
    TEST_ASSERT_TRUE(circular_buffer_read(&cb, read_data, strlen(write_data)));
    read_data[strlen(write_data)] = '\0'; // 确保字符串以空字符结尾

    // 验证读取的数据是否与写入的数据一致
    TEST_ASSERT_EQUAL_STRING(write_data, read_data);

    // 释放缓冲区
    circular_buffer_free(&cb);
}

// 测试环形缓冲区满缓冲区情况
void test_circular_buffer_full(void)
{
    circular_buffer cb;
    size_t buffer_size = 16;
    char write_data[buffer_size];
    char read_data[buffer_size];

    // 初始化缓冲区
    TEST_ASSERT_TRUE(circular_buffer_init(&cb, buffer_size));

    // 写入数据直到缓冲区满
    for (size_t i = 0; i < buffer_size - 1; i++)
    {
        write_data[i] = i;
    }
    TEST_ASSERT_TRUE(circular_buffer_write(&cb, write_data, buffer_size - 1));
    // 尝试写入超出缓冲区大小的数据
    TEST_ASSERT_FALSE(circular_buffer_write(&cb, write_data, 1));

    // 读取数据
    TEST_ASSERT_TRUE(circular_buffer_read(&cb, read_data, buffer_size - 1));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(write_data, read_data, buffer_size - 1);

    // 释放缓冲区
    circular_buffer_free(&cb);
}

// 测试环形缓冲区空缓冲区情况
void test_circular_buffer_empty(void)
{
    circular_buffer cb;
    size_t buffer_size = 16;
    char read_data[buffer_size];

    // 初始化缓冲区
    TEST_ASSERT_TRUE(circular_buffer_init(&cb, buffer_size));

    // 尝试从空缓冲区读取数据
    TEST_ASSERT_FALSE(circular_buffer_read(&cb, read_data, 1));

    // 释放缓冲区
    circular_buffer_free(&cb);
}

// 测试环形缓冲区边界情况
void test_circular_buffer_boundary(void)
{
    circular_buffer cb;
    size_t buffer_size = 16;
    char write_data[buffer_size];
    char read_data[buffer_size];

    // 初始化缓冲区
    TEST_ASSERT_TRUE(circular_buffer_init(&cb, buffer_size));

    // 第一次写入数据直到缓冲区满
    for (size_t i = 0; i < buffer_size - 1; i++)
    {
        write_data[i] = i;
    }
    LOG("写入数据（缓冲区字节数减一）: ");
    PRINT_BUFFER(write_data, buffer_size - 1);
    TEST_ASSERT_TRUE(circular_buffer_write(&cb, write_data, buffer_size - 1));

    TEST_ASSERT_TRUE(circular_buffer_read(&cb, read_data, buffer_size - 1));
    LOG("读取数据（缓冲区字节数减一）: ");
    PRINT_BUFFER(read_data, buffer_size - 1);

    LOG("写入数据（等于缓冲区字节数）: ");
    PRINT_BUFFER(write_data, buffer_size);
    TEST_ASSERT_FALSE(circular_buffer_write(&cb, write_data, buffer_size));

    TEST_ASSERT_FALSE(circular_buffer_read(&cb, read_data, buffer_size - 1));
    LOG("读取数据（缓冲区字节数减一）: ");
    PRINT_BUFFER(read_data, buffer_size - 1);

    // 释放缓冲区
    circular_buffer_free(&cb);
}

// 测试环形缓冲区随机操作
void test_circular_buffer_random_operations(void)
{
    circular_buffer cb;
    size_t buffer_size = 1024;
    char write_data[buffer_size];
    char read_data[buffer_size];

    // 初始化缓冲区
    TEST_ASSERT_TRUE(circular_buffer_init(&cb, buffer_size));

    // 初始化随机数生成器
    srand(time(NULL));
    for (int i = 0; i < 1000; i++)
    {
        // 随机生成写入数据的长度，确保不为0
        size_t length = (rand() % (buffer_size / 2)) + 1;
        // 随机生成写入的数据
        for (size_t j = 0; j < length; j++)
        {
            write_data[j] = rand() % 256;
        }

        LOG("写入数据: ");
        PRINT_BUFFER(write_data, length);

        // 写入数据到缓冲区
        if (circular_buffer_write(&cb, write_data, length))
        {
            // 读取数据
            TEST_ASSERT_TRUE(circular_buffer_read(&cb, read_data, length));

            LOG("读取数据: ");
            PRINT_BUFFER(read_data, length);

            // 验证读取的数据是否与写入的数据一致
            TEST_ASSERT_EQUAL_UINT8_ARRAY(write_data, read_data, length);
        }
    }

    // 释放缓冲区
    circular_buffer_free(&cb);
}

// 压力测试
void test_circular_buffer_stress_test(void)
{
    circular_buffer cb;
    size_t buffer_size = 1024;
    char write_data[buffer_size];
    char read_data[buffer_size];

    // 初始化缓冲区
    TEST_ASSERT_TRUE(circular_buffer_init(&cb, buffer_size));

    // 初始化随机数生成器
    srand(time(NULL));
    for (int i = 0; i < 10000; i++)
    {
        // 随机生成写入数据的长度，确保不为0
        size_t length = (rand() % (buffer_size / 2)) + 1;
        // 随机生成写入的数据
        for (size_t j = 0; j < length; j++)
        {
            write_data[j] = rand() % 256;
        }

        LOG("写入数据: ");
        PRINT_BUFFER(write_data, length);

        // 写入数据到缓冲区
        TEST_ASSERT_TRUE(circular_buffer_write(&cb, write_data, length));
        // 读取数据
        TEST_ASSERT_TRUE(circular_buffer_read(&cb, read_data, length));

        LOG("读取数据: ");
        PRINT_BUFFER(read_data, length);

        // 验证读取的数据是否与写入的数据一致
        TEST_ASSERT_EQUAL_UINT8_ARRAY(write_data, read_data, length);
    }

    // 释放缓冲区
    circular_buffer_free(&cb);
}

// 并发读写测试
void *writer_thread(void *arg)
{
    circular_buffer *cb = (circular_buffer *)arg;
    size_t buffer_size = 1024;
    char write_data[buffer_size];

    srand(time(NULL));
    for (int i = 0; i < 1000; i++)
    {
        size_t length = (rand() % (buffer_size / 2)) + 1;
        for (size_t j = 0; j < length; j++)
        {
            write_data[j] = rand() % 256;
        }
        circular_buffer_write(cb, write_data, length);
    }
    return NULL;
}

void *reader_thread(void *arg)
{
    circular_buffer *cb = (circular_buffer *)arg;
    size_t buffer_size = 1024;
    char read_data[buffer_size];

    for (int i = 0; i < 1000; i++)
    {
        size_t length = (rand() % (buffer_size / 2)) + 1;
        circular_buffer_read(cb, read_data, length);
    }
    return NULL;
}

void test_circular_buffer_concurrent(void)
{
    circular_buffer cb;
    size_t buffer_size = 1024;

    TEST_ASSERT_TRUE(circular_buffer_init(&cb, buffer_size));

    pthread_t writer, reader;
    pthread_create(&writer, NULL, writer_thread, &cb);
    pthread_create(&reader, NULL, reader_thread, &cb);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    circular_buffer_free(&cb);
}

// 主函数运行所有测试
int main(void)
{
    UNITY_BEGIN(); // 启动Unity测试框架

    // 运行各个测试函数
    RUN_TEST(test_circular_buffer_init);
    RUN_TEST(test_circular_buffer_write_and_read);
    RUN_TEST(test_circular_buffer_full);
    RUN_TEST(test_circular_buffer_empty);
    RUN_TEST(test_circular_buffer_boundary);
    RUN_TEST(test_circular_buffer_random_operations);
    RUN_TEST(test_circular_buffer_stress_test);
    RUN_TEST(test_circular_buffer_concurrent);

    return UNITY_END(); // 结束Unity测试框架
}
