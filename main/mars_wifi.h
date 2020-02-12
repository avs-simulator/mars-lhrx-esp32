#pragma once

#include "mars_common.h"

#define LOG_CONTEXT   "LHRX_WIFI"
#define WIFI_AP_SSID  "D0:37:45:75:5F:98"
#define WIFI_AP_PASSW "@Password=MARS-LHRX-2020"

static volatile bool wifi_ready_to_use    = false;
static uint32_t      wifi_address         = 0;
static uint32_t      wifi_gateway_address = 0;

#ifdef __cplusplus
extern "C" {
#endif

static IRAM_ATTR void wifi_event_handler(void *params, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base != WIFI_EVENT) {
        return;
    }
    switch (event_id) {
        case WIFI_EVENT_STA_START:
            DEBUG("Connecting to %s", WIFI_AP_SSID);
            esp_wifi_connect();
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            wifi_ready_to_use = false;
            WARN("Lost connection to %s", WIFI_AP_SSID);
            esp_wifi_connect();
            break;
        default:
            break;
    }
}

static IRAM_ATTR void wifi_ip_event_handler(void *params, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base != IP_EVENT) {
        return;
    }
    if (event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        wifi_address             = event->ip_info.ip.addr;
        wifi_gateway_address     = event->ip_info.gw.addr;
        wifi_ready_to_use        = true;
        DEBUG("Client IPv4 address: %s", ip4addr_ntoa(&event->ip_info.ip));
        DEBUG("Server IPv4 address: %s", ip4addr_ntoa(&event->ip_info.gw));
    }
}

IRAM_ATTR bool wifi_ready() {
    return wifi_ready_to_use;
}

IRAM_ATTR uint32_t wifi_get_owned_ip() {
    return wifi_address;
}

IRAM_ATTR uint32_t wifi_get_target_ip() {
    return wifi_gateway_address;
}

void wifi_start() {
    tcpip_adapter_init();
    esp_event_loop_create_default();
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init_config);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_ip_event_handler, NULL);
    wifi_config_t wifi_config = {
        .sta = {
            .ssid        = WIFI_AP_SSID,
            .password    = WIFI_AP_PASSW,
            .scan_method = WIFI_FAST_SCAN,
            .sort_method = WIFI_CONNECT_AP_BY_SECURITY,
            .threshold   = {
                .rssi     = -90,
                .authmode = WIFI_AUTH_WPA2_PSK,
            }},
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
}

#ifdef __cplusplus
}
#endif

#undef LOG_CONTEXT
