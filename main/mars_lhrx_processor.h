#pragma once

#include "mars_common.h"
#include "mars_tick_rotator.h"
#include "mars_ts4231.h"

#define LOG_CONTEXT                     "LHRX_PROC"
#define LHRX_ENABLED_CHIPS              1
#define LHRX_ENABLED                    LHRX_ENABLED_CHIPS > 0
#define LHRX_0_ENABLED                  LHRX_ENABLED_CHIPS > 0
#define LHRX_1_ENABLED                  LHRX_ENABLED_CHIPS > 1
#define LHRX_2_ENABLED                  LHRX_ENABLED_CHIPS > 2
#define LHRX_3_ENABLED                  LHRX_ENABLED_CHIPS > 3
#define LHRX_LIGHT_DETECTION_TIMEOUT_MS 5000

#if LHRX_0_ENABLED
    #define LHRX_0_E GPIO_NUM_25
    #if USE_TS4231
        #define LHRX_0_D GPIO_NUM_26
    #endif
#endif

#if LHRX_1_ENABLED
    #define LHRX_1_E GPIO_NUM_16
    #if USE_TS4231
        #define LHRX_1_D GPIO_NUM_17
    #endif
#endif

#if LHRX_2_ENABLED
    #define LHRX_2_E GPIO_NUM_32
    #if USE_TS4231
        #define LHRX_2_D GPIO_NUM_33
    #endif
#endif

#if LHRX_3_ENABLED
    #define LHRX_3_E GPIO_NUM_13
    #if USE_TS4231
        #define LHRX_3_D GPIO_NUM_27
    #endif
#endif

#define LHTX_CLOCK_MHZ      (48)
#define LHTX_CYCLE_FULL     (400000)
#define LHTX_CYCLE_OFFSET   (16000)
#define LHTX_BLIP_WINDOW    (192)
#define LHTX_SWIPE_LOWER    (2)
#define LHTX_SWIPE_UPPER    (96)
#define LHTX_BLIP_A_X_MIDPT (3000)
#define LHTX_BLIP_A_Y_MIDPT (3500)
#define LHTX_BLIP_B_X_MIDPT (5000)
#define LHTX_BLIP_B_Y_MIDPT (5500)
#define LHRX_SWIPE_RANGE    case lhrx_swipe_lower ... lhrx_swipe_upper
#define LHRX_BLIP_A_X_RANGE case lhrx_blip_a_x_lower ... lhrx_blip_a_x_upper
#define LHRX_BLIP_A_Y_RANGE case lhrx_blip_a_y_lower ... lhrx_blip_a_y_upper
#define LHRX_BLIP_B_X_RANGE case lhrx_blip_b_x_lower ... lhrx_blip_b_x_upper
#define LHRX_BLIP_B_Y_RANGE case lhrx_blip_b_y_lower ... lhrx_blip_b_y_upper

// Lighthouse Transmitter constants
static const int32_t lhtx_blip_a_x_lower = LHTX_BLIP_A_X_MIDPT - LHTX_BLIP_WINDOW;
static const int32_t lhtx_blip_a_x_upper = LHTX_BLIP_A_X_MIDPT + LHTX_BLIP_WINDOW;
static const int32_t lhtx_blip_a_y_lower = LHTX_BLIP_A_Y_MIDPT - LHTX_BLIP_WINDOW;
static const int32_t lhtx_blip_a_y_upper = LHTX_BLIP_A_Y_MIDPT + LHTX_BLIP_WINDOW;
static const int32_t lhtx_blip_b_x_lower = LHTX_BLIP_B_X_MIDPT - LHTX_BLIP_WINDOW;
static const int32_t lhtx_blip_b_x_upper = LHTX_BLIP_B_X_MIDPT + LHTX_BLIP_WINDOW;
static const int32_t lhtx_blip_b_y_lower = LHTX_BLIP_B_Y_MIDPT - LHTX_BLIP_WINDOW;
static const int32_t lhtx_blip_b_y_upper = LHTX_BLIP_B_Y_MIDPT + LHTX_BLIP_WINDOW;

