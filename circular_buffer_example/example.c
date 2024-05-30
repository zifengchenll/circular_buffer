// main.c
#include <stdio.h>
#include <string.h>
#include "circular_buffer.h"

/**
 * @brief 测试环形缓冲区功能
 */
void test_circular_buffer() {
    circular_buffer cb;
    const size_t buffer_size = 16; // 缓冲区大小为2的幂次
    char write_data[] = "1234567890";
    char read_data[buffer_size]; // 缓冲区大小

    if (!circular_buffer_init(&cb, buffer_size)) {
        printf("初始化环形缓冲区失败\n");
        return;
    }

    printf("写入数据到环形缓冲区: %s\n", write_data);
    if (circular_buffer_write(&cb, write_data, strlen(write_data))) {
        printf("写入成功\n");
    } else {
        printf("写入失败\n");
    }

    printf("环形缓冲区中的有效数据长度: %zu\n", circular_buffer_length(&cb));
    printf("环形缓冲区是否为空: %s\n", circular_buffer_is_empty(&cb) ? "是" : "否");
    printf("环形缓冲区是否已满: %s\n", circular_buffer_is_full(&cb) ? "是" : "否");

    printf("从环形缓冲区读取数据\n");
    size_t length_to_read = strlen(write_data);
    if (circular_buffer_read(&cb, read_data, length_to_read)) {
        read_data[length_to_read] = '\0'; // 确保字符串以空字符结尾
        printf("读取成功: %s\n", read_data);
    } else {
        printf("读取失败\n");
    }

    printf("环形缓冲区中的有效数据长度: %zu\n", circular_buffer_length(&cb));
    printf("环形缓冲区是否为空: %s\n", circular_buffer_is_empty(&cb) ? "是" : "否");
    printf("环形缓冲区是否已满: %s\n", circular_buffer_is_full(&cb) ? "是" : "否");

    circular_buffer_free(&cb);
    printf("释放环形缓冲区\n");
}

/**
 * @brief 主函数
 * 
 * @return int 
 */
int main() {
    printf("开始测试环形缓冲区\n");
    test_circular_buffer();
    printf("测试完成\n");
    return 0;
}
