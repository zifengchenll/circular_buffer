// circular_buffer.c
#include "circular_buffer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief 检查是否为2的幂
 *
 * @param size 待检查的大小
 * @return 是2的幂返回true，否则返回false
 */
static bool is_power_of_two(size_t size)
{
    // 检查size是否为2的幂次
    // 2的幂次在二进制表示中只有一个1，其余位都是0
    // 例如，4的二进制表示是100，8的二进制表示是1000

    // 首先检查size是否不为0
    // 如果size为0，则不是2的幂次，因为2的任何幂都不可能为0
    // 所以我们需要确保size不为0

    // 现在考虑size为非0的情况
    // 利用位操作来检查size是否是2的幂次
    // 具体来说，我们使用表达式 (size & (size - 1)) == 0
    // 其原理如下：

    // 对于一个数size，如果它是2的幂次，例如4 (100) 或8 (1000)
    // 它的二进制表示中只有一个1，其余位都是0

    // 当我们从size中减去1时
    // 例如，4 - 1 = 3，二进制表示为100 - 1 = 011
    // 或者8 - 1 = 7，二进制表示为1000 - 1 = 0111

    // 减1操作会将size的二进制表示中的最低位的1变为0
    // 并将这个1右边的所有0变为1

    // 现在，考虑按位与操作 size & (size - 1)
    // 对于4：100 & 011 = 000
    // 对于8：1000 & 0111 = 0000

    // 可以看到，如果size是2的幂次，size & (size - 1) 的结果必然是0
    // 因为在减去1之后，最低位的1被清除了，且没有其他相同的位会保留1

    // 如果size不是2的幂次，例如5 (101)
    // 5 - 1 = 4，二进制表示为101 - 1 = 100
    // size & (size - 1) = 101 & 100 = 100
    // 结果不为0，因为原来的最低位的1并没有被清除

    // 因此，通过检查 (size & (size - 1)) == 0
    // 我们可以判断size是否是2的幂次
    return (size != 0) && ((size & (size - 1)) == 0);
}


/**
 * @brief 初始化环形缓冲区
 *
 * @param cb 环形缓冲区结构体指针
 * @param size 缓冲区大小（必须为2的幂次），真正可以用于存储数据的长度为 size-1
 * @return 成功返回true，失败返回false
 */
bool circular_buffer_init(circular_buffer *cb, size_t size)
{
    // 检查size是否为2的幂次
    if (size == 0 || !is_power_of_two(size))
    {
        return false;                      // 缓冲区大小必须为2的幂次
    }
    cb->size = size;                       // 设置缓冲区大小
    cb->start = 0;                         // 初始化起始位置为0
    cb->end = 0;                           // 初始化结束位置为0
    cb->buffer = (char *)malloc(cb->size); // 分配缓冲区空间
    if (cb->buffer == NULL)
    {
        return false;                      // 分配失败返回false
    }
    memset(cb->buffer, 0, cb->size);       // 清零缓冲区内存
    // 初始化互斥锁，防止多线程竞争
    if (!mutex_init(&cb->mutex))
    {                     // 初始化互斥锁
        free(cb->buffer); // 若互斥锁初始化失败，释放已分配的内存
        return false;     // 互斥锁初始化失败返回false
    }
    return true;          // 成功初始化缓冲区
}

/**
 * @brief 释放环形缓冲区资源
 *
 * @param cb 环形缓冲区结构体指针
 */
void circular_buffer_free(circular_buffer *cb)
{
    if (cb->buffer)
    {
        free(cb->buffer);      // 释放缓冲区内存
        cb->buffer = NULL;     // 将指针置空，避免野指针
    }
    mutex_destroy(&cb->mutex); // 销毁互斥锁
    cb->size = 0;              // 重置缓冲区大小
    cb->start = 0;             // 重置起始位置
    cb->end = 0;               // 重置结束位置
}

/**
 * @brief 向环形缓冲区写入数据
 *
 * @param cb 环形缓冲区结构体指针
 * @param data 写入数据的指针
 * @param length 写入数据的长度
 * @return 成功返回true，失败返回false
 */
