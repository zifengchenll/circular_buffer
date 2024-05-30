/**
 * @file config.h
 * @brief 平台和功能设置的配置头文件
 *
 * 本文件包含选择平台、启用或禁用调试以及切换锁使用的宏。
 * 根据具体项目需求调整这些设置。
 */

#ifndef CONFIG_H
#define CONFIG_H

/**
 * @def PLATFORM_LINUX
 * @brief 选择Linux平台
 *
 * 定义此宏以选择Linux平台。
 * 将此宏的值设置为1以启用Linux平台相关的功能和配置。
 */
#define PLATFORM_LINUX 1

/**
 * @def PLATFORM_FREERTOS
 * @brief 选择FreeRTOS平台
 *
 * 定义此宏以选择FreeRTOS平台。
 * 将此宏的值设置为1以启用FreeRTOS平台相关的功能和配置。
 * 取消注释此宏以启用FreeRTOS平台。
 */
// #define PLATFORM_FREERTOS 1

/**
 * @def PLATFORM_BARE_METAL
 * @brief 选择裸机平台
 *
 * 定义此宏以选择裸机平台。
 * 将此宏的值设置为1以启用裸机平台相关的功能和配置。
 * 取消注释此宏以启用裸机平台。
 */
// #define PLATFORM_BARE_METAL 1

/**
 * @def ENABLE_DEBUG
 * @brief 调试开关
 *
 * 定义此宏以启用或禁用调试功能。
 * 设置为0以禁用调试，设置为1以启用调试。
 * 当启用调试功能时，系统会输出调试信息，有助于开发和排错。
 */
#define ENABLE_DEBUG 0

/**
 * @def ENABLE_LOCK
 * @brief 锁开关
 *
 * 定义此宏以启用或禁用锁功能。
 * 设置为0以禁用锁，设置为1以启用锁。
 * 锁功能用于保护共享资源，避免多线程或多任务环境中的数据竞争问题。
 */
#define ENABLE_LOCK 1

#endif // CONFIG_H
