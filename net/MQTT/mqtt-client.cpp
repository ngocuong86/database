//
// Created by tai1235 on 29/01/2019.
//

#include "mqtt-client.hpp"
#include <utility>
#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/Timer/SysTemTimer.hpp"
#include "Libraries/Utils/String.hpp"
#include "common.hpp"
bool MqttClient::m_isConnect = FALSE;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
MqttClient::OptionMqtt::OptionMqtt()
:_id("database_" + std::to_string(rand())),
    _host(""),
    _port(0),
    _keepalive(60),
    _enableSSL(FALSE),
    _user(""),
    _pwd("")
{
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
MqttClient::OptionMqtt::~OptionMqtt()
{
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
MqttClient::MqttClient() {
    LOG_INFO("MqttClient-client::MqttClient-client");
    mosquitto_lib_init();
    m_client = NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
MqttClient::~MqttClient()
{
    if (IsConnect()) {
        Disconnect();
    }
    mosquitto_destroy(m_client);
    mosquitto_lib_cleanup();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
MqttClient::CreateSession(
        MqttClient::OptionMqtt &optionMqttClient
){
    LOG_INFO("MqttClient-client::CreateSession");
    m_id = optionMqttClient._id;
    m_host = optionMqttClient._host;
    m_port = optionMqttClient._port;
    m_keepalive = optionMqttClient._keepalive;
    m_enableSSL = optionMqttClient._enableSSL;
    m_user = optionMqttClient._user;
    m_pwd = optionMqttClient._pwd;

    if (m_client != NULL) {
        mosquitto_reinitialise(m_client, m_id.c_str(), true, nullptr);
    } else {
        m_client = mosquitto_new(m_id.c_str(), true, nullptr);
    }

    if (!m_client) {
        LOG_ERRO("Could not create mosquitto client");
        return;
    }

    if (m_user != "" || m_pwd != "") {
        LOG_DBUG("Set user name pass %s, %s", m_user, m_pwd);
        mosquitto_username_pw_set(m_client, m_user, m_pwd);
        mosquitto_tls_insecure_set(m_client, TRUE);
    }
    LOG_CRIT("Pass username_password");
    if (m_enableSSL) {
        mosquitto_tls_opts_set(m_client, SSL_VERIFY_PEER, "tlsv1.2", nullptr);
        mosquitto_tls_set(m_client, CAFILE, nullptr, nullptr, nullptr, nullptr);
        mosquitto_tls_insecure_set(m_client, TRUE);
    }
    m_TimerFunctor = makeFunctor((TimerFunctor_t*) NULL, *this, &MqttClient::Reconnect);
    // Set callback
    mosquitto_user_data_set(m_client, this);
    mosquitto_reconnect_delay_set(m_client, 3, 12, TRUE);
    mosquitto_connect_callback_set(m_client, HandleConnect);
    mosquitto_disconnect_callback_set(m_client, HandleDisconnect);
    mosquitto_message_callback_set(m_client, HandleMessage);
    mosquitto_log_callback_set(m_client, HandleLog);
    SYSTIMER().StartTimer(SysTemTimer::FOREVER, 7, &m_TimerFunctor, NULL);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
MqttClient::CreateSession(
){
    LOG_INFO("MqttClient-client::CreateSession");
    m_id = "database_" + std::to_string(rand());
    m_host = DATABASE_MQTT_HOST;
    m_port = DATABASE_MQTT_PORT;
    m_keepalive = 60;
    m_enableSSL = FALSE;
    m_user = DATABASE_MQTT_USERNAME_DEFAULT;
    m_pwd = DATABASE_MQTT_PASSWORD_DEFAULT;

    if (m_client != NULL) {
        mosquitto_reinitialise(m_client, m_id.c_str(), true, nullptr);
    } else {
        m_client = mosquitto_new(m_id.c_str(), true, nullptr);
    }

    if (!m_client) {
        LOG_ERRO("Could not create mosquitto client");
        return;
    }

    if (m_user != "" || m_pwd != "") {
        LOG_DBUG("Set user name pass %s, %s", m_user, m_pwd);
        mosquitto_username_pw_set(m_client, m_user, m_pwd);
        mosquitto_tls_insecure_set(m_client, TRUE);
    }
    LOG_CRIT("Pass username_password");
    if (m_enableSSL) {
        mosquitto_tls_opts_set(m_client, SSL_VERIFY_PEER, "tlsv1.2", nullptr);
        mosquitto_tls_set(m_client, CAFILE, nullptr, nullptr, nullptr, nullptr);
        mosquitto_tls_insecure_set(m_client, TRUE);
    }
    m_TimerFunctor = makeFunctor((TimerFunctor_t*) NULL, *this, &MqttClient::Reconnect);
    // Set callback
    mosquitto_user_data_set(m_client, this);
    mosquitto_reconnect_delay_set(m_client, 3, 12, TRUE);
    mosquitto_connect_callback_set(m_client, HandleConnect);
    mosquitto_disconnect_callback_set(m_client, HandleDisconnect);
    mosquitto_message_callback_set(m_client, HandleMessage);
    mosquitto_log_callback_set(m_client, HandleLog);
    SYSTIMER().StartTimer(SysTemTimer::FOREVER, 7, &m_TimerFunctor, NULL);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
MqttClient::HandleConnect(
        struct mosquitto *client,
        void_p userdata, int rc
){
    LOG_INFO("ZbMqttClientClient::HandleConnect");
    if (rc == 0) {
        LOG_INFO("Connected to MqttClient broker local");
        MqttClient::m_isConnect = TRUE;
        bool_t subResult = Subscribe(client);
        if (subResult == TRUE) {
            static_cast<MqttClient*>(userdata)->ReceivedMessage(DATABASE_MQTT_SUBDONE, "status");
        }
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
MqttClient::HandleDisconnect(
        struct mosquitto *client,
        void_p userdata,
        int_t rc
){
    LOG_ERRO("Lost connect to Broker local...error code : %d", rc);
    MqttClient::m_isConnect = FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
MqttClient::Reconnect(
        void_p p_Buffer
){
    if (!m_isConnect) {
        mosquitto_reconnect(m_client);
        LOG_WARN("Reconnecting to Broker local.....");
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
MqttClient::HandleMessage(
        struct mosquitto *client,
        void_p userdata,
        const struct mosquitto_message *message
){
    LOG_CRIT("Recv: %s",String((char*)message->payload).c_str());
    static_cast<MqttClient*>(userdata)->ReceivedMessage(String((char*)message->payload), String((char*)message->topic));
}
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
MqttClient::HandleLog(
        struct mosquitto *client,
        void_p userdata,
        int level,
        const char *log
) {
//    LOG_DBUG("Log: %s", log);
}
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool
MqttClient::Connect(
) {
    LOG_INFO("MqttClient-client::Connect");
    LOG_CRIT("Into call connect host: %s port: %d, keepalive: %d", m_host.c_str(), m_port, m_keepalive);
    bool_t result = mosquitto_connect(m_client, m_host.c_str(), m_port, m_keepalive) == MOSQ_ERR_SUCCESS;
    LOG_CRIT("Result connect %d", result);
    if (result) {
        mosquitto_loop_forever(m_client, -1, 1);
    }
    return result;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool
MqttClient::Disconnect()
{
    bool result = mosquitto_disconnect(m_client) == MOSQ_ERR_SUCCESS;
    return result;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool
MqttClient::IsConnect() {
    return m_isConnect;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool
MqttClient::Publish(
        String message,
        String topic
) {
    String baseTopic = DATABASE_MQTT_TOPIC_REQ + topic;
    LOG_INFO("Database --> \n%s \nTopic %s", message.c_str(), baseTopic.c_str());
    return mosquitto_publish(m_client, nullptr, baseTopic.c_str(), strlen(message.c_str()), message.c_str(), 0, false) == MOSQ_ERR_SUCCESS;
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool
MqttClient::Subscribe(struct mosquitto* client) {

    if (m_isConnect) {
        bool_t result = mosquitto_subscribe(client, nullptr, DATABASE_MQTT_TOPIC_RES, 0) == MOSQ_ERR_SUCCESS;
        if (!result) {
            LOG_ERRO("Could not subscribe to topic: %s", DATABASE_MQTT_TOPIC_RES);
            return false;
        }
            LOG_CRIT("Could subscribe to topic: %s", DATABASE_MQTT_TOPIC_RES);
        return true;
    }
    return false;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SignalPayload_t&
MqttClient::StringReceived(
) {
	LOG_INFO("MqttClient-client::StringReceived");
    return m_signalPayload;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
MqttClient::ReceivedMessage(
        String payload,
        String topic
) {
    m_signalPayload.Emit(payload, topic);
}