// Lighthouse Receiver constants
static const int32_t lhrx_tick_div       = LHTX_CLOCK_MHZ / tr_clock_mhz;
static const int32_t lhrx_blip_window    = LHTX_BLIP_WINDOW / lhrx_tick_div;
static const int32_t lhrx_swipe_lower    = LHTX_SWIPE_LOWER / lhrx_tick_div;
static const int32_t lhrx_swipe_upper    = LHTX_SWIPE_UPPER / lhrx_tick_div;
static const int32_t lhrx_blip_a_x_lower = lhtx_blip_a_x_lower / lhrx_tick_div;
static const int32_t lhrx_blip_a_x_upper = lhtx_blip_a_x_upper / lhrx_tick_div;
static const int32_t lhrx_blip_a_y_lower = lhtx_blip_a_y_lower / lhrx_tick_div;
static const int32_t lhrx_blip_a_y_upper = lhtx_blip_a_y_upper / lhrx_tick_div;
static const int32_t lhrx_blip_b_x_lower = lhtx_blip_b_x_lower / lhrx_tick_div;
static const int32_t lhrx_blip_b_x_upper = lhtx_blip_b_x_upper / lhrx_tick_div;
static const int32_t lhrx_blip_b_y_lower = lhtx_blip_b_y_lower / lhrx_tick_div;
static const int32_t lhrx_blip_b_y_upper = lhtx_blip_b_y_upper / lhrx_tick_div;
static const int32_t lhrx_cycle_full     = LHTX_CYCLE_FULL / lhrx_tick_div;
static const int32_t lhrx_cycle_offset   = LHTX_CYCLE_OFFSET / lhrx_tick_div;
static const int32_t lhrx_deg_timespan   = lhrx_cycle_full - lhrx_cycle_offset;
static const int32_t lhrx_deg_360        = 360 * 1000;
static const int32_t lhrx_deg_offset     = -180 * 1000;

typedef enum {
    LHRX_WAIT_BLIP_A = 0,
    LHRX_WAIT_BLIP_B = 1,
    LHRX_WAIT_SWIPE  = 2,
} lhrx_wait_state_enum;

typedef enum {
    LHRX_RESYNC           = 0,
    LHRX_UPDATE_A_X_ANGLE = 1,
    LHRX_UPDATE_A_Y_ANGLE = 2,
    LHRX_UPDATE_B_X_ANGLE = 3,
    LHRX_UPDATE_B_Y_ANGLE = 4,
} lhrx_pending_action_enum;

#pragma pack(push, 1)
typedef union {
    struct {
        bool    valid_a_x;
        bool    valid_a_y;
        bool    valid_b_x;
        bool    valid_b_y;
        int32_t a_x;
        int32_t a_y;
        int32_t b_x;
        int32_t b_y;
    } angles;
    uint8_t bytes[20];
} lhrx_data_t;
#pragma pack(pop)

static_assert(sizeof(lhrx_data_t) == 20, "Invalid size of lhrx_data_t!");

#pragma pack(push, 1)
typedef struct {
    size_t                   target_chip;
    lhrx_pending_action_enum pending_action;
    int64_t                  blip_a_risen_timestamp;
    int64_t                  swipe_risen_timestamp;
    int64_t                  swipe_width;
} lhrx_pending_data_t;
#pragma pack(pop)

static_assert(sizeof(lhrx_pending_data_t) == 32, "Invalid size of lhrx_pending_data_t!");

#pragma pack(push, 1)
typedef struct {
    size_t  target_chip;
    int64_t rise_timestamp;
    int64_t fallen_timestamp;
} lhrx_signal_queue_item_t;
#pragma pack(pop)

static_assert(sizeof(lhrx_signal_queue_item_t) == 20, "Invalid size of lhrx_signal_queue_item_t!");

#if LHRX_ENABLED
    #if USE_TS4231
static ts4231_t   lhrx_chips[LHRX_ENABLED_CHIPS]   = {0};
static gpio_num_t lhrx_pins[LHRX_ENABLED_CHIPS][2] = {
        #if LHRX_0_ENABLED
    {LHRX_0_E, LHRX_0_D},
        #endif
        #if LHRX_1_ENABLED
    {LHRX_1_E, LHRX_1_D},
        #endif
        #if LHRX_2_ENABLED
    {LHRX_2_E, LHRX_2_D},
        #endif
        #if LHRX_3_ENABLED
    {LHRX_3_E, LHRX_3_D},
        #endif
};
    #else
