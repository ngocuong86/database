#include "dbus-client.hpp"
#include "Libraries/Utils/Vector.hpp"
#include "Libraries/Utils/Map.hpp"
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"
#include "Libraries/Signal/SignalF.hpp"
#include "Libraries/Utils/Signal.hpp"
#include "Libraries/Log/LogPlus.hpp"
#include <gio/gio.h>
#include "common.hpp"
//GDBusNodeInfo *introspection_data = NULL;
GDBusInterfaceVTable DbusClient::interface_vtable = {
		.method_call = DbusClient::ReceiverMethodCall,
		.get_property = DbusClient::HandleGetProperty,
		.set_property = DbusClient::HandleSetProperty
};
SignalMethodDbus_t DbusClient::m_signalDbus;
void DbusClient::ReceiverMethodCall(
	GDBusConnection *connection,
	const gchar *sender,
	const gchar *object_path,
	const gchar *interface_name,
	const gchar *method_name,
	GVariant *parameters,
	GDBusMethodInvocation *invocation,
	gpointer user_data
) {
	LOG_INFO("ReceiverMethodCall");
	m_signalDbus.Emit(method_name, parameters, invocation);
}
GVariant *DbusClient::HandleGetProperty(
    GDBusConnection *connection,
    const gchar *sender,
    const gchar *object_path,
    const gchar *interface_name,
    const gchar *property_name,
    GError **error,
    gpointer user_data
) {

}
gboolean DbusClient::HandleSetProperty(
    GDBusConnection *connection,
    const gchar *sender,
    const gchar *object_path,
    const gchar *interface_name,
    const gchar *property_name,
    GVariant *value,
    GError **error,
    gpointer user_data
) {

}

