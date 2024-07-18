#include "BSP_key.h"
#include "HIDReport.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_check.h"
#include "esp_gap_ble_api.h"
#include "esp_gatt_common_api.h"
#include "esp_gatts_api.h"
#include "freertos/FreeRTOS.h"
#include <driver/gpio.h>
#include <nvs_flash.h>
#include <stdio.h>
#include <string.h>

#define APP_ID 0
#define DEVICE_NAME "赛博24"

typedef enum {
    i_service_hid = 0,

    i_char_hidInfo,
    i_val_hidInfo,

    i_char_hidCP,
    i_val_hidCP,

    i_char_hidReport,
    i_val_hidReport,

    i_char_protocolMode,
    i_val_protocolMode,

    i_char_mouseIn,
    i_val_mouseIn,
    i_des_mouseIn,
    i_ccc_mouseIn,

    i_char_gamepadIn,
    i_val_gamepadIn,
    i_des_gamepadIn,
    i_ccc_gamepadIn,
} att_tbl_index;

// 属性uuid
uint16_t uuid_type_service = 0x2800;
uint16_t uuid_type_character = 0x2803;

// 服务uuid
uint16_t hid_service_uuid = 0x1812;

// 特性uuid
uint16_t hidInfo_char_uuid = 0x2A4A;
uint16_t hidCP_char_uuid = 0x2A4C;
uint16_t HIDReportMap_char_uuid = 0x2A4B; // 报告地图
uint16_t protocolMode_char_uuid = 0x2A4e;
uint16_t HIDReport_char_uuid = 0x2A4D; // 报告数据值

// 描述uuid
uint16_t reportReference_des_uuid = 0x2908; // 报告ID
uint16_t character_client_config_uuid = 0x2902; // 客户端配置

// 属性权限
uint16_t character_perm = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE;

// 特性权限
static const uint8_t char_prop_read_notify = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY;
// static const uint8_t char_prop_read_write = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE;