static gpio_num_t lhrx_pins[LHRX_ENABLED_CHIPS] = {
        #if LHRX_0_ENABLED
    LHRX_0_E,
        #endif
        #if LHRX_1_ENABLED
    LHRX_1_E,
        #endif
        #if LHRX_2_ENABLED
    LHRX_2_E,
        #endif
        #if LHRX_3_ENABLED
    LHRX_3_E,
        #endif
};
static lhrx_data_t       lhrx_data[LHRX_ENABLED_CHIPS] = {0};
static SemaphoreHandle_t lhrx_data_mutex               = NULL;
static StaticSemaphore_t lhrx_mutex_buffer             = {0};
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if LHRX_ENABLED

static inline void lhrx_set_next_state(lhrx_wait_state_enum *__restrict__ wait_state) {
    switch (*wait_state) {
        case LHRX_WAIT_BLIP_A:
            *wait_state = LHRX_WAIT_BLIP_B;
            break;
        case LHRX_WAIT_BLIP_B:
            *wait_state = LHRX_WAIT_SWIPE;
            break;
        default:
            *wait_state = LHRX_WAIT_BLIP_A;
            break;
    }
}

static inline void lhrx_reset_state(lhrx_wait_state_enum *__restrict__ wait_state) {
    *wait_state = LHRX_WAIT_BLIP_A;
}

static inline void lhrx_process_data(lhrx_pending_data_t *__restrict__ pending_data) {
    lhrx_data_t *data_ref               = &lhrx_data[pending_data->target_chip];
    int32_t      blip_a_risen_timestamp = pending_data->blip_a_risen_timestamp;
    int32_t      swipe_timestamp        = pending_data->swipe_risen_timestamp + pending_data->swipe_width / 2;
    int64_t      swipe_tick_degree      = lhrx_cycle_offset + (int64_t)(swipe_timestamp - blip_a_risen_timestamp);
    int32_t      swipe_degree           = (int32_t)(lhrx_deg_offset + ((lhrx_deg_360 * swipe_tick_degree) / lhrx_deg_timespan));
    xSemaphoreTake(lhrx_data_mutex, portMAX_DELAY);
    switch (pending_data->pending_action) {
        case LHRX_UPDATE_A_X_ANGLE:
            data_ref->angles.a_x       = swipe_degree;
            data_ref->angles.valid_a_x = true;
            break;
        case LHRX_UPDATE_A_Y_ANGLE:
            data_ref->angles.a_y       = swipe_degree;
            data_ref->angles.valid_a_y = true;
            break;
        case LHRX_UPDATE_B_X_ANGLE:
            data_ref->angles.b_x       = swipe_degree;
            data_ref->angles.valid_b_x = true;
            break;
        case LHRX_UPDATE_B_Y_ANGLE:
            data_ref->angles.b_y       = swipe_degree;
            data_ref->angles.valid_b_y = true;
            break;
        default:
            break;
    }
    xSemaphoreGive(lhrx_data_mutex);
}

static void *lhrx_processing_loop(void *__restrict__ shared_state) {
    return NULL;
}

static void *lhrx_detection_loop(void *__restrict__ shared_state) {
    portDISABLE_INTERRUPTS();
    bool                     signal_level                         = false;
    bool                     lhrx_level[LHRX_ENABLED_CHIPS]       = {0};
    int64_t                  lhrx_rise_temp[LHRX_ENABLED_CHIPS]   = {0};
    lhrx_signal_queue_item_t lhrx_signal_data[LHRX_ENABLED_CHIPS] = {0};
    while (true) {
        for (size_t i = 0; i < LHRX_ENABLED_CHIPS; ++i) {
    #if USE_TS4231
            signal_level = !gpio_get_level(lhrx_pins[i][0]);
    #else
            signal_level = !gpio_get_level(lhrx_pins[i]);
    #endif
            if (lhrx_level[i] && !signal_level) {
                lhrx_level[i]                        = signal_level;
                lhrx_signal_data[i].target_chip      = i;
                lhrx_signal_data[i].rise_timestamp   = lhrx_rise_temp[i];
                lhrx_signal_data[i].fallen_timestamp = tr_get_timestamp_ticks();
            } else if (!lhrx_level[i] && signal_level) {
                lhrx_level[i]     = signal_level;
                lhrx_rise_temp[i] = tr_get_timestamp_ticks();
            }
        }
    }
    portENABLE_INTERRUPTS();
    return NULL;
}

