//
// Created by cuong on 03/10/2023.
//
#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Functors/Functor.hpp"
#include "Libraries/Thread/Thread.hpp"
#include "Libraries/Utils/String.hpp"
#include "common.hpp"
#include "database/database.h"

#include <chrono>
#include <thread>
#include <filesystem>
#include "Libraries/Utils/json.hpp"
#include "session.hpp"
#include "database/record.hpp"
using json = nlohmann::json;
int count_rec = 0;
SignalSessionToController_t SessClient::signal_session;

String ConvertGchartoString(gchar* value){
	char* ch = (char*)value;
	return String(ch);
}
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SessClient::SessClient(
) : optionMqtt_(),
    mqttClient_(),
    dbusClient_(),
    threadStartConnectMqtt_(),
	threadStartConnectDbus_(),
    func1RStartConnectMqtt_(makeFunctor((Func1R<void_p, void_p>*)NULL, *this, &SessClient::ConnectToLocalMqtt)),
    func1RStartConnectDbus_(makeFunctor((Func1R<void_p, void_p>*)NULL, *this, &SessClient::ConnectDbus))
	{
	RecoverDataFromFlash();
    threadStartConnectMqtt_.AddFunctor(func1RStartConnectMqtt_);
    threadStartConnectDbus_.AddFunctor(func1RStartConnectDbus_);
	bool_t boRet = TRUE;
	boRet &= threadStartConnectMqtt_.Start();
	boRet &= threadStartConnectDbus_.Start();
    if(!boRet){
		LOG_ERRO("SessClient::SessClient threadStartConnectMqtt_ or threadStartConnectDbus_ fail");
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SessClient::~SessClient() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SessClient&
SessClient::Instance() {
    static SessClient object;
    return object;
}

///**
// * @func
// * @brief  None
// * @param  None
// * @retval None
// */
//void_p SessClient::BackupDataToFlash(void *buffer) {
//	const std::chrono::seconds interval(30); //time sleep
//	while(TRUE){
//		if(isChangeData){
//			std::string command = std::string("tar -czf ") + DATABASE_DBUS_PATH_BACKUP + "database" + ".tar.gz" +" -C"+ "/tmp/ database";
//			int exit_code = std::system(command.c_str());
//			if (exit_code == 0) {
//				cout << "Compression successful." << std::endl;
//			} else {
//				cerr << "Compression failed with exit code " << exit_code << ": " << strerror(errno) << std::endl;
//			}
//			isChangeData = FALSE;
//		}
//		std::this_thread::sleep_for(interval);
//	}
//	return nullptr;
//}
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void SessClient::RecoverDataFromFlash() {
//	if(std::filesystem::exists(DATABASE_DBUS_PATH_BACKUP + String("database") + String(".tar.gz"))){
//		std::string command = std::string("tar -xzf ") + DATABASE_DBUS_PATH_BACKUP + "database" + ".tar.gz" +" -C"+ "/tmp/";
//		int exit_code = std::system(command.c_str());
//		if (exit_code == 0) {
//			cout << "Extract successful." << std::endl;
//		} else {
//			cerr << "Extract failed with exit code " << exit_code << ": " << strerror(errno) << std::endl;
//		}
//	}
	return ;
}
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void*
SessClient::ConnectToLocalMqtt( void *buffer) {
    LOG_INFO("===========================SESSCLIENT CONNECT TO LOCAL MQTT================================");
    mqttClient_.CreateSession();
    mqttClient_.Connect();
    mqttClient_.StringReceived() += Slot(this, &SessClient::ReceivedFromLocal);
	return nullptr;
}
void*
SessClient::ConnectDbus( void *buffer ){
	LOG_INFO("===========================SESSCLIENT CREATE DBUS================================");
	dbusClient_.Init();
	dbusClient_.DbusReceived() += Slot(this, &SessClient::ReceivedRequestDbus);
	dbusClient_.Start();
	return nullptr;
}
//**********************RECEIVER DBUS*********************************************************
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void SessClient::ReceivedRequestDbus(const gchar *method_name,
                                     GVariant *parameters,
                                     GDBusMethodInvocation *invocation
) {
	LOG_CRIT("======================================= %d =====================================================", ++count_rec);
	LOG_ERRO("ReceivedRequestDbus");
	//LOG_INFO("%s <-- :%s", method_name, g_variant_print(parameters, TRUE));
	DataBaseCommand commandRequest;
	commandRequest.type_connect = DATABASE_TYPE_CONNECT_DBUS;
	if(g_strcmp0(method_name, DBUS_METHOD_ADD_RECORD) == 0){
		gchar *nameDB, *nameTable, *key, *value;
		g_variant_get(parameters, "(ssss)", &nameDB,&nameTable, &key, &value);
		Record record(ConvertGchartoString(key) , ConvertGchartoString(value));
		commandRequest.name_table = ConvertGchartoString(nameTable);
		commandRequest.list_record.push_back(record);
		commandRequest.invocation = invocation;
		commandRequest.name_db = ConvertGchartoString(nameDB);
		commandRequest.flag = DBUS_METHOD_ADD_RECORD;
	}else if(g_strcmp0(method_name, DBUS_METHOD_ADD_MULTI_RECORDS) == 0) {
		gchar *nameDB, *nameTable;
		GVariant *keyValueArray;
		g_variant_get(parameters, "(ss@a{ss})",&nameDB, &nameTable, &keyValueArray);
		commandRequest.name_table = ConvertGchartoString(nameTable);
		commandRequest.invocation = invocation;
		gsize length = g_variant_n_children(keyValueArray);
		gchar **key = g_new0(gchar *, length + 1);
		gchar **value = g_new0(gchar *, length + 1);
		for (gsize i = 0; i < length; i++) {
			GVariant *keyValue = g_variant_get_child_value(keyValueArray, i);
			g_variant_get(keyValue, "{ss}", &key[i], &value[i]);
		}
		for (int j = 0; key[j] != NULL; j++) {
			Record record(ConvertGchartoString(key[j]) , ConvertGchartoString(value[j]));
			commandRequest.list_record.push_back(record);
		}
		commandRequest.name_db = ConvertGchartoString(nameDB);
		commandRequest.flag = DBUS_METHOD_ADD_MULTI_RECORDS;
		g_strfreev(key);
		g_strfreev(value);
	}else if(g_strcmp0(method_name, DBUS_METHOD_DELETE_RECORD) == 0) {
		gchar *nameDB, *nameTable, *key;
		g_variant_get(parameters, "(sss)", &nameDB, &nameTable, &key);
		commandRequest.name_db = ConvertGchartoString(nameDB);
		commandRequest.name_table = ConvertGchartoString(nameTable);
		commandRequest.invocation = invocation;
		commandRequest.list_record.push_back(Record(ConvertGchartoString(key), ""));
		commandRequest.flag = DBUS_METHOD_DELETE_RECORD;
	}else if(g_strcmp0(method_name, DBUS_METHOD_DELETE_MULTI_RECORDS) == 0) {
		gchar *nameDB, *nameTable, **key;
		g_variant_get(parameters, "(ss^as)", &nameDB, &nameTable, &key);
		commandRequest.name_table = ConvertGchartoString(nameTable);
		commandRequest.invocation = invocation;
		Vector<String> keys;
		for (int j = 0; key[j] != NULL; j++) {
			commandRequest.list_record.push_back(Record(ConvertGchartoString(key[j]), ""));
		}
		commandRequest.name_db = ConvertGchartoString(nameDB);
		commandRequest.flag = DBUS_METHOD_DELETE_MULTI_RECORDS;
	}else if(g_strcmp0(method_name, DBUS_METHOD_DELETE_ALL_RECORDS) == 0) {
		gchar *nameDB, *nameTable;
		g_variant_get(parameters, "(ss)", &nameDB, &nameTable);
		commandRequest.name_table = ConvertGchartoString(nameTable);
		commandRequest.invocation = invocation;
		commandRequest.name_db = ConvertGchartoString(nameDB);
		commandRequest.flag = DBUS_METHOD_DELETE_ALL_RECORDS;
	}else if(g_strcmp0(method_name, DBUS_METHOD_GET_RECORD) == 0) {
		gchar *nameDB, *nameTable, *key;
		g_variant_get(parameters, "(sss)", &nameDB, &nameTable, &key);
		commandRequest.invocation = invocation;
		commandRequest.name_table = ConvertGchartoString(nameTable);
		commandRequest.list_record.push_back(Record(ConvertGchartoString(key), ""));
		commandRequest.name_db = ConvertGchartoString(nameDB);
		commandRequest.flag = DBUS_METHOD_GET_RECORD;
	}else if(g_strcmp0(method_name, DBUS_METHOD_GET_MULTI_RECORDS) == 0) {
		gchar *nameDB, *nameTable, **key;
		g_variant_get(parameters, "(ss^as)", &nameDB, &nameTable, &key);
		commandRequest.invocation = invocation;
		commandRequest.name_table = ConvertGchartoString(nameTable);
		for (int j = 0; key[j] != NULL; j++) {
			commandRequest.list_record.push_back(Record(ConvertGchartoString(key[j]), ""));
		}
		commandRequest.name_db = ConvertGchartoString(nameDB);
		commandRequest.flag = DBUS_METHOD_GET_MULTI_RECORDS;
	}else if(g_strcmp0(method_name, DBUS_METHOD_BACKUP_DATABASE) == 0) {
		gchar *nameDB, *nameTable;
		g_variant_get(parameters, "(s)", &nameDB);
		commandRequest.invocation = invocation;
		commandRequest.name_db = ConvertGchartoString(nameDB);
		commandRequest.flag = DBUS_METHOD_BACKUP_DATABASE;
	}else if(g_strcmp0(method_name, DBUS_METHOD_RECOVER_FROM_BACKUP) == 0) {
		gchar *nameDB, *nameTable;
		g_variant_get(parameters, "(ss)", &nameDB, &nameTable);
		commandRequest.invocation = invocation;
		commandRequest.name_table = ConvertGchartoString(nameTable);
		commandRequest.name_db = ConvertGchartoString(nameDB);
		commandRequest.flag = DBUS_METHOD_RECOVER_FROM_BACKUP;
	}else if(g_strcmp0(method_name, DBUS_METHOD_GET_ALL_RECORDS) == 0) {
		gchar *nameDB, *nameTable;
		g_variant_get(parameters, "(ss)", &nameDB, &nameTable);
		commandRequest.invocation = invocation;
		commandRequest.name_table = ConvertGchartoString(nameTable);
		commandRequest.name_db = ConvertGchartoString(nameDB);
		commandRequest.flag = DBUS_METHOD_GET_ALL_RECORDS;
	}else if(g_strcmp0(method_name, DBUS_METHOD_SEARCH_IN_VALUE) == 0) {
		gchar *nameDB, *nameTable;
		GVariant *keyValueArray;
		g_variant_get(parameters, "(ss@a{ss})",&nameDB, &nameTable, &keyValueArray);
		gsize length = g_variant_n_children(keyValueArray);
		gchar **key = g_new0(gchar *, length + 1);
		gchar **field = g_new0(gchar *, length + 1);
		for (gsize i = 0; i < length; i++) {
			GVariant *keyValue = g_variant_get_child_value(keyValueArray, i);
			g_variant_get(keyValue, "{ss}", &key[i], &field[i]);
			commandRequest.list_record.push_back(Record(ConvertGchartoString(key[i]), ""));
			commandRequest.list_field.push_back(ConvertGchartoString(field[i]));
		}
		commandRequest.invocation = invocation;
		commandRequest.name_table = ConvertGchartoString(nameTable);
		commandRequest.name_db = ConvertGchartoString(nameDB);
		commandRequest.flag = DBUS_METHOD_SEARCH_IN_VALUE;
		g_strfreev(key);
		g_strfreev(field);
	}else{
		LOG_ERRO("Method name not found");
		return;
	}
	signal_session.Emit(commandRequest);
//	return ;
}
//***********************RECEIVER MQTT********************************************************
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
SessClient::ReceivedFromLocal(
        std::string& payload,
        std::string& topic
) {
    LOG_INFO("Database <-- %s \nTopic %s", payload.c_str(),topic.c_str());
	return;
}
SignalSessionToController_t& SessClient::SessionSignal(){
	return signal_session;
}
///===============================HANDLER====================================================
//void SessClient::HandlerAddRecord(GVariant *parameters,
//                      GDBusMethodInvocation *invocation
//){
//	gchar *nameDB, *nameTable, *key, *value;
//	g_variant_get(parameters, "(ssss)", &nameDB,&nameTable, &key, &value);
//	OpenDataBase(nameDB, nameTable);
//	DatabaseManager& db_manager = DatabaseManager::GetInstance();
//	Record record(ConvertGchartoString(key) , ConvertGchartoString(value));
//
//	int status = db_manager.AddRecord(record);
//	db_manager.IterateRecords(Print);
//	g_dbus_method_invocation_return_value(invocation, g_variant_new("(b)", status == 0 ? TRUE : FALSE));
//	db_manager.CloseDatabase();
//	isChangeData = TRUE;
//	return;
//}
//void SessClient::HandlerAddMultiRecords(GVariant *parameters,
//                            GDBusMethodInvocation *invocation
//){
//	gchar *nameDB, *nameTable;
//	GVariant *keyValueArray;
//	g_variant_get(parameters, "(ss@a{ss})",&nameDB, &nameTable, &keyValueArray);
//	OpenDataBase(nameDB, nameTable);
//
//	DatabaseManager& db_manager = DatabaseManager::GetInstance();
//	gsize length = g_variant_n_children(keyValueArray);
//	gchar **key = g_new0(gchar *, length + 1);
//	gchar **value = g_new0(gchar *, length + 1);
//	for (gsize i = 0; i < length; i++) {
//		GVariant *keyValue = g_variant_get_child_value(keyValueArray, i);
//		g_variant_get(keyValue, "{ss}", &key[i], &value[i]);
//	}
//	Vector<Record> records;
//	for (int j = 0; key[j] != NULL; j++) {
//		Record record(ConvertGchartoString(key[j]) , ConvertGchartoString(value[j]));
//		records.push_back(record);
//	}
//	Vector<int> status;
//	status = db_manager.AddMultiRecords(records);
//	GVariantBuilder resultKeyValueBuilder;
//	bool isFalse = FALSE;
//	g_variant_builder_init(&resultKeyValueBuilder, G_VARIANT_TYPE("a{sb}"));
//	for (int i = 0; i < records.size(); i++) {
//		if(status[i] != 0){
//			g_variant_builder_add(&resultKeyValueBuilder, "{sb}", records[i].key.c_str(),  FALSE);
//			isFalse = TRUE;
//		}
//	}
//	if(isFalse == FALSE){
//		LOG_INFO("all key success");
//		String allKey = "all_key";
//		g_variant_builder_add(&resultKeyValueBuilder, "{sb}",allKey.c_str(),  TRUE);
//	}
//	GVariant *resultKeyValueSuccess = g_variant_builder_end(&resultKeyValueBuilder);
//	db_manager.IterateRecords(Print);
//	g_dbus_method_invocation_return_value(invocation, g_variant_new("(@a{sb})", resultKeyValueSuccess));
//	g_strfreev(key);
//	g_strfreev(value);
//	db_manager.CloseDatabase();
//	isChangeData = TRUE;
//	return;
//}
//
//void SessClient::HandlerDeleteRecord(GVariant *parameters,
//                         GDBusMethodInvocation *invocation
//){
//	gchar *nameDB, *nameTable, *key;
//	g_variant_get(parameters, "(sss)", &nameDB, &nameTable, &key);
//	OpenDataBase(nameDB, nameTable);
//
//	DatabaseManager& db_manager = DatabaseManager::GetInstance();
//	int status =  db_manager.DeleteRecord(ConvertGchartoString(key));
//	db_manager.IterateRecords(Print);
//	g_dbus_method_invocation_return_value(invocation, g_variant_new("(b)", status == 0 ? TRUE : FALSE));
//	db_manager.CloseDatabase();
//	isChangeData = TRUE;
//	return;
//
//}
//void SessClient::HandlerDeleteMultiRecords(GVariant *parameters,
//                               GDBusMethodInvocation *invocation
//){
//	gchar *nameDB, *nameTable, **key;
//	g_variant_get(parameters, "(ss^as)", &nameDB, &nameTable, &key);
//	OpenDataBase(nameDB, nameTable);
//
//	DatabaseManager& db_manager = DatabaseManager::GetInstance();
//	Vector<String> keys;
//	for (int j = 0; key[j] != NULL; j++) {
//		keys.push_back(ConvertGchartoString(key[j]));
//	}
//	vector<int> status =  db_manager.DeleteMultiRecords(keys);
//	GVariantBuilder resultKeyValueBuilder;
//	g_variant_builder_init(&resultKeyValueBuilder, G_VARIANT_TYPE("a{sb}"));
//	for (int i = 0; i < keys.size(); i++) {
//		if(status[i] != 0)
//		{
//			g_variant_builder_add(&resultKeyValueBuilder, "{sb}", keys[i].c_str(), FALSE);
//		}
//	}
//	GVariant *resultKeyValueSuccess = g_variant_builder_end(&resultKeyValueBuilder);
//	if(!g_variant_get_size(resultKeyValueSuccess)){
//		g_variant_builder_add(&resultKeyValueBuilder, "{sb}", "all_key",  TRUE);
//	}
//	db_manager.IterateRecords(Print);
//	g_dbus_method_invocation_return_value(invocation, g_variant_new("(@a{sb})", resultKeyValueSuccess));
//	db_manager.CloseDatabase();
//	isChangeData = TRUE;
//	return;
//}
//void SessClient::HanglerDeleteAllRecords(GVariant *parameters,
//                             GDBusMethodInvocation *invocation
//){
//	gchar *nameDB, *nameTable;
//	g_variant_get(parameters, "(ss)", &nameDB, &nameTable);
//	OpenDataBase(nameDB, nameTable);
//
//	DatabaseManager& db_manager = DatabaseManager::GetInstance();
//	int status = db_manager.DeleteAllRecords();
//	db_manager.IterateRecords(Print);
//	g_dbus_method_invocation_return_value(invocation, g_variant_new("(b)", status == 0? TRUE:FALSE));
//	db_manager.CloseDatabase();
//	isChangeData = TRUE;
//	return;
//}
//void SessClient::HandlerGetRecord(GVariant *parameters,
//                      GDBusMethodInvocation *invocation
//){
//	gchar *nameDB, *nameTable, *key;
//	g_variant_get(parameters, "(sss)", &nameDB, &nameTable, &key);
//	OpenDataBase(nameDB, nameTable);
//
//	DatabaseManager& db_manager = DatabaseManager::GetInstance();
//	Record result = db_manager.GetRecord(ConvertGchartoString(key));
//	db_manager.IterateRecords(Print);
//	g_dbus_method_invocation_return_value(invocation, g_variant_new("(s)", result.value.c_str()));
//	db_manager.CloseDatabase();
//	return;
//}
//void SessClient::HandlerGetMultiRecords(GVariant *parameters,
//                            GDBusMethodInvocation *invocation
//){
//	gchar *nameDB, *nameTable, **key;
//	g_variant_get(parameters, "(ss^as)", &nameDB, &nameTable, &key);
//	OpenDataBase(nameDB, nameTable);
//
//	DatabaseManager& db_manager = DatabaseManager::GetInstance();
//	Vector<String> keys;
//	for (int j = 0; key[j] != NULL; j++) {
//		keys.push_back(ConvertGchartoString(key[j]));
//	}
//	Vector<Record> result = db_manager.GetMultiRecords(keys);
//	GVariantBuilder resultKeyValueBuilder;
//	g_variant_builder_init(&resultKeyValueBuilder, G_VARIANT_TYPE("a{ss}"));
//	for (int i = 0; i < result.size(); i++) {
//		g_variant_builder_add(&resultKeyValueBuilder, "{ss}", result[i].key.c_str(), result[i].value.c_str());
//	}
//	GVariant *resultKeyValueSuccess = g_variant_builder_end(&resultKeyValueBuilder);
//	g_dbus_method_invocation_return_value(invocation, g_variant_new("(@a{ss})", resultKeyValueSuccess));
//	db_manager.CloseDatabase();
//	return;
//}
//void SessClient::HandlerGetAllRecords(GVariant *parameters,
//						  GDBusMethodInvocation *invocation){
//	gchar* nameDB, *nameTable;
//	g_variant_get(parameters, "(ss)", &nameDB, &nameTable);
//	OpenDataBase(nameDB, nameTable);
//	DatabaseManager& db_manager = DatabaseManager::GetInstance();
//	Vector<Record> result = db_manager.GetAllRecords();
//	GVariantBuilder resultKeyValueBuilder;
//	g_variant_builder_init(&resultKeyValueBuilder, G_VARIANT_TYPE("a{ss}"));
//	for (int i = 0; i < result.size(); i++) {
//		g_variant_builder_add(&resultKeyValueBuilder, "{ss}", result[i].key.c_str(), result[i].value.c_str());
//	}
//	GVariant *resultKeyValueSuccess = g_variant_builder_end(&resultKeyValueBuilder);
//	g_dbus_method_invocation_return_value(invocation, g_variant_new("(@a{ss})", resultKeyValueSuccess));
//	db_manager.CloseDatabase();
//	return;
//}
//void SessClient::HandlerBackupDatabase(GVariant *parameters,
//                           GDBusMethodInvocation *invocation
//){
//	LOG_INFO("backupdatabase");
//	gchar *nameDB;
//	g_variant_get(parameters, "(s)", &nameDB);
//	String path_database = DATABASE_DBUS_PATH + ConvertGchartoString(nameDB);
//	String path_backup_database = DATABASE_DBUS_PATH_BACKUP + ConvertGchartoString(nameDB);
//	Database::GetInstance().Initialize(database_platform, path_backup_database, path_backup_database, "");
//	Database::GetInstance().Initialize(database_platform,path_database,path_backup_database, "");
//	DatabaseManager& db_manager = DatabaseManager::GetInstance();
//	int status = db_manager.BackupDatabase();
//	g_dbus_method_invocation_return_value(invocation, g_variant_new("(b)", status == 0 ? TRUE : FALSE));
//	db_manager.CloseDatabase();
//	return;
//}
//void SessClient::HandlerRecoveredDatabase(GVariant *parameters,
//                              GDBusMethodInvocation *invocation
//){
//	gchar *nameDB;
//	g_variant_get(parameters, "(s)", &nameDB);
//	String path_database = DATABASE_DBUS_PATH + ConvertGchartoString(nameDB);
//	String path_backup_database = DATABASE_DBUS_PATH_BACKUP + ConvertGchartoString(nameDB);
//	Database::GetInstance().Initialize(database_platform,path_database,path_backup_database, "");
//	DatabaseManager& db_manager = DatabaseManager::GetInstance();
//	int status = db_manager.RecoverFromBackup();
//	g_dbus_method_invocation_return_value(invocation, g_variant_new("(b)", status == 0 ? TRUE : FALSE));
//	db_manager.CloseDatabase();
//	return;
//}
//void SessClient::HandlerUpdateRecord(GVariant *parameters,
//									 GDBusMethodInvocation *invocation) {
//	gchar *nameDB, *nameTable, *key, *value;
//	g_variant_get(parameters, "(ssss)", &nameDB,&nameTable, &key, &value);
//	OpenDataBase(nameDB, nameTable);
//
//	DatabaseManager& db_manager = DatabaseManager::GetInstance();
//	Record record(ConvertGchartoString(key) , ConvertGchartoString(value));
//	int status = db_manager.UpdateRecord(record);
//	db_manager.IterateRecords(Print);
//	g_dbus_method_invocation_return_value(invocation, g_variant_new("(b)", status == 0 ? TRUE : FALSE));
//	db_manager.CloseDatabase();
//	return;
//}
//void SessClient::HandlerUpdateMultiRecords(GVariant *parameters,
//										GDBusMethodInvocation *invocation) {
//	gchar *nameDB, *nameTable;
//	GVariant *keyValueArray;
//	g_variant_get(parameters, "(ss@a{ss})",&nameDB, &nameTable, &keyValueArray);
//	OpenDataBase(nameDB, nameTable);
//
//	DatabaseManager& db_manager = DatabaseManager::GetInstance();
//	gsize length = g_variant_n_children(keyValueArray);
//	gchar **key = g_new0(gchar *, length + 1);
//	gchar **value = g_new0(gchar *, length + 1);
//	for (gsize i = 0; i < length; i++) {
//		GVariant *keyValue = g_variant_get_child_value(keyValueArray, i);
//		g_variant_get(keyValue, "{ss}", &key[i], &value[i]);
//	}
//	Vector<Record> records;
//	for (int j = 0; key[j] != NULL; j++) {
//		Record record(ConvertGchartoString(key[j]) , ConvertGchartoString(value[j]));
//		records.push_back(record);
//	}
//	vector <int> status = db_manager.UpdateMultiRecords(records);
//	GVariantBuilder resultKeyValueBuilder;
//	g_variant_builder_init(&resultKeyValueBuilder, G_VARIANT_TYPE("a{sb}"));
//	for (int i = 0; i < records.size(); i++) {
//		g_variant_builder_add(&resultKeyValueBuilder, "{sb}", records[i].key.c_str(), (status[i] == 0 ? TRUE : FALSE));
//	}
//	GVariant *resultKeyValueSuccess = g_variant_builder_end(&resultKeyValueBuilder);
//	db_manager.IterateRecords(Print);
//	g_dbus_method_invocation_return_value(invocation, g_variant_new("(@a{sb})", resultKeyValueSuccess));
//	g_strfreev(key);
//	g_strfreev(value);
//	db_manager.CloseDatabase();
//	return;
//}
//void SessClient::HandlerSearchInValue(GVariant *parameters, GDBusMethodInvocation *invocation) {
//	gchar *nameDB, *nameTable;
//	GVariant *keyValueArray;
//	g_variant_get(parameters, "(ss@a{ss})",&nameDB, &nameTable, &keyValueArray);
//	OpenDataBase(nameDB, nameTable);
//
//	DatabaseManager& db_manager = DatabaseManager::GetInstance();
//	gsize length = g_variant_n_children(keyValueArray);
//	gchar **key = g_new0(gchar *, length + 1);
//	gchar **field = g_new0(gchar *, length + 1);
//	for (gsize i = 0; i < length; i++) {
//		GVariant *keyValue = g_variant_get_child_value(keyValueArray, i);
//		g_variant_get(keyValue, "{ss}", &key[i], &field[i]);
//	}
//	Vector<String> keys;
//	for (int j = 0; key[j] != NULL; j++) {
//		keys.push_back(ConvertGchartoString(key[j]));
//	}
//	Vector<Record> result = db_manager.GetMultiRecords(keys);
//	GVariantBuilder resultKeyValueBuilder;
//	g_variant_builder_init(&resultKeyValueBuilder, G_VARIANT_TYPE("a{ss}"));
//	for (int i = 0; i < result.size(); i++) {
//		json object_value = json::parse(result[i].value);
//		if(object_value.contains(ConvertGchartoString(field[i]))){
//			g_variant_builder_add(&resultKeyValueBuilder, "{ss}", result[i].key.c_str(), object_value[ConvertGchartoString(field[i])].dump().c_str());
//		}
//	}
//	GVariant *resultKeyValueSuccess = g_variant_builder_end(&resultKeyValueBuilder);
//	g_dbus_method_invocation_return_value(invocation, g_variant_new("(@a{ss})", resultKeyValueSuccess));
//	return;
//}
//void SessClient::OpenDataBase(gchar *nameDB, gchar *nameTable){
//	String nameDBString = ConvertGchartoString(nameDB);
//	String path_database = DATABASE_DBUS_PATH + nameDBString;
//	String path_backup_database = DATABASE_DBUS_PATH_BACKUP + nameDBString;
//	Database::GetInstance().Initialize(database_platform,path_database,path_backup_database, ConvertGchartoString(nameTable));
//	return;
//}
