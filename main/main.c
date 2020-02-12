#include "avs_common.h"

#define LOG_CONTEXT "LHRX-MAIN"

static int64_t ccount_last = 0;
static int64_t ccount_current = 0;
static int64_t ccount_delta = 0;

void app_main(void) {
    INFO("CPU Frequency is %u", CPU_FREQ);
    while (true) {
        ccount_current = xthal_get_ccount();
        ccount_delta = ccount_current - ccount_last;
        ccount_last = ccount_current;
        if (ccount_delta <= 0) {
            INFO("CPU Counter overflow!");
        }
        if (GET_TIMESTAMP_MS() % 1000 == 0) {
            INFO("Tick %u", xthal_get_ccount());
        }
    }
}

#undef LOG_CONTEXT