bool circular_buffer_write(circular_buffer *cb, const char *data, size_t length)
{
    if (length == 0)
    {
        return false; // 写入数据长度不能为0
    }

    DEBUG_PRINT("尝试获取写锁\n");
    mutex_lock(&cb->mutex); // 加锁，防止多线程同时访问缓冲区
    DEBUG_PRINT("已获取写锁\n");

    // 计算缓冲区内的有效数据长度
    // 使用位操作确保索引在0到size-1范围内，处理缓冲区环绕情况
    // 例如，假设缓冲区大小为8，则掩码为0111
    // 比如end=10, start=2，则current_length = (10-2) & 7 = 8 & 7 = 0
    // 也就是说，end的值大于size时，会自动回绕到缓冲区起始位置
    // 图示:
    // start
    //  |
    // [ ][ ][X][X][X][ ][ ][ ]
    //          |
    //         end
    // current_length = (end - start) & (size - 1)
    //                = (10 - 2) & (8 - 1)
    //                = 8 & 7
    //                = 0
    size_t current_length = (cb->end - cb->start) & (cb->size - 1);
    // 计算缓冲区剩余空间大小
    // 缓冲区总大小减去当前有效数据长度再减1，以区分缓冲区满和空的状态
    // 若缓冲区大小为8，current_length = 5，则available_space = 8 - 5 - 1 = 2
    // 图示:
    // 缓冲区大小为8
    // start
    //  |
    // [ ][ ][X][X][X][X][ ][ ]
    //              |
    //             end
    // available_space = size - current_length - 1
    //                 = 8 - 5 - 1
    //                 = 2
    size_t available_space = cb->size - current_length - 1;

    // 如果剩余空间不足以写入新数据
    if (available_space < length)
    {
#if CIRCULAR_BUFFER_OVERWRITE
        // 覆盖旧数据
        // 如果新数据长度大于可用空间，计算需要覆盖的字节数
        // 若length = 4，则excess = 4 - 2 = 2
        size_t excess = length - available_space;
        // 更新起始位置，将起始位置向前移动excess个位置，使用环绕效果
        // 假设start = 2, excess = 2, size = 8
        // new_start = (2 + 2) & 7 = 4 & 7 = 4
        cb->start = (cb->start + excess) & (cb->size - 1);
#else
        DEBUG_PRINT("缓冲区空间不足，无法写入\n");
        mutex_unlock(&cb->mutex); // 解锁
        return false;             // 缓冲区空间不足，丢弃新数据
#endif
    }

    // 循环写入数据到缓冲区
    for (size_t i = 0; i < length; ++i)
    {
        cb->buffer[cb->end] = data[i]; // 将数据写入缓冲区
        // 更新结束位置，使用环绕效果
        // 假设end = 3, size = 8, end = (3+1) & 7 = 4 & 7 = 4
        // 图示:
        // 写入数据前
        // start
        //  |
        // [ ][ ][ ][ ][ ][ ][ ][ ]
        //    |
        //   end
        // 写入数据后
        // start
        //  |
        // [ ][ ][X][ ][ ][ ][ ][ ]
        //      |
        //     end
        cb->end = (cb->end + 1) & (cb->size - 1);
    }

    DEBUG_PRINT("写入完成，释放写锁\n");
    mutex_unlock(&cb->mutex); // 解锁
    return true;              // 数据写入成功
}

/**
 * @brief 从环形缓冲区读取数据
 *
 * @param cb 环形缓冲区结构体指针
 * @param data 读取数据的指针
 * @param length 读取数据的长度
 * @return 成功返回true，失败返回false
 */