// hid信息
static const uint8_t hidInfo[3] = {
    0x0111 & 0xff, 0x0111 >> 8, // bcdHID (USB HID version)表示v1.11
    0x00 // bCountryCode
};
// 协议模式 报告
uint8_t hidProtocolMode = 1;
// 鼠标报告ID及类型 input output feature 123
static uint8_t hidReportRefMouseIn[2] = { 1, 1 };
// 手柄报告ID及类型 input output feature 123
static uint8_t hidReportRefGamepadIn[2] = { 2, 1 };
// 特征报告ID及类型 input output feature 123
// static uint8_t hidReportRefFeature[2] = { 0, 3 };
// 属性表
static const esp_gatts_attr_db_t att_tbl[] = {
    // 服务 HID
    [i_service_hid] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
        .att_desc.uuid_length = 2,
        .att_desc.uuid_p = (uint8_t*)&uuid_type_service,
        .att_desc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .att_desc.value = (uint8_t*)&hid_service_uuid,
        .att_desc.length = sizeof(hid_service_uuid),
        .att_desc.max_length = sizeof(hid_service_uuid) },

    // HID信息
    [i_char_hidInfo] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
        .att_desc.uuid_length = 2,
        .att_desc.uuid_p = (uint8_t*)&uuid_type_character,
        .att_desc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .att_desc.value = (uint8_t*)&character_perm,
        .att_desc.length = sizeof(character_perm),
        .att_desc.max_length = sizeof(character_perm) },
    [i_val_hidInfo] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
        .att_desc.uuid_length = 2,
        .att_desc.uuid_p = (uint8_t*)&hidInfo_char_uuid,
        .att_desc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .att_desc.value = (uint8_t*)&hidInfo,
        .att_desc.length = sizeof(hidInfo),
        .att_desc.max_length = sizeof(hidInfo) },
    // HID控制点
    [i_char_hidCP] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
        .att_desc.uuid_length = 2,
        .att_desc.uuid_p = (uint8_t*)&uuid_type_character,
        .att_desc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .att_desc.value = (uint8_t*)&character_perm,
        .att_desc.length = sizeof(character_perm),
        .att_desc.max_length = sizeof(character_perm) },
    [i_val_hidCP] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
        .att_desc.uuid_length = 2,
        .att_desc.uuid_p = (uint8_t*)&hidCP_char_uuid,
        .att_desc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .att_desc.value = NULL,
        .att_desc.length = 0,
        .att_desc.max_length = 1 },
    // HID报告地图
    [i_char_hidReport] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
        .att_desc.uuid_length = 2,
        .att_desc.uuid_p = (uint8_t*)&uuid_type_character,
        .att_desc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .att_desc.value = (uint8_t*)&character_perm,
        .att_desc.length = 2,
        .att_desc.max_length = 2 },
    [i_val_hidReport] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
        .att_desc.uuid_length = 2,
        .att_desc.uuid_p = (uint8_t*)&HIDReportMap_char_uuid,
        .att_desc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .att_desc.value = (uint8_t*)&ReportDescriptor,
        .att_desc.length = sizeof(ReportDescriptor),
        .att_desc.max_length = 256 },
    // 协议模式
    [i_char_protocolMode] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
        .att_desc.uuid_length = 2,
        .att_desc.uuid_p = (uint8_t*)&uuid_type_character,
        .att_desc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .att_desc.value = (uint8_t*)&character_perm,
        .att_desc.length = 2,
        .att_desc.max_length = 2 },
    [i_val_protocolMode] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
        .att_desc.uuid_length = 2,
        .att_desc.uuid_p = (uint8_t*)&protocolMode_char_uuid,
        .att_desc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .att_desc.value = (uint8_t*)&hidProtocolMode,
        .att_desc.length = sizeof(hidProtocolMode),
        .att_desc.max_length = sizeof(hidProtocolMode) },

    // 鼠标输入
    // [i_char_mouseIn] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
    //     .att_desc.uuid_length = 2,
    //     .att_desc.uuid_p = (uint8_t*)&uuid_type_character,
    //     .att_desc.perm = ESP_GATT_PERM_READ,
    //     .att_desc.value = (uint8_t*)&char_prop_read_notify,
    //     .att_desc.length = 1,
    //     .att_desc.max_length = 1 },
    // [i_val_mouseIn] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
    //     .att_desc.uuid_length = 2,
    //     .att_desc.uuid_p = (uint8_t*)&HIDReport_char_uuid,
    //     .att_desc.perm = ESP_GATT_PERM_READ,
    //     .att_desc.value = NULL,
    //     .att_desc.length = 0,
    //     .att_desc.max_length = 10 },
    // [i_ccc_mouseIn] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
    //     .att_desc.uuid_length = 2,
    //     .att_desc.uuid_p = (uint8_t*)&character_client_config_uuid,
    //     .att_desc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
    //     .att_desc.value = 0,
    //     .att_desc.length = 0,
    //     .att_desc.max_length = 2 },
    // [i_des_mouseIn] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
    //     .att_desc.uuid_length = 2,
    //     .att_desc.uuid_p = (uint8_t*)&reportReference_des_uuid,
    //     .att_desc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
    //     .att_desc.value = hidReportRefMouseIn,
    //     .att_desc.length = sizeof(hidReportRefMouseIn),
    //     .att_desc.max_length = sizeof(hidReportRefMouseIn) },

    // 手柄输入
    [i_char_gamepadIn] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
        .att_desc.uuid_length = 2,
        .att_desc.uuid_p = (uint8_t*)&uuid_type_character,
        .att_desc.perm = ESP_GATT_PERM_READ,
        .att_desc.value = (uint8_t*)&char_prop_read_notify,
        .att_desc.length = 1,
        .att_desc.max_length = 1 },
    [i_val_gamepadIn] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
        .att_desc.uuid_length = 2,
        .att_desc.uuid_p = (uint8_t*)&HIDReport_char_uuid,
        .att_desc.perm = ESP_GATT_PERM_READ,
        .att_desc.value = NULL,
        .att_desc.length = 0,
        .att_desc.max_length = 100 },
    [i_ccc_gamepadIn] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
        .att_desc.uuid_length = 2,
        .att_desc.uuid_p = (uint8_t*)&character_client_config_uuid,
        .att_desc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .att_desc.value = 0,
        .att_desc.length = 0,
        .att_desc.max_length = 2 },
    [i_des_gamepadIn] = { .attr_control.auto_rsp = ESP_GATT_AUTO_RSP,
        .att_desc.uuid_length = 2,
        .att_desc.uuid_p = (uint8_t*)&reportReference_des_uuid,
        .att_desc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .att_desc.value = hidReportRefGamepadIn,
        .att_desc.length = sizeof(hidReportRefGamepadIn),
        .att_desc.max_length = sizeof(hidReportRefGamepadIn) },

};
uint16_t hid_handle[sizeof(att_tbl) / sizeof(esp_gatts_attr_db_t)] = { 0 }; // 属性表权柄

