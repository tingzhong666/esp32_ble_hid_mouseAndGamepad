/**
 * BSP_key.h
 * @tingzhong666
 * 2024-05-09 19:26:55
 */
#pragma once

#ifndef BSP_key_H
#define BSP_key_H

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include <driver/gpio.h>

typedef enum {
    L_SW_PIN = 5,
    R_SW_PIN = 6,
    S1_PIN = 7,
    S2_PIN = 8,
    S3_PIN = 9,
    S4_PIN = 10,
    S5_PIN = 11,
    S6_PIN = 12,
    S7_PIN = 13
} key_num;
#define key_PIN (1ULL << L_SW_PIN) | (1ULL << R_SW_PIN) | (1ULL << S1_PIN) | (1ULL << S2_PIN) | (1ULL << S3_PIN) | (1ULL << S4_PIN) | (1ULL << S5_PIN) | (1ULL << S6_PIN) | (1ULL << S7_PIN)

#define ADC1_BITWIDTH ADC_BITWIDTH_DEFAULT
#define ADC1_ATTEN ADC_ATTEN_DB_12

// 2个摇杆的XY adc通道
typedef enum {
    LX_CHANNEL = 1,
    LY_CHANNEL = 0,
    RX_CHANNEL = 3,
    RY_CHANNEL = 2,
} rocker_channel;

typedef struct {
    rocker_channel channel; // adc通道
    adc_cali_handle_t cali_handle; // 曲线校准句柄
    adc_cali_curve_fitting_config_t cali_curve_fitting_config; // 曲线校准配置
} rocker;
// lx_rocker
// ly_rocker
// rx_rocker
// ry_rocker

void BSP_key_init(void);
int BSP_key_get(key_num key);
int BSP_rocker_get(rocker_channel channel);

#ifdef __cplusplus
}
#endif

#endif
