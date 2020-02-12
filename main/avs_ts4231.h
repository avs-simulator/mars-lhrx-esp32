#pragma once

#include "avs_common.h"

#define LOG_CONTEXT "LHRX-TS4231"

#define TS_CONFIG_WORD 0x392B
#define TS_BUS_TRANS_DELAY() spin_wait_us(1)
#define TS_BUS_CHECK_DELAY() spin_wait_us(500)
#define TS_SLEEP_RECOVERY() spin_wait_us(100)
#define TS_D_AS_INPUT() gpio_set_direction(pin_d, GPIO_MODE_INPUT)
#define TS_D_AS_OUTPUT() gpio_set_direction(pin_d, GPIO_MODE_OUTPUT)
#define TS_E_AS_INPUT() gpio_set_direction(pin_e, GPIO_MODE_INPUT)
#define TS_E_AS_OUTPUT() gpio_set_direction(pin_e, GPIO_MODE_OUTPUT)
#define TS_D_IS_HIGH gpio_get_level(pin_d)
#define TS_E_IS_HIGH gpio_get_level(pin_e)
#define TS_D_SET_HIGH() GPIO_SET_LEVEL(pin_d, GPIO_HIGH)
#define TS_D_SET_LOW() GPIO_SET_LEVEL(pin_d, GPIO_LOW)
#define TS_E_SET_HIGH() GPIO_SET_LEVEL(pin_e, GPIO_HIGH)
#define TS_E_SET_LOW() GPIO_SET_LEVEL(pin_e, GPIO_LOW)
#define TS_GET_PIN_PATTERN() ((TS_D_IS_HIGH << 1) & TS_E_IS_HIGH)
#define TS_HAS_ENOUGH_STATE_COUNT >= 2
#define TS_PIN_PARAMS gpio_num_t pin_d, \
                      gpio_num_t pin_e
#define TS_PIN_ARGS pin_d, pin_e

typedef enum {
    TS_STATE_S0 = 0x00,
    TS_STATE_SLEEP = 0x01,
    TS_STATE_WATCH = 0x02,
    TS_STATE_S3 = 0x03,
    TS_STATE_UNKNOWN = 0x04
} ts4231_state_enum;

typedef enum {
    TS_D_LO_E_LO = 0b00,
    TS_D_LO_E_HI = 0b01,
    TS_D_HI_E_LO = 0b10,
    TS_D_HI_E_HI = 0b11,
} ts4231_pin_pattern_enum;

typedef enum {
    TS_START_BUS_FAIL = 0x01,
    TS_START_VERIFY_FAIL = 0x02,
    TS_START_WATCH_FAIL = 0x03,
    TS_START_SUCCESS = 0x04,
} ts4231_start_result_t;

#pragma pack(push, 1)
typedef struct {
    gpio_num_t pin_d;
    gpio_num_t pin_e;
    gpio_config_t ts_gpio_config;
} ts4231_t;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

static inline ts4231_state_enum ts_get_state(gpio_num_t pin_d, gpio_num_t pin_e) {
    uint8_t s0_count = 0;
    uint8_t sleep_count = 0;
    uint8_t watch_count = 0;
    uint8_t s3_count = 0;
    for (uint8_t i = 0; i < 3; ++i) {
        switch (TS_GET_PIN_PATTERN()) {
            case TS_D_HI_E_HI:
                s3_count++;
                break;
            case TS_D_HI_E_LO:
                sleep_count++;
                break;
            case TS_D_LO_E_HI:
                watch_count++;
                break;
            case TS_D_LO_E_LO:
                s0_count++;
                break;
            default:
                break;
        }
        TS_BUS_CHECK_DELAY();
    }
    if (s0_count TS_HAS_ENOUGH_STATE_COUNT) {
        return TS_STATE_S0;
    }
    if (sleep_count TS_HAS_ENOUGH_STATE_COUNT) {
        return TS_STATE_SLEEP;
    }
    if (sleep_count TS_HAS_ENOUGH_STATE_COUNT) {
        return TS_STATE_WATCH;
    }
    if (s3_count TS_HAS_ENOUGH_STATE_COUNT) {
        return TS_STATE_S3;
    }
    return TS_STATE_UNKNOWN;
}

