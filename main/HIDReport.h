/**
 * HIDReport.h
 * @tingzhong666
 * 2024-04-20 20:17:41
 */

#ifndef HIDREPORT_H
#define HIDREPORT_H

#ifdef __cplusplus
extern "C" {
#endif

char ReportDescriptor[] = {
    // 鼠标 3byte 左中右3bit+保留5bit+xy2byte=3byte
    // 0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    // 0x09, 0x02, // USAGE (Mouse)
    // 0x85, 0x01, // REPORT_ID(1)
    // 0xa1, 0x01, // COLLECTION (Application)
    // 0x09, 0x01, //   USAGE (Pointer)
    // 0xa1, 0x00, //   COLLECTION (Physical)
    // 0x05, 0x09, //     USAGE_PAGE (Button)
    // 0x19, 0x01, //     USAGE_MINIMUM (Button 1)
    // 0x29, 0x03, //     USAGE_MAXIMUM (Button 3)
    // 0x15, 0x00, //     LOGICAL_MINIMUM (0)
    // 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
    // 0x95, 0x03, //     REPORT_COUNT (3)
    // 0x75, 0x01, //     REPORT_SIZE (1)
    // 0x81, 0x02, //     INPUT (Data,Var,Abs)
    // 0x95, 0x01, //     REPORT_COUNT (1)
    // 0x75, 0x05, //     REPORT_SIZE (5)
    // 0x81, 0x03, //     INPUT (Cnst,Var,Abs)
    // 0x05, 0x01, //     USAGE_PAGE (Generic Desktop)
    // 0x09, 0x30, //     USAGE (X)
    // 0x09, 0x31, //     USAGE (Y)
    // 0x15, 0x81, //     LOGICAL_MINIMUM (-127)
    // 0x25, 0x7f, //     LOGICAL_MAXIMUM (127)
    // 0x75, 0x08, //     REPORT_SIZE (8)
    // 0x95, 0x02, //     REPORT_COUNT (2)
    // 0x81, 0x06, //     INPUT (Data,Var,Rel)
    // 0xc0, //   END_COLLECTION
    // 0xc0, // END_COLLECTION

    // 手柄 6byte = 16按键2byte+2摇杆xy4byte
    0x05, 0x01, // Usage Page (Generic Desktop Ctrls)
    0x09, 0x05, // Usage (Game Pad)
    0x85, 0x02, // Report ID (2)
    0xA1, 0x01, // Collection (Application)
    0x05, 0x09, //   Usage Page (Button)
    0x19, 0x01, //   Usage Minimum (Button 1)
    0x29, 0x10, //   Usage Maximum (Button 16)
    0x15, 0x00, //   Logical Minimum (0)
    0x25, 0x01, //   Logical Maximum (1)
    0x75, 0x01, //   Report Size (1)
    0x95, 0x10, //   Report Count (16)
    0x81, 0x02, //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01, //   Usage Page (Generic Desktop Ctrls)
    0x15, 0x81, //   Logical Minimum (-127)
    0x25, 0x7F, //   Logical Maximum (127)
    0x09, 0x30, //   Usage (X)
    0x09, 0x31, //   Usage (Y)
    // 0x09, 0x33, //   Usage (RX)
    // 0x09, 0x34, //   Usage (RY)
    0x09, 0x32, //   Usage (z)
    0x09, 0x35, //   Usage (Rz)
    0x75, 0x08, //   Report Size (8)
    0x95, 0x04, //   Report Count (4)
    0x81, 0x02, //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0, // End Col

    // 0x05, 0x01, // 0       GLOBAL_USAGE_PAGE(Generic Desktop Controls)
    // 0x09, 0x05, // 2       LOCAL_USAGE(Game Pad)
    // 0xA1, 0x01, // 4       MAIN_COLLECTION(Applicatior)
    // 0xA1, 0x00, // 6       MAIN_COLLECTION(Physical)
    // 0x09, 0x30, // 8       LOCAL_USAGE(X)
    // 0x09, 0x31, // 10      LOCAL_USAGE(Y)
    // 0x15, 0x00, // 12      GLOBAL_LOGICAL_MINIMUM(0)
    // 0x26, 0xFF, 0xFF, // 14      GLOBAL_LOCAL_MAXIMUM(-1)
    // 0x35, 0x00, // 17      GLOBAL_PHYSICAL_MINIMUM(0)
    // 0x46, 0xFF, 0xFF, // 19      GLOBAL_PHYSICAL_MAXIMUM(65535)
    // 0x95, 0x02, // 22      GLOBAL_REPORT_COUNT(2)
    // 0x75, 0x10, // 24      GLOBAL_REPORT_SIZE(16)
    // 0x81, 0x02, // 26      MAIN_INPUT(data var absolute NoWrap linear PreferredState NoNullPosition NonVolatile )
    // 0xC0, //, 28      MAIN_COLLECTION_END
    // 0xA1, 0x00, // 29      MAIN_COLLECTION(Physical)
    // 0x09, 0x33, // 31      LOCAL_USAGE(Rx)
    // 0x09, 0x34, // 33      LOCAL_USAGE(Ry)
    // 0x15, 0x00, // 35      GLOBAL_LOGICAL_MINIMUM(0)
    // 0x26, 0xFF, 0xFF, // 37      GLOBAL_LOCAL_MAXIMUM(-1)
    // 0x35, 0x00, // 40      GLOBAL_PHYSICAL_MINIMUM(0)
    // 0x46, 0xFF, 0xFF, // 42      GLOBAL_PHYSICAL_MAXIMUM(65535)
    // 0x95, 0x02, // 45      GLOBAL_REPORT_COUNT(2)
    // 0x75, 0x10, // 47      GLOBAL_REPORT_SIZE(16)
    // 0x81, 0x02, // 49      MAIN_INPUT(data var absolute NoWrap linear PreferredState NoNullPosition NonVolatile )
    // 0xC0, //, 51      MAIN_COLLECTION_END
    // 0xA1, 0x00, // 52      MAIN_COLLECTION(Physical)
    // 0x09, 0x32, // 54      LOCAL_USAGE(Z)
    // 0x15, 0x00, // 56      GLOBAL_LOGICAL_MINIMUM(0)
    // 0x26, 0xFF, 0xFF, // 58      GLOBAL_LOCAL_MAXIMUM(-1)
    // 0x35, 0x00, // 61      GLOBAL_PHYSICAL_MINIMUM(0)
    // 0x46, 0xFF, 0xFF, // 63      GLOBAL_PHYSICAL_MAXIMUM(65535)
    // 0x95, 0x01, // 66      GLOBAL_REPORT_COUNT(1)
    // 0x75, 0x10, // 68      GLOBAL_REPORT_SIZE(16)
    // 0x81, 0x02, // 70      MAIN_INPUT(data var absolute NoWrap linear PreferredState NoNullPosition NonVolatile )
    // 0xC0, //, 72      MAIN_COLLECTION_END
    // 0x05, 0x09, // 73      GLOBAL_USAGE_PAGE(Button)
    // 0x19, 0x01, // 75      LOCAL_USAGE_MINIMUM(1)
    // 0x29, 0x0A, // 77      LOCAL_USAGE_MAXIMUM(10)
    // 0x95, 0x0A, // 79      GLOBAL_REPORT_COUNT(10)
    // 0x75, 0x01, // 81      GLOBAL_REPORT_SIZE(1)
    // 0x81, 0x02, // 83      MAIN_INPUT(data var absolute NoWrap linear PreferredState NoNullPosition NonVolatile )
    // 0x05, 0x01, // 85      GLOBAL_USAGE_PAGE(Generic Desktop Controls)
    // 0x09, 0x39, // 87      LOCAL_USAGE(Hat switch)
    // 0x15, 0x01, // 89      GLOBAL_LOGICAL_MINIMUM(1)
    // 0x25, 0x08, // 91      GLOBAL_LOCAL_MAXIMUM(8)
    // 0x35, 0x00, // 93      GLOBAL_PHYSICAL_MINIMUM(0)
    // 0x46, 0x3B, 0x10, // 95      GLOBAL_PHYSICAL_MAXIMUM(4155)
    // 0x66, 0x0E, 0x00, // 98      GLOBAL_REPORT_UNIT(14)
    // 0x75, 0x04, // 101     GLOBAL_REPORT_SIZE(4)
    // 0x95, 0x01, // 103     GLOBAL_REPORT_COUNT(1)
    // 0x81, 0x42, // 105     MAIN_INPUT(data var absolute NoWrap linear PreferredState NullState NonVolatile )
    // 0x75, 0x02, // 107     GLOBAL_REPORT_SIZE(2)
    // 0x95, 0x01, // 109     GLOBAL_REPORT_COUNT(1)
    // 0x81, 0x03, // 111     MAIN_INPUT(const var absolute NoWrap linear PreferredState NoNullPosition NonVolatile )
    // 0x75, 0x08, // 113     GLOBAL_REPORT_SIZE(8)
    // 0x95, 0x02, // 115     GLOBAL_REPORT_COUNT(2)
    // 0x81, 0x03, // 117     MAIN_INPUT(const var absolute NoWrap linear PreferredState NoNullPosition NonVolatile )
    // 0xC0, //, 119     MAIN_COLLECTION_END
};

#ifdef __cplusplus
}
#endif

#endif
