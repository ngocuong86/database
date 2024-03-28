//
// Created by cuong on 03/10/2023.
//

#ifndef DATABASE_SESSION_HPP
#define DATABASE_SESSION_HPP
#include "net/MQTT/mqtt-client.hpp"
#include "net/Dbus/dbus-client.hpp"
#include <gio/gio.h>
#include "database/database.h"
#include "database/database-command.hpp"

typedef Signal<void(DataBaseCommand
)> SignalSessionToController_t;
class SessClient {
private:
    MqttClient                mqttClient_;
    MqttClient::OptionMqtt    optionMqtt_;
    DbusClient                dbusClient_;
    Func1R<void*, void*>        func1RStartConnectMqtt_;
    Func1R<void*, void*>        func1RStartConnectDbus_;
    Thread      threadStartConnectMqtt_;
    Thread      threadStartConnectDbus_;
	bool isChangeData = false;
	static SignalSessionToController_t signal_session;
public:

    SessClient();
    ~SessClient();
    static SessClient& Instance();
    void Init();
	struct MethodOption{
		std::string method_name;
		std::string format;
		std::string type;
	};
    void ReceivedFromLocal(std::string& payload, std::string& topic);
    void* ConnectToLocalMqtt( void *buffer);
    void* ConnectDbus( void *buffer);
	void RecoverDataFromFlash();
    void ReceivedRequestDbus(const gchar *method_name,
                             GVariant *parameters,
                             GDBusMethodInvocation *invocation
                             );
	SignalSessionToController_t& SessionSignal();
//	void HandlerAddRecord(GVariant *parameters,
//						  GDBusMethodInvocation *invocation
//						  );
//	void HandlerAddMultiRecords(GVariant *parameters,
//								GDBusMethodInvocation *invocation
//								);
//
//	void HandlerDeleteRecord(GVariant *parameters,
//							 GDBusMethodInvocation *invocation
//							 );
//	void HandlerDeleteMultiRecords(GVariant *parameters,
//								   GDBusMethodInvocation *invocation
//								   );
//	void HanglerDeleteAllRecords(GVariant *parameters,
//								 GDBusMethodInvocation *invocation
//								 );
//	void HandlerGetRecord(GVariant *parameters,
//						  GDBusMethodInvocation *invocation
//						  );
//	void HandlerGetMultiRecords(GVariant *parameters,
//								GDBusMethodInvocation *invocation
//								);
//	void HandlerBackupDatabase(GVariant *parameters,
//							   GDBusMethodInvocation *invocation
//							   );
//	void HandlerRecoveredDatabase(GVariant *parameters,
//								  GDBusMethodInvocation *invocation
//								  );
//	void HandlerGetAllRecords(GVariant *parameters,
//							  GDBusMethodInvocation *invocation);
//	void HandlerSearchInValue(GVariant *parameters,
//							  GDBusMethodInvocation *invocation);
//	void OpenDataBase(gchar *nameDB, gchar *nameTable);
//	void HandlerUpdateRecord(GVariant *parameters,
//							 GDBusMethodInvocation *invocation);
//	void HandlerUpdateMultiRecords(GVariant *parameters,
//								   GDBusMethodInvocation *invocation);

};

typedef SessClient  SessClient_t;
typedef SessClient* SessClient_p;
#endif //DATABASE_SESSION_HPP
