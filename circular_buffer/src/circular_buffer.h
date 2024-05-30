// circular_buffer.h
#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include "port.h"

// 策略宏定义（1 表示覆盖旧数据，0 表示丢弃新数据）
#define CIRCULAR_BUFFER_OVERWRITE 0

/**
 * @brief 环形缓冲区结构体
 */
typedef struct
{
    size_t size;   /**< 缓冲区大小（必须为2的幂次） */
    size_t start;  /**< 起始位置（读取位置） */
    size_t end;    /**< 结束位置（写入位置） */
    char *buffer;  /**< 缓冲区数据指针 */
    mutex_t mutex; /**< 平台无关的互斥锁 */
} circular_buffer;

/**
 * @brief 初始化环形缓冲区
 *
 * @param cb 环形缓冲区结构体指针
 * @param size 缓冲区大小（必须为2的幂次）
 * @return 成功返回true，失败返回false
 */
bool circular_buffer_init(circular_buffer *cb, size_t size);

/**
 * @brief 释放环形缓冲区资源
 *
 * @param cb 环形缓冲区结构体指针
 */
void circular_buffer_free(circular_buffer *cb);

/**
 * @brief 向环形缓冲区写入数据
 *
 * @param cb 环形缓冲区结构体指针
 * @param data 写入数据的指针
 * @param length 写入数据的长度
 * @return 成功返回true，失败返回false
 */
bool circular_buffer_write(circular_buffer *cb, const char *data, size_t length);

/**
 * @brief 从环形缓冲区读取数据
 *
 * @param cb 环形缓冲区结构体指针
 * @param data 读取数据的指针
 * @param length 读取数据的长度
 * @return 成功返回true，失败返回false
 */
bool circular_buffer_read(circular_buffer *cb, char *data, size_t length);

/**
 * @brief 获取环形缓冲区中的有效数据长度
 *
 * @param cb 环形缓冲区结构体指针
 * @return 有效数据长度
 */
size_t circular_buffer_length(circular_buffer *cb);

/**
 * @brief 检查环形缓冲区是否为空
 *
 * @param cb 环形缓冲区结构体指针
 * @return 为空返回true，不为空返回false
 */
bool circular_buffer_is_empty(circular_buffer *cb);

/**
 * @brief 检查环形缓冲区是否已满
 *
 * @param cb 环形缓冲区结构体指针
 * @return 已满返回true，未满返回false
 */
bool circular_buffer_is_full(circular_buffer *cb);

#endif // CIRCULAR_BUFFER_H
