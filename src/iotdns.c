#include <stdio.h>
#include <stddef.h>
#include "tuya_config_defaults.h"
#include "tuya_endpoint.h"
#include "tuya_log.h"
#include "cJSON.h"
#include "base64.h"
#include "tuya_error_code.h"
#include "system_interface.h"
#include "storage_interface.h"
#include "http_client_interface.h"

#define IOTDNS_REQUEST_FMT "{\"config\":[{\"key\":\"mqttsUrl\",\"need_ca\":true}],\"region\":\"%s\",\"env\":\"%s\"}"
#define IOTDNS_REQUEST_FMT_NOREGION "{\"config\":[{\"key\":\"mqttsUrl\",\"need_ca\":true}],\"env\":\"%s\"}"
#define IOTDNS_CACERT_REQUEST_FMT "[{\"host\":\"%s\",\"port\":%d,\"need_ca\":true,\"need_ip\":false,\"need_ip6\":false}]"

const uint8_t iot_dns_cert_der[] = {
    "-----BEGIN CERTIFICATE-----\n"
    "MIICGDCCAb2gAwIBAgIRAI4kVSI/DR6TlRqvv0C7A4EwCgYIKoZIzj0EAwIwNTEdMBsGA1UECgwU\n"
    "U2luYmF5IEdyb3VwIExpbWl0ZWQxFDASBgNVBAMMC0Nsb3VkIFJDQSAyMCAXDTIyMDUzMTE2MDAw\n"
    "MFoYDzIwNzIwNjMwMTU1OTU5WjA1MR0wGwYDVQQKDBRTaW5iYXkgR3JvdXAgTGltaXRlZDEUMBIG\n"
    "A1UEAwwLQ2xvdWQgUkNBIDIwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAATqjfuzyXh8P0MuuWrH\n"
    "PUSoOp9OqsSHnCvDL18EK/Wfo1MOaQoIAy82zaC+ggjQph0AwCICTfzauMr0AUKw28Vko4GrMIGo\n"
    "MA4GA1UdDwEB/wQEAwIBBjBFBgNVHSUEPjA8BggrBgEFBQcDAQYIKwYBBQUHAwIGCCsGAQUFBwMD\n"
    "BggrBgEFBQcDCAYIKwYBBQUHAwQGCCsGAQUFBwMJMA8GA1UdEwQIMAYBAf8CAQEwHwYDVR0jBBgw\n"
    "FoAUjW5pdbOF5Bmvn+MrD+yG6tcJ7yowHQYDVR0OBBYEFI1uaXWzheQZr5/jKw/shurXCe8qMAoG\n"
    "CCqGSM49BAMCA0kAMEYCIQDaNnFTr66LnhYY+55C234I7MWBveU3RLg5pcVzb5EYUAIhAJN4+4go\n"
    "F3rrb03/o2AsmPMLLZ+UjTjeCXrTXUyxBt2N\n"
    "-----END CERTIFICATE-----\n"};

static int iotdns_cacert_response_decode(const uint8_t *input, size_t ilen, tuya_endpoint_t *endport)
{
    cJSON *root = cJSON_Parse((const char *)input);
    if (root == NULL)
    {
        return OPRT_CJSON_PARSE_ERR;
    }

    if (cJSON_GetObjectItem(root, "mqttsUrl") == NULL)
    {
        return OPRT_CR_CJSON_ERR;
    }

    char *mqttsUrl = cJSON_GetObjectItem(cJSON_GetArrayItem(root, 0), "host")->valuestring;
    char *caArr0 = cJSON_GetObjectItem(cJSON_GetArrayItem(root, 0), "ca")->valuestring;
    TY_LOGV("mqttsUrl:%s", mqttsUrl);

    /* cert decode */
    // base64 decode buffer
    size_t caArr0_len = strlen(caArr0);
    size_t buffer_len = caArr0_len * 3 / 4;
    uint8_t *caArr_raw = system_malloc(buffer_len);
    size_t caArr_raw_len = 0;

    // base64 decode
    if (mbedtls_base64_decode(caArr_raw, buffer_len, &caArr_raw_len, (const uint8_t *)caArr0, caArr0_len) != 0)
    {
        TY_LOGE("base64 decode error");
        system_free(caArr_raw);
        cJSON_Delete(root);
        return OPRT_COM_ERROR;
    }

    endport->mqtt.cert = caArr_raw;
    endport->mqtt.cert_len = caArr_raw_len;
    cJSON_Delete(root);
    return OPRT_OK;
}