static inline bool lhrx_init() {
    lhrx_data_mutex = xSemaphoreCreateMutexStatic(&lhrx_mutex_buffer);
    for (uint8_t i = 0; i < LHRX_ENABLED_CHIPS; ++i) {
    #if USE_TS4231
        if (!ts_init(&lhrx_chips[i], lhrx_pins[i][0], lhrx_pins[i][1])) {
            log_error("Faulty initialization of TS4231[%d]!", i);
            return false;
        }
        log_info("TS4231[%d] initialized", i);
    #else
        gpio_config_t lhrx_gpio_config_e;
        lhrx_gpio_config_e.pin_bit_mask = BIT64(lhrx_pins[i]);
        lhrx_gpio_config_e.mode         = GPIO_MODE_INPUT;
        lhrx_gpio_config_e.pull_up_en   = GPIO_PULLUP_DISABLE;
        lhrx_gpio_config_e.pull_down_en = GPIO_PULLDOWN_DISABLE;
        lhrx_gpio_config_e.intr_type    = GPIO_INTR_DISABLE;
        gpio_config(&lhrx_gpio_config_e);
    #endif
    }
    return true;
}

static inline bool lhrx_detect_light() {
    #if USE_TS4231
    for (uint8_t i = 0; i < LHRX_ENABLED_CHIPS; ++i) {
        if (!ts_wait_for_light(&lhrx_chips[i], LHRX_LIGHT_DETECTION_TIMEOUT_MS)) {
            log_error("TS4231[%d] cannot detect light!", i);
            return false;
        }
        log_info("TS4231[%d] light detected", i);
    }
    #endif
    return true;
}

static inline bool lhrx_configure() {
    #if USE_TS4231
    for (uint8_t i = 0; i < LHRX_ENABLED_CHIPS; ++i) {
        if (ts_configure(&lhrx_chips[i]) != TS_START_SUCCESS) {
            log_error("TS4231[%d] cannot be configured!", i);
            return false;
        }
        log_info("TS4231[%d] configured", i);
    }
    #endif
    return true;
}

static inline bool lhrx_monitor() {
    // Timestamp Processing Thread
    pthread_t         processing_thread        = 0;
    esp_pthread_cfg_t processing_thread_config = esp_pthread_get_default_config();
    processing_thread_config.pin_to_core       = PRO_CPU_NUM;
    processing_thread_config.prio              = CONFIG_PTHREAD_TASK_PRIO_DEFAULT - 1;
    esp_pthread_set_cfg(&processing_thread_config);
    pthread_create(&processing_thread, NULL, lhrx_processing_loop, NULL);
    // Edge Detection Thread
    pthread_t         detection_thread        = 1;
    esp_pthread_cfg_t detection_thread_config = esp_pthread_get_default_config();
    detection_thread_config.pin_to_core       = APP_CPU_NUM;
    detection_thread_config.prio              = CONFIG_PTHREAD_TASK_PRIO_DEFAULT;
    esp_pthread_set_cfg(&detection_thread_config);
    pthread_create(&detection_thread, NULL, lhrx_detection_loop, NULL);
    return true;
}
#else
static inline bool lhrx_init() {
    WARN("No TS4231 specified, skipping initialization...");
    return true;
}

static inline bool lhrx_detect_light() {
    WARN("No TS4231 specified, skipping light detection...");
    return true;
}

static inline bool lhrx_configure() {
    WARN("No TS4231 specified, skipping configuration...");
    return true;
}

static inline bool lhrx_set_isr() {
    WARN("No TS4231 specified, skipping interrupt setup...");
    return true;
}

static inline bool lhrx_monitor() {
    WARN("No TS4231 specified, skipping monitoring...");
    return true;
}
#endif

bool lhrx_start() {
    return lhrx_init() && lhrx_detect_light() && lhrx_configure() && lhrx_monitor();
}

#ifdef __cplusplus
}
#endif

#undef LOG_CONTEXT