static inline uint16_t ts_read_config(TS_PIN_PARAMS) {
    uint16_t config_value = 0x0000;
    TS_E_SET_HIGH();
    TS_D_SET_HIGH();
    TS_E_AS_OUTPUT();
    TS_D_AS_OUTPUT();
    TS_BUS_TRANS_DELAY();
    TS_D_SET_LOW();
    TS_BUS_TRANS_DELAY();
    TS_E_SET_LOW();
    TS_BUS_TRANS_DELAY();
    TS_D_SET_HIGH();
    TS_BUS_TRANS_DELAY();
    TS_E_SET_HIGH();
    TS_BUS_TRANS_DELAY();
    TS_D_AS_INPUT();
    TS_BUS_TRANS_DELAY();
    TS_E_SET_LOW();
    TS_BUS_TRANS_DELAY();
    for (uint8_t i = 0; i < 14; ++i) {
        TS_E_SET_HIGH();
        TS_BUS_TRANS_DELAY();
        config_value = (config_value << 1) | (TS_D_IS_HIGH & 0x0001);
        TS_E_SET_LOW();
        TS_BUS_TRANS_DELAY();
    }
    TS_D_SET_LOW();
    TS_D_AS_OUTPUT();
    TS_BUS_TRANS_DELAY();
    TS_E_SET_HIGH();
    TS_BUS_TRANS_DELAY();
    TS_D_SET_HIGH();
    TS_BUS_TRANS_DELAY();
    TS_E_AS_INPUT();
    TS_D_AS_INPUT();
    return config_value;
}

static inline bool ts_write_config(TS_PIN_PARAMS, uint16_t config_value) {
    TS_E_SET_HIGH();
    TS_D_SET_HIGH();
    TS_E_AS_OUTPUT();
    TS_D_AS_OUTPUT();
    TS_BUS_TRANS_DELAY();
    TS_D_SET_LOW();
    TS_BUS_TRANS_DELAY();
    TS_E_SET_LOW();
    TS_BUS_TRANS_DELAY();
    for (uint8_t i = 0; i < 15; ++i) {
        config_value <<= 1;
        if ((config_value & 0x8000) > 0) {
            TS_D_SET_HIGH();
        } else {
            TS_D_SET_LOW();
        }
        TS_BUS_TRANS_DELAY();
        TS_E_SET_HIGH();
        TS_BUS_TRANS_DELAY();
        TS_E_SET_LOW();
        TS_BUS_TRANS_DELAY();
    }
    TS_D_SET_LOW();
    TS_BUS_TRANS_DELAY();
    TS_E_SET_HIGH();
    TS_BUS_TRANS_DELAY();
    TS_D_SET_HIGH();
    TS_BUS_TRANS_DELAY();
    TS_E_AS_INPUT();
    TS_D_AS_INPUT();
    return ts_read_config(TS_PIN_ARGS) == config_value;
}

bool ts_init(ts4231_t *__restrict__ instance, gpio_num_t pin_e, gpio_num_t pin_d) {
    instance->pin_d = pin_d;
    instance->pin_e = pin_e;
    gpio_config_t *ts_gpio_config = &instance->ts_gpio_config;
    ts_gpio_config->pin_bit_mask = BIT64(pin_e) | BIT64(pin_d);
    ts_gpio_config->mode = GPIO_MODE_INPUT;
    ts_gpio_config->pull_up_en = GPIO_PULLUP_DISABLE;
    ts_gpio_config->pull_down_en = GPIO_PULLDOWN_DISABLE;
    ts_gpio_config->intr_type = GPIO_INTR_DISABLE;
    return gpio_config(ts_gpio_config) == ESP_OK;
}

