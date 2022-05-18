# Tuya IoT Core SDK for Embedded C

Tuya IoT Core SDK is written in C and supports the Tuya-defined things data model, aiming to help developers to connect their proprietary hardware to the Tuya IoT Core. This SDK contains the implementation of essential IoT capabilities including IoT device activation, bidirectional data exchange between the cloud and devices, and OTA updates. It is independent of platforms and operating systems (OS), and it can run on a single-tasking operating system. If your devices support the TCP/IP protocol stack, you can develop with this SDK to connect your devices to the platform.

## Procedure

### Step 1: Download the SDK

[Download TuyaLink SDK from GitHub](https://github.com/tuya/tuya-iot-link-sdk-embedded-c/tree/iotcore-develop).

The following describes the structure of the SDK directory:

- `certs`: Private keys, device certificates, and server-side CA root certificates.
- `docs`: Development documentation.
- `libraries`: External dependencies including MQTT client, HTTP client, and Mbed TLS.
- `interface`: The SDK function API, which is required to port to your platform.
- `include`: SDK APIs.
- `src`: Source code.
- `platform`: Interface adaptation used for porting.
- `utils`: Common tools.
- `examples`: Routines.

### Step 2: Configure device information

To get the platform authorization, you need to create a product on the [Tuya IoT Development Platform](https://iot.tuya.com/). Add the obtained product and authorization information to your code to enable cloud connection.

1. Log in to the [Tuya IoT Development Platform](https://iot.tuya.com/).
2. Click **Create**.
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1638902296e8dfe82332f.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1638902296e8dfe82332f.png)
    
3. Select the required product category. Choose from the category list. For example, in the **Standard Category** tab, click **Small Home Appliances** > **Heater**.
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1638902381970db348319.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1638902381970db348319.png)
    
4. Select **TuyaLink**. Complete the required information and click **Create**.
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/163890245822ba87c75cc.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/163890245822ba87c75cc.png)
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1638902527f8f4344070f.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1638902527f8f4344070f.png)
    
5. In **Function Definition**, click **Add** to add standard functions or create custom functions.
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1639161146d569c61f4b7.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1639161146d569c61f4b7.png)
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1639161183bf96ca96a64.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1639161183bf96ca96a64.png)
    
    <img src="[https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/16388879162e972f21bd8.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/16388879162e972f21bd8.png)" style="zoom: 60%;" />
    
6. In **Device Development**, select and download the SDK and click **Next**.
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1639207743a128ee2f956.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1639207743a128ee2f956.png)
    
7. Tuya offers free licenses for debugging purposes. Click **Get 2 Free Licenses** to request the licenses. Then, register your device using the obtained license and the device information will be shown.
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1638887916bc7f001a5fa.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1638887916bc7f001a5fa.png)
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/16388879163f96800ecd8.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/16388879163f96800ecd8.png)
    
8. Add the device information to the `examples/subdevice_basic_demo/subdevice_basic_demo.c` file. Build and run the program. Then, your device can connect to the cloud. For more information about the build process, see the following **Build and run the demo**.
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/163920781314c8faa9f66.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/163920781314c8faa9f66.png)
    
    ```
    const char productId[] = "rwosj58aaqjk****";
    const char deviceId[] = "6c95875d0f5ba69607****";
    const char deviceSecret[] = "*******************";
    
    ```
    

### Step 3: Build and run the program in Ubuntu

This section uses Ubuntu as an example to show you how to build and run the program. The build processes also apply to Debian system.

1. Install `make` and other dependencies.
    
    ```
    sudo apt-get install make cmake
    
    ```
    
2. Create a folder to save the build output.
    
    ```
    mkdir build && cd build
    cmake ..
    make
    
    ```
    
3. Run the demo. The SDK contains the basic sample code for communication, such as code for sub-device management.
    
    ```
    ./bin/subdevice_basic_demo
    
    ```
    
4. View log files.
If you see **MQTT client connected**, your device is successfully connected to the cloud.
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/16346999546683390039a.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/16346999546683390039a.png)
    
    Go back to the platform and refresh the **Device Status**. You will find it is **Online**.
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/16392176150ef2fb6f74e.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/16392176150ef2fb6f74e.png)
    

## Sample application