// tasks
TaskHandle_t test_task_handle = NULL;
void test_task(void* data);

void adv()
{
    static esp_ble_adv_data_t adv_data = {
        .set_scan_rsp = false, // 扫描是否响应
        .include_name = true, // 是否显示设备名
        .include_txpower = false, // 是否显示发射功率
        .min_interval = 0x0006, // 从机连接最小间隔 Time = min_interval * 1.25 msec
        .max_interval = 0x000C, // 从机连接最大间隔 Time = max_interval * 1.25 msec
        .appearance = 0x03C4, // 设备外观
        .p_manufacturer_data = 0, // 制造商数据
        .p_service_data = 0, // 服务信息
        .p_service_uuid = 0, // 服务数据
        .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT), // 发现模式
    };
    static esp_ble_adv_params_t adv_params = {
        .adv_int_min = 0x20, // 注意着俩参数有范围，超出范围配置无效
        .adv_int_max = 0x40,
        .adv_type = ADV_TYPE_IND, // 广播报文类型 通用广播 在广播包-header-PDU部分
        .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
        .channel_map = ADV_CHNL_ALL, // 广播哪几个信道
        .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY, // 操作策略 可被扫描与连接
    };
    esp_ble_gap_config_adv_data(&adv_data);
    esp_ble_gap_start_advertising(&adv_params);
}
bool connect = false;
static void gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param)
{
    // gap cb
    // 安全响应
    // 配对完成
    // 解绑时
    switch (event) {
    case ESP_GAP_BLE_SEC_REQ_EVT:
        ESP_LOGI("debug", "安全请求响应");
        esp_ble_gap_security_rsp(param->ble_security.ble_req.bd_addr, true);
        break;
    case ESP_GAP_BLE_AUTH_CMPL_EVT:
        ESP_LOGI("debug", "配对状态: %d", param->ble_security.auth_cmpl.success);
        connect = param->ble_security.auth_cmpl.success;
        break;
    case ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT:
        ESP_LOGI("debug", "解绑");
        break;
    default:
        break;
    }
}

esp_gatt_if_t u_gatts_if = NULL;
uint16_t u_conn_id;
void gatts_cb(esp_gatts_cb_event_t event,
    esp_gatt_if_t gatts_if,
    esp_ble_gatts_cb_param_t* param)
{
    // GATTS CB
    // 注册应用时 创建属性表
    // 连接时 停止广播 启用安全连接
    // 断连时 广播
    switch (event) {
    // 连接时
    case ESP_GATTS_CONNECT_EVT:
        u_conn_id = param->connect.conn_id;
        // // 停止广播
        esp_ble_gap_stop_advertising();
        // // 创建属性表
        esp_ble_gatts_create_attr_tab(att_tbl, gatts_if, sizeof(att_tbl) / sizeof(esp_gatts_attr_db_t), 0);
        break;
    // 断连时
    case ESP_GATTS_DISCONNECT_EVT:
        connect = false;
        adv();
        break;

    // 创建属性表后
    case ESP_GATTS_CREAT_ATTR_TAB_EVT:
        // 启动服务
        memcpy(hid_handle, param->add_attr_tab.handles, sizeof(hid_handle));
        esp_ble_gatts_start_service(hid_handle[i_service_hid]);
        break;
    case ESP_GATTS_REG_EVT:
        u_gatts_if = gatts_if;
        break;

    default:
        break;
    }
}

double rocker_compute(double rocker_adc, double rocker_min, double rocker_mid, double rocker_max)
{

    double res = 0;
    if (rocker_adc < rocker_mid)
        res = -(rocker_mid - rocker_adc) / (rocker_mid - rocker_min) * 127;
    else
        res = (rocker_adc - rocker_mid) / (rocker_max - rocker_mid) * 127;
    res = res > 127 ? 127 : res;
    res = res < -127 ? -127 : res;
    return res;
}

