//
// Created by cuong on 27/10/2023.
//
#include "local_session.hpp"
#include "Libraries/Log/LogPlus.hpp"
#include <gio/gio.h>
#include "Libraries/Utils/json.hpp"
using json = nlohmann::json;
LocalSession::LocalSession():
func1RLocalSession_(makeFunctor((Func1R<void_p, void_p> *) NULL, *this, &LocalSession::ConnectLocalSession))
{
	threadLocalSession_.AddFunctor(func1RLocalSession_);
	threadLocalSession_.Start();
}
LocalSession::~LocalSession() {
}
void LocalSession::Start() {
	controller.Start();
}
void *LocalSession::ConnectLocalSession(void *buffer) {
	controller.ControllerSignal() += Slot(this, &LocalSession::ReceivedFromLocal);
}
void LocalSession::ReceivedFromLocal(Vector<int> status, Vector<Record> list_record, DataBaseCommand database_command) {
	LOG_ERRO("ReceivedFromLocal");
	if(database_command.type_connect == DATABASE_TYPE_CONNECT_DBUS){
		if(database_command.flag == DBUS_METHOD_ADD_RECORD
		|| database_command.flag == DBUS_METHOD_DELETE_RECORD
		|| database_command.flag == DBUS_METHOD_DELETE_ALL_RECORDS ){
			g_dbus_method_invocation_return_value(database_command.invocation,
												  g_variant_new("(b)", status[0] == 0 ? TRUE : FALSE));
		}else if(database_command.flag == DBUS_METHOD_ADD_MULTI_RECORDS
		      || database_command.flag == DBUS_METHOD_DELETE_MULTI_RECORDS) {
			GVariantBuilder resultKeyValueBuilder;
			bool isFalse = FALSE;
			g_variant_builder_init(&resultKeyValueBuilder, G_VARIANT_TYPE("a{sb}"));
			for (int i = 0; i < status.size(); ++i) {
				if(status[i] != 0){
					g_variant_builder_add(&resultKeyValueBuilder, "{sb}", list_record[i].key.c_str(),  FALSE);
					isFalse = TRUE;
				}
			}
			if(isFalse == FALSE){
				String allKey = "all_key";
				g_variant_builder_add(&resultKeyValueBuilder, "{sb}",allKey.c_str(),  TRUE);
			}
			GVariant *resultKeyValueSuccess = g_variant_builder_end(&resultKeyValueBuilder);
			g_dbus_method_invocation_return_value(database_command.invocation, g_variant_new("(@a{sb})", resultKeyValueSuccess));
		}else if(database_command.flag == DBUS_METHOD_GET_RECORD) {
			g_dbus_method_invocation_return_value(database_command.invocation, g_variant_new("(s)", list_record[0].value.c_str()));
		}else if(database_command.flag == DBUS_METHOD_GET_MULTI_RECORDS
		      || database_command.flag == DBUS_METHOD_GET_ALL_RECORDS) {
			GVariantBuilder resultKeyValueBuilder;
			g_variant_builder_init(&resultKeyValueBuilder, G_VARIANT_TYPE("a{ss}"));
			for (int i = 0; i < list_record.size(); ++i) {
				if(list_record[i].value != ""){
					g_variant_builder_add(&resultKeyValueBuilder, "{ss}", list_record[i].key.c_str(), list_record[i].value.c_str());
				}
			}
			GVariant *resultKeyValueSuccess = g_variant_builder_end(&resultKeyValueBuilder);
			g_dbus_method_invocation_return_value(database_command.invocation, g_variant_new("(@a{ss})", resultKeyValueSuccess));
		}else if(database_command.flag == DBUS_METHOD_SEARCH_IN_VALUE) {
			GVariantBuilder resultKeyValueBuilder;
			g_variant_builder_init(&resultKeyValueBuilder, G_VARIANT_TYPE("a{ss}"));
			for (int i = 0; i < list_record.size(); i++) {
					json object_value = json::parse(list_record[i].value);
					if(object_value.contains(database_command.list_field[i])){
						g_variant_builder_add(&resultKeyValueBuilder, "{ss}", list_record[i].key.c_str(),
											  object_value[database_command.list_field[i]].dump().c_str());
					}
			}
			GVariant *resultKeyValueSuccess = g_variant_builder_end(&resultKeyValueBuilder);
			g_dbus_method_invocation_return_value(database_command.invocation, g_variant_new("(@a{ss})", resultKeyValueSuccess));
		}else{
			LOG_ERRO("ReceivedFromLocal: flag not found");
		}
	}
	LOG_CRIT("==========================================================================================================");
}
void LocalSession::Process() {
	controller.Process();
}