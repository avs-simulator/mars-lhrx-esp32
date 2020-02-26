#pragma once

#include "mars_common.h"

#define LOG_CONTEXT "LHRX_TR"

#define TR_PROBE_ENABLED false
#define TR_RES_NS        (125LL)
#define TR_GROUP_ID      TIMER_GROUP_1
#define TR_ID            TIMER_1
#define TR_OBJECT        TIMERG1.hw_timer[TR_ID]
#define TR_TUPLE         TR_GROUP_ID, TR_ID
#define TR_RESET_COUNT   (UINT32_MAX)
#define TR_LATENCY_US    (0.5629401f)

const int32_t tr_div       = TIMER_BASE_CLK * TR_RES_NS / NS_PER_SEC;
const int32_t tr_clock_mhz = TIMER_BASE_CLK / tr_div / 1000000;

#if TR_PROBE_ENABLED
static const uint64_t PROBE_COUNT = 153479917248;
#endif

volatile bool   tr_init_finished = false;
static uint32_t tr_us_rshift     = 0;

#ifdef __cplusplus
extern "C" {
#endif

IRAM_ATTR int64_t tr_get_tickstamp() {
    if (TR_OBJECT.cnt_low == UINT32_MAX) {
        TR_OBJECT.update = true;
        wait_for_cycles(CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ / tr_clock_mhz);
    } else {
        TR_OBJECT.update = true;
    }
    return (((int64_t)TR_OBJECT.cnt_high << 32) | TR_OBJECT.cnt_low);
}

IRAM_ATTR int64_t tr_get_timestamp_ns() {
    return tr_get_tickstamp() * TR_RES_NS;
}

IRAM_ATTR int64_t tr_get_timestamp_us() {
    return tr_get_tickstamp() >> tr_us_rshift;
}

IRAM_ATTR int64_t tr_get_timestamp_ms() {
    return tr_get_timestamp_us() / 1000;
}

static void *tr_initializer(void *params) {
    timer_config_t tr_config;
    tr_config.divider     = tr_div;
    tr_config.counter_dir = TIMER_COUNT_UP;
    tr_config.counter_en  = TIMER_START;
    tr_config.alarm_en    = TIMER_ALARM_EN;
    tr_config.intr_type   = TIMER_INTR_LEVEL;
    tr_config.auto_reload = TIMER_AUTORELOAD_DIS;
    timer_init(TR_TUPLE, &tr_config);
    timer_set_counter_value(TR_TUPLE, 0ULL);
    timer_start(TR_TUPLE);
    TR_OBJECT.cnt_high = 0;
    TR_OBJECT.cnt_low  = 0;
    TR_OBJECT.update   = true;
#if TR_PROBE_ENABLED
    int64_t  timestamp_last        = 0;
    int64_t  timestamp_current     = 0;
    int64_t  timestamp_delta       = 0;
    int64_t  timestamp_worst_delta = 0;
    int64_t  timestamp_best_delta  = INT64_MAX;
    uint64_t loop_quota            = PROBE_COUNT;
    INFO("Running %llu loops (est. %f seconds)to find faulty memory barrier...",
         loop_quota, loop_quota * TR_LATENCY_US / 1000000);
    for (uint64_t i = 0; i < 10000; --i) {
        timestamp_last = tr_get_timestamp_us();
    }
    portDISABLE_INTERRUPTS();
    while (loop_quota--) {
        timestamp_current = tr_get_timestamp_us();
        timestamp_delta   = timestamp_current - timestamp_last;
        if (timestamp_delta < 0) {
            break;
        }
        timestamp_worst_delta = timestamp_worst_delta < timestamp_delta
                                    ? timestamp_delta
                                    : timestamp_worst_delta;
        timestamp_best_delta = timestamp_best_delta > timestamp_delta
                                   ? timestamp_delta
                                   : timestamp_best_delta;
        timestamp_last = timestamp_current;
    }
    portENABLE_INTERRUPTS();
    if (timestamp_delta < 0) {
        ERROR("Faulty memory barrier found!");
        ERROR("Current Timestamp %lli us", timestamp_current);
        ERROR("Last Timestamp %lli us", timestamp_last);
    }
    INFO("Finished!");
    INFO("Last: %lli us | Last delta: %lli us | Worst Delta: %lli us | Best Delta: %lli us",
         timestamp_current, timestamp_delta, timestamp_worst_delta, timestamp_best_delta);
#endif
    tr_init_finished = true;
    return NULL;
}

void tr_start() {
    // Constant init
    tr_us_rshift = (uint32_t)__builtin_log2(1000.0 / ((double)TR_RES_NS));
    // Timer init in other thread
    pthread_t         t0;
    esp_pthread_cfg_t config = esp_pthread_get_default_config();
    config.thread_name       = "tr_init";
    config.prio              = 255;
    esp_pthread_set_cfg(&config);
    pthread_create(&t0, NULL, tr_initializer, NULL);
}

#ifdef __cplusplus
}
#endif

#undef LOG_CONTEXT
