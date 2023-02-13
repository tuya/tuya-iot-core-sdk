#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "cJSON.h"
#include "tuya_cacert.h"
#include "tuya_log.h"
#include "tuya_error_code.h"
#include "system_interface.h"
#include "mqtt_client_interface.h"
#include "tuyalink_core.h"
#include "tuya_endpoint.h"

const char productId[] = "al0fhe2uqyg6pzeb";
const char deviceId[] = "264fcea74768270efdcer6";
const char deviceSecret[] = "ca690fedd80a1a03";

tuya_mqtt_context_t client_instance;

void on_connected(tuya_mqtt_context_t* context, void* user_data)
{
    TY_LOGI("on connected");

    /* data model test code */
    /* Set region regist */
    tuya_endpoint_region_regist_set("AY","pro");
    /* Update endpoint CA certificate */
    tuya_endpoint_update_auto_region();

    /* Initialize device firmware version */
    tuyalink_ota_firmware_report(context,deviceId,"{\"bizType\":\"INIT\",\"pid\":\"al0fhe2uqyg6pzeb\",\"otaChannel\":[{\"channel\":0,\"version\":\"1.0.0\"},{\"channel\":9,\"version\":\"1.0.0\"}]}");
    system_sleep(2000);
    /* Get firmware upgrade */
    tuyalink_ota_get(context,deviceId);
    system_sleep(1000);
    /* Report OTA progress */
    tuyalink_ota_progress_report(context,deviceId,"{\"channel\":0,\"progress\":98}");
    system_sleep(1000);
    /* Report upgrade status */
    tuyalink_ota_progress_report(context,deviceId,"{\"channel\":0,\"errorCode\":42,\"errorMsg\":\"下载失败,FLASH空间不足\"}");
    system_sleep(2000);
    /* Update device firmware version */
    tuyalink_ota_firmware_report(context,deviceId,"{\"bizType\":\"UPDATE\",\"otaChannel\":[{\"channel\":0,\"version\":\"1.0.6\"},{\"channel\":9,\"version\":\"1.0.0\"}]}");
    
    system_sleep(2000);
    /* NTP */
    tuyalink_time_get(context,"{\"bizType\":\"NTP\",\"dst\":1670232110835}");

    /* DST */
    tuyalink_time_get(context,"{\"bizType\":\"DST\",\"timezoneId\":\"asia/shanghai\"}");
}

void on_disconnect(tuya_mqtt_context_t* context, void* user_data)
{
    TY_LOGI("on disconnect");
}

void on_messages(tuya_mqtt_context_t* context, void* user_data, const tuyalink_message_t* msg)
{
    TY_LOGI("on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
    switch (msg->type) {
        case THING_TYPE_OTA_ISSUE:
            TY_LOGI("Upgrade data:%s", msg->data_string);
            break;

        case THING_TYPE_OTA_GET_RSP:
            TY_LOGI("OTA slient upgrade get response:%s", msg->data_string);
            break;
        
        case THING_TYPE_EXT_TIME_RESPONSE:
            TY_LOGI("Time response:%s", msg->data_string);
            break;

        default:
            break;
    }
    printf("\r\n");
}

int main(int argc, char** argv)
{
    int ret = OPRT_OK;

    tuya_mqtt_context_t* client = &client_instance;

    ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t) {
        .host = "m1.tuyacn.com",
        //.host = "m1-cn.wgine.com",
        .port = 8883,
        .cacert = tuya_cacert_pem,
        .cacert_len = sizeof(tuya_cacert_pem),
        .device_id = deviceId,
        .device_secret = deviceSecret,
        .keepalive = 60,
        .timeout_ms = 2000,
        .on_connected = on_connected,
        .on_disconnect = on_disconnect,
        .on_messages = on_messages
    });
    assert(ret == OPRT_OK);

    ret = tuya_mqtt_connect(client);
    assert(ret == OPRT_OK);

    for (;;) {
        /* Loop to receive packets, and handles client keepalive */
        tuya_mqtt_loop(client);
    }

    return ret;
}
