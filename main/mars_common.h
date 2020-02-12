#pragma once

#include <driver/gpio.h>
#include <driver/periph_ctrl.h>
#include <driver/timer.h>
#include <driver/rmt.h>
#include <esp32/rom/ets_sys.h>
#include <esp_attr.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_types.h>
#include <esp_pthread.h>
#include <esp_timer.h>
#include <esp_wifi.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <xtensa/hal.h>

#define USE_TS4321  false
#define CPU_FREQ    (CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ * 1000 * 1000U)
#define TICK_PER_US (CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ)
#define NS_PER_SEC  (1000 * 1000 * 1000ULL)
#define GPIO_HIGH   1
#define GPIO_LOW    0
#define GPIO_SET_LEVEL(pin_number, level) \
    gpio_set_level(pin_number, level); \
    usleep(1)

#ifndef INFO
    #define INFO(format, ...) ESP_LOGI(LOG_CONTEXT, format, ##__VA_ARGS__)
#endif

#ifndef DEBUG
    #define DEBUG(format, ...) ESP_LOGD(LOG_CONTEXT, format, ##__VA_ARGS__)
#endif

#ifndef WARN
    #define WARN(format, ...) ESP_LOGW(LOG_CONTEXT, format, ##__VA_ARGS__)
#endif

#ifndef ERROR
    #define ERROR(format, ...) ESP_LOGE(LOG_CONTEXT, format, ##__VA_ARGS__)
#endif

#ifdef __cplusplus
extern "C" {
#endif

inline void IRAM_ATTR wait_for_cycles(volatile uint32_t cycle_to_wait) {
    while (cycle_to_wait--) {
        asm("nop");
    }
}

#ifdef __cplusplus
}
#endif