1. Instantiate and initialize a device object `tuya_iot_client_t` to assign initial values, such as product ID (PID) and authorization information, for this object.
    
    ```
    /* instantiate the client */
    tuya_mqtt_context_t* client = &client_instance;
    
    /* initialize the client */
    ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t) {
        .host = "m2.tuyacn.com",
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
    
    ```
    
2. Define event callbacks in the application layer, used to receive the event notifications from the SDK, such as the data point (DP) data from the cloud or cloud connection status.
    
    ```
    /* Tuya SDK event callback */
    void on_messages(tuya_mqtt_context_t* context, void* user_data, const tuyalink_message_t* msg)
    {
        TY_LOGI("on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
        switch (msg->type) {
            case THING_TYPE_MODEL_RSP:
                TY_LOGI("Model data:%s", msg->data_string);
                break;
    
            case THING_TYPE_PROPERTY_SET:
                TY_LOGI("property set:%s", msg->data_string);
                break;
    
            case THING_TYPE_PROPERTY_REPORT_RSP:
                break;
    
            default:
                break;
        }
    printf("\\r\\n");
    }
    
    ```
    
3. Start TuyaOS SDK service.
    
    ```
    ret = tuya_mqtt_connect(client);
    // TuyaOS SDK service tasks such as handling data and keepalive mechanism.
    
    ```
    
4. Loop the following function to create threads for the underlying Link SDK client.
    
    ```
    tuya_mqtt_loop(client);
    
    ```
    
    Example of reporting DP data to the cloud:
    
    ```
       /* data model test code */
    tuyalink_thing_data_model_get(context, NULL);
    tuyalink_thing_desired_get(context, NULL, "[\\"power\\"]");
    tuyalink_thing_property_report(context, NULL, "{\\"power\\":{\\"value\\":1234,\\"time\\":1631708204231}}");
    tuyalink_thing_property_report_with_ack(context, NULL, "{\\"power\\":{\\"value\\":1234,\\"time\\":1631708204231}}");
    tuyalink_thing_event_trigger(context, NULL, "{\\"eventCode\\":\\"boom\\",\\"eventTime\\":1626197189630,\\"outputParams\\":{\\"param1\\":100}}");
    tuyalink_thing_batch_report(context, "{\\"msgId\\":\\"45lkj3551234001\\",\\"time\\":1626197189638,\\"sys\\":{\\"ack\\":0},\\"data\\":{\\"properties\\":{\\"power\\":{\\"value\\":11,\\"time\\":1626197189638}},\\"events\\":{\\"boom\\":{\\"outputParams\\":{\\"param1\\":\\"10\\"},\\"eventTime\\":1626197189001}}}}");}
     /* subdevice test code */
    tuyalink_subdevice_bind(context, "[{\\"productId\\":\\"jtwe4q9jrs0bbc8q\\",\\"nodeId\\":\\"123456\\",\\"clientId\\":\\"123455asdf\\"}]");
    tuyalink_subdevice_bind_login(context, "[\\"6c17c5ba952143f592b8g1\\",\\"6c41626e5cea758aees0ik\\"]");
    tuyalink_subdevice_bind_logout(context, "[\\"6c17c5ba952143f592b8g1\\"]");
    tuyalink_subdevice_topo_add(context, "[{\\"productId\\":\\"jtwe4q9jrs0bbc8q\\",\\"deviceId\\":\\"6c17c5ba952143f592b8g1\\",\\"sign\\":\\"366508ed895644e70a3006bdef2dbe77ef73e18a\\",\\"signMethod\\":\\"hmacSha1\\",\\"timestamp\\":\\"1636989480\\"}]");
    tuyalink_subdevice_topo_delete(context,"[\\"6c41626e5cea758aees0ik\\"]");
    tuyalink_subdevice_topo_get(context);
    
    ```
    

## Code debugging

When a device is connected to the MQTT server and goes online, you can start **debugging code online**.

1. Click the **Online Debug** tab and **select a device**. Enter the **DeviceId** of the online device to show the collection of the defined data points (DPs).
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/163912953959473ef46ea.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/163912953959473ef46ea.png)
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1639129538a23f9c0c49d.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1639129538a23f9c0c49d.png)
    
2. The right screen shows the device logs in real time.
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1639129538506fd3ea739.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1639129538506fd3ea739.png)
    
3. You can click **Get** to request the current device data.
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1639129539b283a8b2996.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1639129539b283a8b2996.png)
    
