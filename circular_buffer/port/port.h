// port.h
#ifndef PORT_H
#define PORT_H

#include "config.h"
#include <stdbool.h>

#if ENABLE_LOCK
    #if defined(PLATFORM_LINUX)
    #include <pthread.h>
    typedef pthread_mutex_t mutex_t;
    #elif defined(PLATFORM_FREERTOS)
    #include "FreeRTOS.h"
    #include "semphr.h"
    typedef SemaphoreHandle_t mutex_t;
    #elif defined(PLATFORM_BARE_METAL)
    // 在裸机平台上，定义一个空的互斥锁结构
    typedef struct {} mutex_t;
    #endif

    /**
     * @brief 初始化互斥锁
     * 
     * @param mutex 互斥锁指针
     * @return 成功返回true，失败返回false
     */
    bool mutex_init(mutex_t *mutex);

    /**
     * @brief 销毁互斥锁
     * 
     * @param mutex 互斥锁指针
     */
    void mutex_destroy(mutex_t *mutex);

    /**
     * @brief 加锁
     * 
     * @param mutex 互斥锁指针
     */
    void mutex_lock(mutex_t *mutex);

    /**
     * @brief 解锁
     * 
     * @param mutex 互斥锁指针
     */
    void mutex_unlock(mutex_t *mutex);
#else
    // 如果不启用锁，定义空的锁操作
    typedef struct {} mutex_t;

    #define mutex_init(mutex)   (true)
    #define mutex_destroy(mutex) ((void)0)
    #define mutex_lock(mutex)    ((void)0)
    #define mutex_unlock(mutex)  ((void)0)
#endif

#if ENABLE_DEBUG
#include <stdio.h>
#define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) 
#endif

#endif // PORT_H
