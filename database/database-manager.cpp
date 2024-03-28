#include <memory>
#include <experimental/filesystem>
#include <utility>

#include "../common.hpp"
#include "database-manager.h"
#include "adapters/leveldb-adapter.h"
#include "adapters/sqlite-adapter.h"
//#include "adapters/rockdb-adapter.h"
#include "database_config.hpp"
#include "database-command.hpp"
//DatabaseManager* DatabaseManager::database_manager_instance_ = nullptr;
//
//DatabaseManager& DatabaseManager::GetInstance() {
//    if(!database_manager_instance_) {
//        database_manager_instance_ = new DatabaseManager();
//    }
//    return *database_manager_instance_;
//}

int DatabaseManager::InitializeDatabase(database_config config) {
    SetDatabaseType(config.database_platform);
    SetDatabasePath(config.database_path);
    SetBackupPath(config.database_backup_path);
    if (config.database_platform ==  DATABASE_PLATFORM_LEVELDB) {
        database_interface_ = std::make_unique<LevelDBAdapter>();
    } else if (config.database_platform ==  DATABASE_PLATFORM_SQLITE) {
        database_interface_ = std::make_unique<SQLiteAdapter>();
    }
    else {
        cout << "--> FAIL platform wrong" << endl;
		return -1;
    }
    return database_interface_->Open(config);
}

int  DatabaseManager::CloseDatabase() {
    return database_interface_->Close();
}

vector<int> DatabaseManager::AddRecords(DataBaseCommand database_command) {
    return database_interface_->AddRecords(database_command);
}
vector<int> DatabaseManager::DeleteRecords(DataBaseCommand database_command) {
    return database_interface_->DeleteRecords(database_command);
}
int DatabaseManager::DeleteAllRecords(DataBaseCommand database_command) {
    return database_interface_->DeleteAllRecords(database_command);
}

vector<Record> DatabaseManager::GetRecords(DataBaseCommand database_command) {
    return  database_interface_->GetRecords(database_command);
}
vector<Record> DatabaseManager::GetAllRecords(DataBaseCommand database_command) {
	return  database_interface_->GetAllRecords(database_command);
}

bool DatabaseManager::IsIntegrity(DataBaseCommand database_command) {
    return  database_interface_->IsIntegrity(database_command);
}

int DatabaseManager::CheckForCorruption() {
//    bool integrity = database_interface_->IsIntegrity();
//    if(!integrity) {
//        cout << "Database corrupted --> recover: " << GetBackupPath() << endl;
//       return database_interface_->RecoverFromBackup(database_path_, backup_database_path_);
//    }
}

int DatabaseManager::RecoverFromBackup(database_config config) {
   return  database_interface_->RecoverFromBackup( config);
}

int DatabaseManager::BackupDatabase(database_config config) {
    LOG_ERRO("BackupDatabase");
   return database_interface_->BackupDatabase(config);
}

void DatabaseManager::SetBackupPath(const string& backup_database_path) {
    backup_database_path_ = backup_database_path;
}

const string& DatabaseManager::GetBackupPath() const {
    return backup_database_path_;
}

void DatabaseManager::SetDatabaseType(const string& database_type) {
    database_type_ = database_type;
}

const string& DatabaseManager::GetDatabaseType() const {
    return database_type_;
}

void DatabaseManager::SetDatabasePath(const string& database_path) {
    database_path_ = database_path;
}
const string& DatabaseManager::GetDatabasePath() const {
    return database_path_;
}
int DatabaseManager::IterateRecords(function<void(Record &)> callback, DataBaseCommand database_command) {
	return database_interface_->IterateRecords(std::move(callback), database_command);
}
int DatabaseManager::CompressDataBase(const string& database_path_compress, const string &filename, const string &database_path_compress_to) {
	return database_interface_->CompressDataBase(database_path_compress, filename,database_path_compress_to);
}
int DatabaseManager::ExtractDataBase(const string& database_path_extract, const string &filename, const string &database_path_extract_to) {
	return database_interface_->ExtractDataBase(database_path_extract, filename,database_path_extract_to);
}
int DatabaseManager::CompactDatabase(database_config config) {
	return database_interface_->CompactDatabase(config);
}