4. You can click **Set** to change the property value. The payload data will be displayed in the log on the right screen. You can also view the payload in the local log file.
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/163912953912eb24fe0fe.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/163912953912eb24fe0fe.png)
    
    ![https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/163877562080db9c488a9.png](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/163877562080db9c488a9.png)
    

## API description

### SDK initialization

[Untitled](https://www.notion.so/0982f9a73bf54037bb037822978b3219)

### Start services

[Untitled](https://www.notion.so/32afc1d6f81e4ffb941b3d3d196a1103)

### Stop services

[Untitled](https://www.notion.so/245fa4f3ad2d41f2b42c7a94e6fb637b)

### Run in background

[Untitled](https://www.notion.so/aef3abeefe1e488dae3432df9818446e)

### Get things data model

[Untitled](https://www.notion.so/5eaa41b201f14885980acb7ebc95fdcf)

### Report device properties

[Untitled](https://www.notion.so/9c123404283349e2b832a72a7a09843d)

### Report device properties (with ACK)

[Untitled](https://www.notion.so/2607abab374346bf84d01e86f71f99ed)

### Respond to device events

[Untitled](https://www.notion.so/b75f65c0f5ce45fea951ec764af5e63f)

### Report status in bulk

[Untitled](https://www.notion.so/224ff52e61de43099f7fb5b1241815f9)

### Bind sub-devices

[Untitled](https://www.notion.so/f08a019a4f4448319230af2d1a647227)

### Sub-device goes online

[Untitled](https://www.notion.so/8e8b6296d3034c99b180c70fffc48ed8)

### Sub-device goes offline

[Untitled](https://www.notion.so/f6bab9b4db9e404781f6d4a6ea5ba414)

### Add topology for sub-devices

[Untitled](https://www.notion.so/045fde6ee58647d0adf5bb509290cdc5)

### Delete topology for sub-devices

[Untitled](https://www.notion.so/245a1d4c094244858e58d00ace518637)

### Get topology for sub-devices

[Untitled](https://www.notion.so/2e3a81802ef3408dacae4da887943558)

## Example:

```
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

const char productId[] = "3jbcpefnn1jxxxxx";
const char deviceId[] = "6ced2aa564727c01xxxxx";
const char deviceSecret[] = "ac5d367db39xxxxx";

tuya_mqtt_context_t client_instance;

void on_connected(tuya_mqtt_context_t* context, void* user_data)
{
    TY_LOGI("on connected");

    /* data model test code */
    tuyalink_thing_data_model_get(context, NULL);
    tuyalink_thing_desired_get(context, NULL, "[\\"power\\"]");
    tuyalink_thing_property_report(context, NULL, "{\\"power\\":{\\"value\\":1234,\\"time\\":1631708204231}}");
    tuyalink_thing_property_report_with_ack(context, NULL, "{\\"power\\":{\\"value\\":1234,\\"time\\":1631708204231}}");
    tuyalink_thing_event_trigger(context, NULL, "{\\"eventCode\\":\\"boom\\",\\"eventTime\\":1626197189630,\\"outputParams\\":{\\"param1\\":100}}");
    tuyalink_thing_batch_report(context, "{\\"msgId\\":\\"45lkj3551234001\\",\\"time\\":1626197189638,\\"sys\\":{\\"ack\\":0},\\"data\\":{\\"properties\\":{\\"power\\":{\\"value\\":11,\\"time\\":1626197189638}},\\"events\\":{\\"boom\\":{\\"outputParams\\":{\\"param1\\":\\"10\\"},\\"eventTime\\":1626197189001}}}}");
}

void on_disconnect(tuya_mqtt_context_t* context, void* user_data)
{
    TY_LOGI("on disconnect");
}

void on_messages(tuya_mqtt_context_t* context, void* user_data, const tuyalink_message_t* msg)
{
    TY_LOGI("on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
    switch (msg->type) {
        case THING_TYPE_MODEL_RSP:
            TY_LOGI("Model data:%s", msg->data_string);
            break;

        case THING_TYPE_PROPERTY_SET:
            TY_LOGI("property set:%s", msg->data_string);
            break;

        case THING_TYPE_PROPERTY_REPORT_RSP:
            break;

        default:
            break;
    }
    printf("\\r\\n");
}

int main(int argc, char** argv)
{
    int ret = OPRT_OK;

    tuya_mqtt_context_t* client = &client_instance;

    ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t) {
        .host = "m2.tuyacn.com",
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

```