void DbusClient::BusAcquired(GDBusConnection *connection, const gchar *name, gpointer user_data) {
	LOG_INFO("Dbus BusAcquired");
    GError *error = nullptr;
    auto self = (DbusClient *)user_data;
	guint registration_id;
	registration_id = g_dbus_connection_register_object(connection,
	                                                    "/",
	                                                    self->introspection_data->interfaces[0],
	                                                    &self->interface_vtable,
	                                                    self,
	                                                    NULL,
	                                                    &error);
	if (registration_id == 0) {
		std::cerr << "Error registering object: " << error->message << std::endl;
        return ;
	}
}
//void DbusClient::HandlerBusAcquired(
//    GDBusConnection *connection,
//    const gchar *name,
//    gpointer user_data,
//    const GDBusInterfaceVTable
//) {
//	LOG_INFO("Dbus BusAcquiredHandler");
//}
DbusClient::DbusClient() {

}
DbusClient::~DbusClient() {

}
void DbusClient::Init(
) {
//	xmlInitParser();
//	xmlDocPtr doc = xmlReadFile(DATABASE_DBUS_PATH_XML, NULL, 0);
//	if (doc == NULL) {
//		fprintf(stderr, "Error Read File XML.\n");
//		return ;
//	}
//	xmlChar* introspection_xml;
//	int introspection_size;
//	xmlDocDumpMemory(doc, &introspection_xml, &introspection_size);
//	xmlFreeDoc(doc);
//	xmlCleanupParser();
//	printf("XML:\n%s\n", (const char*)introspection_xml);
//	GError* error = nullptr;
//	introspection_data = g_dbus_node_info_new_for_xml((const char*)introspection_xml, &error);
//	if (error != nullptr) {
//		std::cerr << "Error create introspection data: " << error->message << std::endl;
//		g_error_free(error);
//	}
//	xmlFree(introspection_xml);
    const gchar introspection_xml[] =
            "<node>"
            "<interface name='lumi.database.interface'>"
            "<method name='DeleteRecord'>"
            "<arg type='s'           name='nameDB'           direction='in'/>"
            "<arg type='s'           name='nameTable'        direction='in'/>"
            "<arg type='s'           name='key'           	direction='in'/>"
            "<arg type='b'           name='status'           direction='out'/>"
            "</method>"
            "<method name='AddRecord'>"
            "<arg type='s'           name='nameDB'           direction='in'/>"
            "<arg type='s'           name='nameTable'        direction='in'/>"
            "<arg type='s'           name='key'           	direction='in'/>"
            "<arg type='s'           name='value'           	direction='in'/>"
            "<arg type='b'           name='status'           direction='out'/>"
            "</method>"
            "<method name='GetRecord'>"
            "<arg type='s'           name='nameDB'           direction='in'/>"
            "<arg type='s'           name='nameTable'        direction='in'/>"
            "<arg type='s'           name='key'           	direction='in'/>"
            "<arg type='s'           name='value'           	direction='out'/>"
            "</method>"
            "<method name='DeleteMultiRecords'>"
            "<arg type='s'           name='nameDB'           direction='in'/>"
            "<arg type='s'           name='nameTable'        direction='in'/>"
            "<arg type='as'          name='key'           	direction='in'/>"
            "<arg type='a{sb}'       name='status'           direction='out'/>"
            "</method>"
            "<method name='GetMultiRecords'>"
            "<arg type='s'           name='nameDB'           direction='in'/>"
            "<arg type='s'           name='nameTable'        direction='in'/>"
            "<arg type='as'          name='key'              direction='in'/>"
            "<arg type='a{ss}'       name='key-value'        direction='out'/>"
            "</method>"
            "<method name='AddMultiRecords'>"
            "<arg type='s'           name='nameDB'           direction='in'/>"
            "<arg type='s'           name='nameTable'        direction='in'/>"
            "<arg type='a{ss}'       name='key-value'        direction='in'/>"
            "<arg type='a{sb}'       name='status'           direction='out'/>"
            "</method>"
            "<method name='GetAllRecords'>"
            "<arg type='s'           name='nameDB'           direction='in'/>"
            "<arg type='s'           name='nameTable'        direction='in'/>"
            "<arg type='a{ss}'       name='key-value'        direction='out'/>"
            "</method>"
            "<method name='DeleteAllRecord'>"
            "<arg type='s'           name='nameDB'           direction='in'/>"
            "<arg type='s'           name='nameTable'        direction='in'/>"
            "<arg type='b'           name='status'           direction='out'/>"
            "</method>"
            "<method name='SearchInValue'>"
            "<arg type='s'           name='nameDB'           direction='in'/>"
            "<arg type='s'           name='nameTable'        direction='in'/>"
            "<arg type='a{ss}'       name='key-field'        direction='in'/>"
            "<arg type='a{ss}'       name='key-value'        direction='out'/>"
            "</method>"
            "<method name='BackupDataBase'>"
            "<arg type='s'           name='nameDB'           direction='in'/>"
            "<arg type='b'           name='status'           direction='out'/>"
            "</method>"
            "<method name='RecoverFromBackup'>"
            "<arg type='s'           name='nameDB'           direction='in'/>"
            "<arg type='b'           name='status'           direction='out'/>"
            "</method>"
            "</interface>"
            "</node>"
    ;
    printf("XML:\n%s\n", (const char*)introspection_xml);
	GError* error = nullptr;
	introspection_data = g_dbus_node_info_new_for_xml((const char*)introspection_xml, &error);
	if (error != nullptr) {
		std::cerr << "Error create introspection data: " << error->message << std::endl;
		g_error_free(error);
	}
}
void DbusClient::NameAcquired(GDBusConnection *connection, const gchar *name, gpointer user_data) {
    std::cerr << "Name Acquired" << std::endl;
}
void DbusClient::NameLost(GDBusConnection *connection, const gchar *name, gpointer user_data) {
    std::cerr << "Name Lost" << std::endl;
}
void DbusClient::Start() {
	LOG_INFO("Dbus start");
	guint watcher_id;
	GMainLoop *loop;
	watcher_id = g_bus_own_name(G_BUS_TYPE_SYSTEM, DBUS_NAME,
	                            G_BUS_NAME_OWNER_FLAGS_NONE, BusAcquired,
	                            NameAcquired, NameLost, this, NULL);
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
}
void DbusClient::Stop() {

}
SignalMethodDbus_t& DbusClient::DbusReceived() {
	return m_signalDbus;
}
