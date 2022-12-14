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

const char productId[] = "whmr****";
const char deviceId[] = "26b3198330e98b0105****";
const char deviceSecret[] = "df55e992168a****";

tuya_mqtt_context_t client_instance;

void on_connected(tuya_mqtt_context_t *context, void *user_data)
{
    TY_LOGI("on connected");

    // subscribe topic:tylink/${deviceId}/channel/raw/down
    char raw_down_topic[64];
    sprintf(raw_down_topic, "tylink/%s/channel/raw/down", deviceId);
    mqtt_client_subscribe(context, raw_down_topic, MQTT_QOS_1);

    /* data model test code */
    /**
     * 示例1：上报红外夜视属性数据
     * 传入参数：
     *	    [0x31,0x2c,0x33,0x2c,0x31,0x2c,0x30,0x2c,0x31,0x2c,0x31,0x34,0x2c,0x31,0x36,0x33,0x37,0x31,0x31,0x34,0x38,0x39,0x32,0x32,0x38,0x37,0x2c,0x4f,0x4e]
     * 输出结果：
     *	    {"msgType":"thing.property.report","payload":{"msgId":"14", "time":1637114892287, "sys":{"ack":1}, "data":{"nightvision":{"value":"ON","time": 1637114892287}}}}
     */
    char propertyData[] = {0x31, 0x2c, 0x33, 0x2c, 0x31, 0x2c, 0x30, 0x2c, 0x31, 0x2c, 0x31, 0x34, 0x2c, 0x31, 0x36, 0x33, 0x37, 0x31, 0x31, 0x34, 0x38, 0x39, 0x32, 0x32, 0x38, 0x37, 0x2c, 0x4f, 0x4e};
    tuyalink_raw_up(context, propertyData);
    system_sleep(1000);

    /**
     * 示例2：上报设备故障事件
     * 传入参数：
     *      [0x31,0x2c,0x37,0x2c,0x32,0x2c,0x30,0x2c,0x31,0x2c,0x31,0x36,0x2c,0x31,0x36,0x33,0x37,0x31,0x31,0x34,0x38,0x39,0x32,0x32,0x38,0x37,0x2c,0x35]
     * 输出结果：
     *	    {"msgType":"thing.event.trigger", "payload":{"msgId":"16",  "time":1637114892287,"sys":{"ack":1}, "data":{"eventCode":"fault", "outputParams":{"faultNumber":5}}}}
     */
    char eventData[] = {0x31, 0x2c, 0x37, 0x2c, 0x32, 0x2c, 0x30, 0x2c, 0x31, 0x2c, 0x31, 0x36, 0x2c, 0x31, 0x36, 0x33, 0x37, 0x31, 0x31, 0x34, 0x38, 0x39, 0x32, 0x32, 0x38, 0x37, 0x2c, 0x35};
    tuyalink_raw_up(context, eventData);
    system_sleep(1000);

    /**
     * 示例3：上报设备动作结果
     * 传入参数：
     *   	[0x31,0x2c,0x36,0x2c,0x30,0x2c,0x30,0x2c,0x30,0x2c,0x31,0x37,0x2c,0x31,0x36,0x33,0x37,0x31,0x31,0x34,0x38,0x39,0x32,0x32,0x38,0x37]
     * 输出结果：
     *		{"msgType":"thing.action.execute.response", "payload":{"msgId":"17",  "time":1637114892287, "code":0}}
     */
    char actionRspData[] = {0x31, 0x2c, 0x36, 0x2c, 0x30, 0x2c, 0x30, 0x2c, 0x30, 0x2c, 0x31, 0x37, 0x2c, 0x31, 0x36, 0x33, 0x37, 0x31, 0x31, 0x34, 0x38, 0x39, 0x32, 0x32, 0x38, 0x37};
    tuyalink_raw_up(context, actionRspData);
    system_sleep(1000);
}

void on_disconnect(tuya_mqtt_context_t *context, void *user_data)
{
    TY_LOGI("on disconnect");
}

void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg)
{
    TY_LOGI("on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
    switch (msg->type)
    {
    case THING_TYPW_CHANNEL_RAW_DOWN:
        TY_LOGI("Custom protocol data:%s", msg->data_string);
        break;
    default:
        break;
    }
    printf("\r\n");
}

int main(int argc, char **argv)
{
    int ret = OPRT_OK;

    tuya_mqtt_context_t *client = &client_instance;

    ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t){
                                     .host = "m1.tuyacn.com",
                                     .port = 8883,
                                     .cacert = tuya_cacert_pem,
                                     .cacert_len = sizeof(tuya_cacert_pem),
                                     .device_id = deviceId,
                                     .device_secret = deviceSecret,
                                     .keepalive = 60,
                                     .timeout_ms = 2000,
                                     .on_connected = on_connected,
                                     .on_disconnect = on_disconnect,
                                     .on_messages = on_messages});
    assert(ret == OPRT_OK);

    ret = tuya_mqtt_connect(client);
    assert(ret == OPRT_OK);

    for (;;)
    {
        /* Loop to receive packets, and handles client keepalive */
        tuya_mqtt_loop(client);
    }

    return ret;
}
