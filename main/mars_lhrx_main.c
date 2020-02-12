#include "mars_data_updater.h"
#include "mars_lhrx_processor.h"
#include "mars_tick_rotator.h"
#include "mars_wifi.h"

#define LOG_CONTEXT "LHRX_MAIN"

void IRAM_ATTR app_main(void) {
    tr_start();
    wifi_start();
    du_start();
    lhrx_start();
}

#undef LOG_CONTEXT
