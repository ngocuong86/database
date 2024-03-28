//
// Created by cuong on 04/10/2023.
//

#ifndef DATABASE_DBUS_CLIENT_HPP
#define DATABASE_DBUS_CLIENT_HPP
#include <gio/gio.h>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"
#include "Libraries/Utils/Vector.hpp"
#include "Libraries/Utils/Map.hpp"
#include "Libraries/Signal/SignalF.hpp"
#include "Libraries/Utils/Signal.hpp"
#include <vector>

typedef Signal<void(const gchar *method_name,
                     GVariant *parameters,
                     GDBusMethodInvocation *invocation
					)> SignalMethodDbus_t;
class DbusClient {
private:
	static SignalMethodDbus_t m_signalDbus;
    GDBusNodeInfo *introspection_data;
	static void ReceiverMethodCall(GDBusConnection *connection,
	                               const gchar *sender,
	                               const gchar *object_path,
	                               const gchar *interface_name,
	                               const gchar *method_name,
	                               GVariant *parameters,
	                               GDBusMethodInvocation *invocation,
	                               gpointer user_data);
	static GVariant *HandleGetProperty(GDBusConnection *connection,
	                                   const gchar *sender,
	                                   const gchar *object_path,
	                                   const gchar *interface_name,
	                                   const gchar *property_name,
	                                   GError **error,
	                                   gpointer user_data);
	static gboolean HandleSetProperty(GDBusConnection *connection,
	                                  const gchar *sender,
	                                  const gchar *object_path,
	                                  const gchar *interface_name,
	                                  const gchar *property_name,
	                                  GVariant *value,
	                                  GError **error,
	                                  gpointer user_data);
	static void BusAcquired(GDBusConnection *connection,
	                        const gchar *name,
	                        gpointer user_data);
	static GDBusInterfaceVTable interface_vtable;
public:
    DbusClient();
    ~DbusClient();
	struct ParametersInfo{
		const gchar* arg_type;
		const gchar* arg_name;
		const gchar* arg_direction;
	};
	Vector<ParametersInfo> parameters_info_t;
	typedef struct MethodInfo{
		const gchar* method_name;
		Vector<ParametersInfo> parameters_info_v;
	}MethodInfo_t, *MethodInfo_p;
	Vector<MethodInfo_t> method_info_v;
	void Init();
	void Start();
    void Stop();
	static void NameAcquired(GDBusConnection *connection, const gchar *name, gpointer user_data);
	static void NameLost(GDBusConnection *connection, const gchar *name, gpointer user_data);
	SignalMethodDbus_t& DbusReceived();
};
typedef DbusClient DbusClient_t;
typedef DbusClient* DbusClient_p;
#endif //DATABASE_DBUS_CLIENT_HPP
