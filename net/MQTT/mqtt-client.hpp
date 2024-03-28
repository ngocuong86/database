/*******************************************************************************
  * @ Author: CuongNV
  * @ Create Time: 2023-10-03 08:39:05
  * @ Modified by: CuongNV
  * @ Modified time: 2023-10-03 08:56:10
  * @ Description:
  * @ Email:: cuongngo0806@gmail.com
  ******************************************************************************/

#ifndef __MQTT_CLIENT_HPP__
#define __MQTT_CLIENT_HPP__

#include "openssl/ssl.h"
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Core.hpp"
#include "Libraries/Thread/Thread.hpp"
#include "Libraries/Signal/SignalF.hpp"
#include "Libraries/Utils/Signal.hpp"
#include "Libraries/Functors/Functor.hpp"
#include "Libraries/Utils/BlockingQueue.hpp"
extern "C" {
#include <mosquitto.h>
};
#ifdef MT7688
#define CAFILE "/tmp/keys/LuciRootCA.pem"
#else
#define CAFILE "./LuciRootCA.pem"
#endif
typedef Functor1_t(void_p)      TimerFunctor_t;
typedef Signal<void(std::string&, std::string&)> SignalPayload_t;
class MqttClient {

private:
    String  m_id;
    String  m_host;
    int_t   m_port;
    int_t   m_keepalive;
    bool    m_enableSSL;
    const char* m_user;
    const char* m_pwd;
    TimerFunctor_t      m_TimerFunctor;
    struct mosquitto*   m_client;
    // Signal and Flag
    SignalPayload_t m_signalPayload;
    static bool m_isConnect;
    // Callback
    static void HandleConnect(struct mosquitto* client, void_p userdata, int rc);
    static void HandleDisconnect(struct mosquitto* client, void_p userdata, int_t rc);
    static void HandleMessage(struct mosquitto* client, void_p userdata, const struct mosquitto_message* message);
    static void HandleLog(struct mosquitto* client, void_p userdata, int level, const char * log);
    static bool Subscribe(struct mosquitto* client);
    void ReceivedMessage (std::string payload, std::string topic);

public:
    class OptionMqtt {
    public:
        std::string  _id;
        std::string  _host;
        int          _port;
        int     _keepalive;
        bool    _enableSSL;
        const char* _user;
        const char* _pwd;
        OptionMqtt();
        ~OptionMqtt();
    };

    MqttClient();
    ~MqttClient();
    void CreateSession(MqttClient::OptionMqtt& optionMqtt);
    void CreateSession();
    void Reconnect(void_p p_Buffer);
    bool Connect();
    bool Disconnect();
    bool IsConnect();
    bool Publish(std::string message, std::string topic);
    SignalPayload_t& StringReceived();
};
typedef MqttClient MqttClient_t;
typedef MqttClient* MqttClient_p;
#endif // __MQTT_CLIENT_HPP__