bool circular_buffer_read(circular_buffer *cb, char *data, size_t length)
{
    if (length == 0)
    {
        return false; // 读取数据长度不能为0
    }

    DEBUG_PRINT("尝试获取读锁\n");
    mutex_lock(&cb->mutex); // 加锁，防止多线程同时访问缓冲区
    DEBUG_PRINT("已获取读锁\n");

    // 计算缓冲区内的有效数据长度
    // 使用位操作确保索引在0到size-1范围内，处理缓冲区环绕情况
    // 若end=6, start=2, size=8, current_length = (6-2) & 7 = 4 & 7 = 4
    // 图示:
    // start
    //  |
    // [ ][ ][X][X][X][X][ ][ ]
    //              |
    //             end
    // current_length = (end - start) & (size - 1)
    //                = (6 - 2) & (8 - 1)
    //                = 4 & 7
    //                = 4
    size_t current_length = (cb->end - cb->start) & (cb->size - 1);
    // 如果有效数据长度小于请求的读取长度，则无法读取
    if (current_length < length)
    {
        DEBUG_PRINT("缓冲区数据不足，无法读取\n");
        mutex_unlock(&cb->mutex); // 解锁
        return false;             // 缓冲区数据不足
    }

    // 循环从缓冲区读取数据
    for (size_t i = 0; i < length; ++i)
    {
        data[i] = cb->buffer[cb->start]; // 从缓冲区读取数据
        // 更新起始位置，使用环绕效果
        // 假设start = 2, size = 8, start = (2+1) & 7 = 3 & 7 = 3
        // 图示:
        // 读取数据前
        // start
        //  |
        // [ ][ ][X][ ][ ][ ][ ][ ]
        //      |
        //     end
        // 读取数据后
        //      start
        //       |
        // [ ][ ][ ][ ][ ][ ][ ][ ]
        //      |
        //     end
        cb->start = (cb->start + 1) & (cb->size - 1);
    }

    DEBUG_PRINT("读取完成，释放读锁\n");
    mutex_unlock(&cb->mutex); // 解锁
    return true;              // 数据读取成功
}

/**
 * @brief 获取环形缓冲区中的有效数据长度
 *
 * @param cb 环形缓冲区结构体指针
 * @return 有效数据长度
 */
size_t circular_buffer_length(circular_buffer *cb)
{
    size_t length;
    mutex_lock(&cb->mutex); // 加锁，防止多线程同时访问缓冲区
    // 计算有效数据长度，使用位操作实现环绕效果
    // 若end=6, start=2, size=8, length = (6-2) & 7 = 4 & 7 = 4
    // 图示:
    // start
    //  |
    // [ ][ ][X][X][X][X][ ][ ]
    //              |
    //             end
    // length = (end - start) & (size - 1)
    //        = (6 - 2) & (8 - 1)
    //        = 4 & 7
    //        = 4
    length = (cb->end - cb->start) & (cb->size - 1);
    mutex_unlock(&cb->mutex); // 解锁
    return length;
}

/**
 * @brief 检查环形缓冲区是否为空
 *
 * @param cb 环形缓冲区结构体指针
 * @return 为空返回true，不为空返回false
 */
bool circular_buffer_is_empty(circular_buffer *cb)
{
    bool is_empty;
    mutex_lock(&cb->mutex); // 加锁，防止多线程同时访问缓冲区
    // 起始位置等于结束位置表示缓冲区为空
    // 若start=3, end=3, 则缓冲区为空
    // 图示:
    // start/end
    //  |
    // [ ][ ][ ][ ][ ][ ][ ][ ]
    //  |
    // start/end
    is_empty = (cb->start == cb->end);
    mutex_unlock(&cb->mutex); // 解锁
    return is_empty;
}

/**
 * @brief 检查环形缓冲区是否已满
 *
 * @param cb 环形缓冲区结构体指针
 * @return 已满返回true，未满返回false
 */
bool circular_buffer_is_full(circular_buffer *cb)
{
    bool is_full;
    mutex_lock(&cb->mutex); // 加锁，防止多线程同时访问缓冲区
    // 检查缓冲区是否已满，使用位操作实现环绕效果
    // 当结束位置的下一个位置等于起始位置时，缓冲区已满
    // 若end=7, start=0, size=8, is_full = ((7+1) & 7) == 0 = 0 == 0 = true
    // 图示:
    // start
    //  |
    // [ ][X][X][X][X][X][X][X]
    //                     |
    //                    end
    // is_full = ((end + 1) & (size - 1)) == start
    //         = ((7 + 1) & (8 - 1)) == 0
    //         = (8 & 7) == 0
    //         = 0 == 0
    //         = true
    is_full = ((cb->end + 1) & (cb->size - 1)) == cb->start;
    mutex_unlock(&cb->mutex); // 解锁
    return is_full;
}