void gamepad_send()
{
    uint8_t data[7] = { 0 };
    while (1) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        memset(data, 0, 7);
        if (!connect)
            continue;
        if (BSP_key_get(S1_PIN)) {
            data[0] |= 0x1;

            ESP_LOGI("DEBUG", "S1按下");
        }
        if (BSP_key_get(S2_PIN)) {
            data[0] |= 0x2;

            ESP_LOGI("DEBUG", "S2按下");
        }
        if (BSP_key_get(S3_PIN)) {
            data[0] |= 0x4;

            ESP_LOGI("DEBUG", "S3按下");
        }
        if (BSP_key_get(S4_PIN)) {
            data[0] |= 0x8;

            ESP_LOGI("DEBUG", "S4按下");
        }
        if (BSP_key_get(S5_PIN)) {
            data[0] |= 0x10;

            ESP_LOGI("DEBUG", "S5按下");
        }
        if (BSP_key_get(S6_PIN)) {
            data[0] |= 0x20;

            ESP_LOGI("DEBUG", "S6按下");
        }
        if (BSP_key_get(S7_PIN)) {
            data[0] |= 0x40;

            ESP_LOGI("DEBUG", "S7按下");
        }
        if (BSP_key_get(L_SW_PIN)) {
            data[0] |= 0x80;

            ESP_LOGI("DEBUG", "L_SW_PIN按下");
        }
        if (BSP_key_get(R_SW_PIN)) {
            data[1] |= 0x01;

            ESP_LOGI("DEBUG", "R_SW_PIN按下");
        }
        double lx_ = BSP_rocker_get(LX_CHANNEL);
        double lx = rocker_compute(lx_, 0, 1070, 2500);
        double ly_ = BSP_rocker_get(LY_CHANNEL);
        double ly = rocker_compute(ly_, 0, 1070, 2500);
        double rx_ = BSP_rocker_get(RX_CHANNEL);
        double rx = rocker_compute(rx_, 0, 1110, 2500);
        double ry_ = BSP_rocker_get(RY_CHANNEL);
        double ry = rocker_compute(ry_, 0, 1130, 2500);

        data[2] = -ly;
        data[3] = -lx;
        data[4] = ry;
        data[5] = rx;
        // ESP_LOGI("DEBUG", "LX: %.0f LY: %.0f  LX: %.0f LY: %.0f ", -lx, ly, lx_, ly_);
        ESP_LOGI("DEBUG", "LX: %d LY: %d RX: %d RY: %d LX: %.0f LY: %.0f RX: %.0f RY: %.0f", (int8_t)data[2], (int8_t)data[3], (int8_t)data[4], (int8_t)data[5], lx_, ly_, rx_, ry_);
        esp_ble_gatts_send_indicate(u_gatts_if, u_conn_id, hid_handle[i_val_gamepadIn], 7, data, false);
    }
}

void app_main(void)
{
    // flash
    nvs_flash_erase();
    nvs_flash_init();
    // 控制器
    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    esp_bt_controller_config_t cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init(&cfg);
    esp_bt_controller_enable(ESP_BT_MODE_BLE);
    // 协议栈
    esp_bluedroid_config_t bluedroid_cfg = BT_BLUEDROID_INIT_CONFIG_DEFAULT();
    esp_bluedroid_init_with_cfg(&bluedroid_cfg);
    esp_bluedroid_enable();
    // GAP
    esp_ble_gap_register_callback(gap_cb);
    // GATTS
    esp_ble_gatts_register_callback(gatts_cb);
    // 广播
    esp_ble_gap_set_device_name(DEVICE_NAME);
    adv();
    // 注册应用
    esp_ble_gatts_app_register(APP_ID);
    // MTU
    esp_ble_gatt_set_local_mtu(500);
    // SMP
    esp_ble_io_cap_t iocap = ESP_IO_CAP_NONE; // set the IO capability to No output No input // 配对MITM IO
    esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
    esp_ble_auth_req_t auth_req = ESP_LE_AUTH_BOND; // bonding with peer device after authentication // 绑定
    esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));

    // 按键
    BSP_key_init();

    // 发送按键任务
    xTaskCreate(gamepad_send, "gamepad_send", 8192, NULL, 15, NULL);
}
