#pragma once

#include <pthread.h>
#include <driver/gpio.h>
#include <driver/periph_ctrl.h>
#include <driver/timer.h>
#include <esp32/rom/ets_sys.h>
#include <xtensa/hal.h>
#include <esp_attr.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_types.h>

#define CPU_FREQ (CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ * 1000 * 1000U)
#define TICK_PER_US (CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ)
#define NS_PER_SEC (1000 * 1000 * 1000ULL)
#define GET_TIMESTAMP_MS() esp_log_timestamp()
#define GPIO_SET_LEVEL(pin_number, level) \
    gpio_set_level(pin_number, level); \
    spin_wait_us(1)
#define GPIO_HIGH 1
#define GPIO_LOW 0

#ifndef INFO
#    define INFO(format, ...) ESP_LOGI(LOG_CONTEXT, format, ##__VA_ARGS__)
#endif

#ifndef DEBUG
#    define DEBUG(format, ...) ESP_LOGD(LOG_CONTEXT, format, ##__VA_ARGS__)
#endif

#ifndef WARN
#    define WARN(format, ...) ESP_LOGW(LOG_CONTEXT, format, ##__VA_ARGS__)
#endif

#ifndef ERROR
#    define ERROR(format, ...) ESP_LOGE(LOG_CONTEXT, format, ##__VA_ARGS__)
#endif

#ifdef __cplusplus
extern "C" {
#endif

inline IRAM_ATTR void wait_for_cycles(volatile uint32_t cycle_to_wait) {
    while (cycle_to_wait--) {}
}

inline IRAM_ATTR void spin_wait_us(volatile uint32_t us_to_wait) {
    wait_for_cycles(TICK_PER_US * us_to_wait);
}

#ifdef __cplusplus
}
#endif