uint8_t ts_wait_for_light(ts4231_t *__restrict__ instance, uint32_t timeout_ms) {
    uint32_t timeout_timestamp = GET_TIMESTAMP_MS() + timeout_ms;
    gpio_num_t pin_d = instance->pin_d;
    gpio_num_t pin_e = instance->pin_e;
    if (ts_get_state(pin_d, pin_e) != TS_STATE_S0) {
        return true;
    }
    while (!TS_D_IS_HIGH) {
        if (GET_TIMESTAMP_MS() >= timeout_timestamp) {
            return false;
        }
    }
    while (TS_D_IS_HIGH) {
        if (GET_TIMESTAMP_MS() >= timeout_timestamp) {
            return false;
        }
    }
    return true;
}

bool ts_watch_mode(TS_PIN_PARAMS) {
    switch (ts_get_state(pin_d, pin_e)) {
        case TS_STATE_S0:
            return false;
        case TS_STATE_SLEEP:
            TS_D_SET_HIGH();
            TS_D_AS_OUTPUT();
            TS_E_SET_LOW();
            TS_E_AS_OUTPUT();
            TS_D_SET_LOW();
            TS_D_AS_INPUT();
            TS_E_SET_HIGH();
            TS_E_AS_INPUT();
            TS_SLEEP_RECOVERY();
            return ts_get_state(pin_d, pin_e) == TS_STATE_WATCH;
        case TS_STATE_WATCH:
            return true;
        case TS_STATE_S3:
            TS_E_SET_HIGH();
            TS_E_AS_OUTPUT();
            TS_D_SET_HIGH();
            TS_D_AS_OUTPUT();
            TS_E_SET_LOW();
            TS_D_SET_LOW();
            TS_D_AS_INPUT();
            TS_E_SET_HIGH();
            TS_E_AS_INPUT();
            TS_SLEEP_RECOVERY();
            return ts_get_state(pin_d, pin_e) == TS_STATE_WATCH;
        default:
            return false;
    }
}

ts4231_start_result_t ts_configure(ts4231_t *__restrict__ instance) {
    gpio_num_t pin_d = instance->pin_d;
    gpio_num_t pin_e = instance->pin_e;
    TS_D_AS_INPUT();
    TS_E_AS_INPUT();
    TS_D_SET_LOW();
    TS_E_SET_LOW();
    TS_E_AS_OUTPUT();
    TS_BUS_TRANS_DELAY();
    TS_E_SET_HIGH();
    TS_BUS_TRANS_DELAY();
    TS_E_SET_LOW();
    TS_BUS_TRANS_DELAY();
    TS_E_SET_HIGH();
    TS_BUS_TRANS_DELAY();
    TS_D_AS_OUTPUT();
    TS_BUS_TRANS_DELAY();
    TS_D_SET_HIGH();
    TS_BUS_TRANS_DELAY();
    TS_E_AS_INPUT();
    TS_D_AS_INPUT();
    if (ts_get_state(pin_d, pin_e) != TS_STATE_S3) {
        return TS_START_BUS_FAIL;
    }
    if (!ts_write_config(TS_PIN_ARGS, TS_CONFIG_WORD)) {
        return TS_START_VERIFY_FAIL;
    }
    if (!ts_watch_mode(TS_PIN_ARGS)) {
        return TS_START_WATCH_FAIL;
    }
    return TS_START_SUCCESS;
}

#ifdef __cplusplus
}
#endif

#undef TS_CONFIG_WORD
#undef TS_BUS_TRANS_DELAY
#undef TS_BUS_CHECK_DELAY
#undef TS_SLEEP_RECOVERY
#undef TS_D_AS_INPUT
#undef TS_D_AS_OUTPUT
#undef TS_E_AS_INPUT
#undef TS_E_AS_OUTPUT
#undef TS_D_IS_HIGH
#undef TS_E_IS_HIGH
#undef TS_D_SET_HIGH
#undef TS_D_SET_LOW
#undef TS_E_SET_HIGH
#undef TS_E_SET_LOW
#undef TS_GET_PIN_PATTERN
#undef TS_HAS_ENOUGH_STATE_COUNT
#undef TS_PIN_PARAMS
#undef TS_PIN_ARGS

#undef LOG_CONTEXT