int iotdns_cacert_get(const char *host, const uint16_t port, tuya_endpoint_t *endport)
{
    if (NULL == host || NULL == endport)
    {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;
    http_client_status_t http_status;

    /* POST data buffer */
    size_t body_length = 0;
    char *body_buffer = system_malloc(128);
    if (NULL == body_buffer)
    {
        TY_LOGE("body_buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    body_length = sprintf(body_buffer, IOTDNS_CACERT_REQUEST_FMT, host, port);
    TY_LOGV("out post data len:%d, data:%s", body_length, body_buffer);

    /* HTTP headers */
    http_client_header_t headers[] = {
        {.key = "Content-Type", .value = "application/x-www-form-urlencoded;charset=UTF-8"},
    };
    uint8_t headers_count = sizeof(headers) / sizeof(http_client_header_t);

    /* Response buffer length preview */
    uint8_t *response_buffer = NULL;
    size_t response_buffer_length = 1024 * 6;

    /* response buffer make */
    response_buffer = system_calloc(1, response_buffer_length);
    if (NULL == response_buffer)
    {
        TY_LOGE("response_buffer malloc fail");
        system_free(body_buffer);
        return OPRT_MALLOC_FAILED;
    }
    http_client_response_t http_response = {
        .buffer = response_buffer,
        .buffer_length = response_buffer_length};

    /* HTTP Request send */
    TY_LOGD("http request send!");
    http_status = http_client_request(
        &(const http_client_request_t){
            .cacert = (uint8_t *)iot_dns_cert_der,
            .cacert_len = sizeof(iot_dns_cert_der),
            .host = "h6-cn.iot-dns.com",
            .port = 443,
            .method = "POST",
            .path = "/device/dns_query",
            .headers = headers,
            .headers_count = headers_count,
            .body = (const uint8_t *)body_buffer,
            .body_length = body_length,
            .timeout_ms = HTTP_TIMEOUT_MS_DEFAULT,
        },
        &http_response);

    /* Release http buffer */
    system_free(body_buffer);

    if (HTTP_CLIENT_SUCCESS != http_status)
    {
        TY_LOGE("http_request_send error:%d", http_status);
        system_free(response_buffer);
        return OPRT_LINK_CORE_HTTP_CLIENT_SEND_ERROR;
    }

    /* Decoded response data */
    rt = iotdns_cacert_response_decode(http_response.body, http_response.body_length, endport);
    system_free(response_buffer);
    return rt;
}

static int iotdns_response_decode(const uint8_t *input, size_t ilen, tuya_endpoint_t *endport)
{
    cJSON *root = cJSON_Parse((const char *)input);
    if (root == NULL)
    {
        return OPRT_CJSON_PARSE_ERR;
    }

    if (cJSON_GetObjectItem(root, "mqttsUrl") == NULL)
    {
        return OPRT_CR_CJSON_ERR;
    }

    char *mqttsUrl = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "mqttsUrl"), "addr")->valuestring;
    // char *caArr0 = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "caArr"), 0)->valuestring;
    TY_LOGV("mqttsUrl:%s", mqttsUrl);

    /* MQTT host decode */
    int port = 443;
    sscanf(mqttsUrl, "%99[^:]:%99d[^\n]", endport->mqtt.host, &port);
    endport->mqtt.port = (uint16_t)port;
    TY_LOGV("endport->mqtt.host = \"%s\"", endport->mqtt.host);
    TY_LOGV("endport->mqtt.port = %d", endport->mqtt.port);

    cJSON_Delete(root);
    return OPRT_OK;
}

int iotdns_cloud_endpoint_get(const char *region, const char *env, tuya_endpoint_t *endport)
{
    if (NULL == env || NULL == endport)
    {
        return OPRT_INVALID_PARM;
    }

    int rt = OPRT_OK;
    http_client_status_t http_status;

    /* POST data buffer */
    size_t body_length = 0;
    char *body_buffer = system_malloc(128);
    if (NULL == body_buffer)
    {
        TY_LOGE("body_buffer malloc fail");
        return OPRT_MALLOC_FAILED;
    }

    if (region)
    {
        body_length = sprintf(body_buffer, IOTDNS_REQUEST_FMT, region, env);
    }
    else
    {
        body_length = sprintf(body_buffer, IOTDNS_REQUEST_FMT_NOREGION, env);
    }
    TY_LOGV("out post data len:%d, data:%s", body_length, body_buffer);

    /* HTTP headers */
    http_client_header_t headers[] = {
        {.key = "Content-Type", .value = "application/x-www-form-urlencoded;charset=UTF-8"},
    };
    uint8_t headers_count = sizeof(headers) / sizeof(http_client_header_t);

    /* Response buffer length preview */
    uint8_t *response_buffer = NULL;
    size_t response_buffer_length = 1024 * 6;

    /* response buffer make */
    response_buffer = system_calloc(1, response_buffer_length);
    if (NULL == response_buffer)
    {
        TY_LOGE("response_buffer malloc fail");
        system_free(body_buffer);
        return OPRT_MALLOC_FAILED;
    }
    http_client_response_t http_response = {
        .buffer = response_buffer,
        .buffer_length = response_buffer_length};

    /* HTTP Request send */
    TY_LOGD("http request send!");
    http_status = http_client_request(
        &(const http_client_request_t){
            .cacert = (uint8_t *)iot_dns_cert_der,
            .cacert_len = sizeof(iot_dns_cert_der),
            .host = "h6-cn.iot-dns.com",
            .port = 443,
            .method = "POST",
            .path = "/v2/url_config",
            .headers = headers,
            .headers_count = headers_count,
            .body = (const uint8_t *)body_buffer,
            .body_length = body_length,
            .timeout_ms = HTTP_TIMEOUT_MS_DEFAULT,
        },
        &http_response);

    /* Release http buffer */
    system_free(body_buffer);

    if (HTTP_CLIENT_SUCCESS != http_status)
    {
        TY_LOGE("http_request_send error:%d", http_status);
        system_free(response_buffer);
        return OPRT_LINK_CORE_HTTP_CLIENT_SEND_ERROR;
    }

    /* Decoded response data */
    rt = iotdns_response_decode(http_response.body, http_response.body_length, endport);

    /* get iotdns ca certificate */
    rt = iotdns_cacert_get(endport->mqtt.host, endport->mqtt.port, endport);

    if (region)
    {
        strcpy(endport->region, region);
    }
    system_free(response_buffer);
    return rt;
}