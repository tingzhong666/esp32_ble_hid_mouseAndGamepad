/**
 * BSP_key.c
 * @tingzhong666
 * 2024-05-09 19:26:39
 */
#include <BSP_key.h>
adc_oneshot_unit_handle_t adc1_handle;
rocker lx_rocker = {
    .channel = LX_CHANNEL,
    .cali_curve_fitting_config.atten = ADC1_ATTEN,
    .cali_curve_fitting_config.unit_id = ADC_UNIT_1,
    .cali_curve_fitting_config.bitwidth = ADC1_BITWIDTH,
    .cali_curve_fitting_config.chan = LX_CHANNEL,
};
rocker ly_rocker = {
    .channel = LY_CHANNEL,
    .cali_curve_fitting_config.atten = ADC1_ATTEN,
    .cali_curve_fitting_config.unit_id = ADC_UNIT_1,
    .cali_curve_fitting_config.bitwidth = ADC1_BITWIDTH,
    .cali_curve_fitting_config.chan = LY_CHANNEL,
};
rocker rx_rocker = {
    .channel = RX_CHANNEL,
    .cali_curve_fitting_config.atten = ADC1_ATTEN,
    .cali_curve_fitting_config.unit_id = ADC_UNIT_1,
    .cali_curve_fitting_config.bitwidth = ADC1_BITWIDTH,
    .cali_curve_fitting_config.chan = RX_CHANNEL,
};
rocker ry_rocker = {
    .channel = RY_CHANNEL,
    .cali_curve_fitting_config.atten = ADC1_ATTEN,
    .cali_curve_fitting_config.unit_id = ADC_UNIT_1,
    .cali_curve_fitting_config.bitwidth = ADC1_BITWIDTH,
    .cali_curve_fitting_config.chan = RY_CHANNEL,
};

rocker* rockers[] = {
    [LX_CHANNEL] = &lx_rocker,
    [LY_CHANNEL] = &ly_rocker,
    [RX_CHANNEL] = &rx_rocker,
    [RY_CHANNEL] = &ry_rocker
};

void BSP_key_init()
{
    gpio_config_t cfg = {
        .pin_bit_mask = key_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&cfg);
    //-------------ADC1 Init---------------//
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC1_BITWIDTH, // 12bit
        .atten = ADC1_ATTEN, // ADC衰减 3.1V
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, lx_rocker.channel, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ly_rocker.channel, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, rx_rocker.channel, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ry_rocker.channel, &config));

    //-------------ADC1 Calibration Init---------------//
    adc_cali_create_scheme_curve_fitting(&lx_rocker.cali_curve_fitting_config, &lx_rocker.cali_handle);
    adc_cali_create_scheme_curve_fitting(&ly_rocker.cali_curve_fitting_config, &ly_rocker.cali_handle);
    adc_cali_create_scheme_curve_fitting(&rx_rocker.cali_curve_fitting_config, &rx_rocker.cali_handle);
    adc_cali_create_scheme_curve_fitting(&ry_rocker.cali_curve_fitting_config, &ry_rocker.cali_handle);
}

// 电平0按下 1断开
// 返回1按下 0未按下
int BSP_key_get(key_num key)
{
    if (!gpio_get_level(key)) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        if (!gpio_get_level(key))
            return 1;
    }
    return 0;
}

// 摇杆XY读取
int BSP_rocker_get(rocker_channel channel)
{
    int in_adc = 0, in_voltage = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, rockers[channel]->channel, &in_adc));
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(rockers[channel]->cali_handle, in_adc, &in_voltage));
    return in_voltage;
}