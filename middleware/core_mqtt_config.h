#ifndef _CORE_MQTT_CONFIG_H_
#define _CORE_MQTT_CONFIG_H_

/**************************************************/
/******* DO NOT CHANGE the following order ********/
/**************************************************/


/************ End of logging configuration ****************/

/**
 * @brief Determines the maximum number of MQTT PUBLISH messages, pending
 * acknowledgement at a time, that are supported for incoming and outgoing
 * direction of messages, separately.
 *
 * QoS 1 and 2 MQTT PUBLISHes require acknowledgement from the server before
 * they can be completed. While they are awaiting the acknowledgement, the
 * client must maintain information about their state. The value of this
 * macro sets the limit on how many simultaneous PUBLISH states an MQTT
 * context maintains, separately, for both incoming and outgoing direction of
 * PUBLISHes.
 *
 * @note The MQTT context maintains separate state records for outgoing
 * and incoming PUBLISHes, and thus, 2 * MQTT_STATE_ARRAY_MAX_COUNT amount
 * of memory is statically allocated for the state records.
 */
#define MQTT_STATE_ARRAY_MAX_COUNT    ( 10U )

/**
 * @brief Number of milliseconds to wait for a ping response to a ping
 * request as part of the keep-alive mechanism.
 *
 * If a ping response is not received before this timeout, then
 * #MQTT_ProcessLoop will return #MQTTKeepAliveTimeout.
 */
#define MQTT_PINGRESP_TIMEOUT_MS      ( 1000U * 60U )

/**
 * @brief CORE_MQTT_RX_BUFFER_SIZE
 */
#define CORE_MQTT_RX_BUFFER_SIZE         ( 1024U * 4U)

/**
 * @brief CORE_MQTT_TX_BUFFER_SIZE
 */
#define CORE_MQTT_TX_BUFFER_SIZE         ( 1024U * 4U )

/**
 * @brief client send MQTT connect packet, and wait connect ack from cloud server 
 */
#define MQTT_CONN_ACK_TIMEOUT_MS      ( 1000U * 5U )


#endif /* ifndef CORE_MQTT_CONFIG_H_ */
