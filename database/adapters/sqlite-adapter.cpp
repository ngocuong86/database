#include "sqlite-adapter.h"
#include <cstdlib>  // For system()
#include <cstring>  // For strerror()
#include <cerrno>   // For errno
#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/Utils/Vector.hpp"
#include "Libraries/Utils/json.hpp"
using json = nlohmann::json;

//String database_command.name_table;
SQLiteAdapter::SQLiteAdapter() :
sqlite_result("sqlite.txt"){
    db_ = nullptr;
}
SQLiteAdapter::~SQLiteAdapter() {
    SQLiteAdapter::Close();
    sqlite3_close(db_);
}
int SQLiteAdapter::Open(database_config config) {
    LOG_CRIT("Open SQLiteAdapter");
    int rc = sqlite3_open_v2(
            (config.database_path + config.database_name + ".db").c_str(),
            &db_,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
            nullptr
    );
    if (rc != SQLITE_OK) {
        LOG_CRIT("FAIL: %s", sqlite3_errmsg(db_));
    }
    for(String table_name : config.database_table)
    {
        if(!table_name.empty()) {
            string checkTableQuery = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + table_name + "';";
            int tableExists = 0;
            rc = sqlite3_exec(db_, checkTableQuery.c_str(), [](void *data, int argc, char **argv, char **colName) -> int {
                int *tableExists = reinterpret_cast<int *>(data);
                *tableExists = (argc > 0 && argv[0] != nullptr);
                return SQLITE_OK;
            }, &tableExists, nullptr);

            if (rc != SQLITE_OK) {
                LOG_ERRO("Error checking for table existence: %s", sqlite3_errmsg(db_));
                sqlite3_close(db_);
                return rc;
            }
            // create new table if not exists
            if (!tableExists) {
                string createTableQuery = "CREATE TABLE " + table_name + " (key TEXT PRIMARY KEY, value TEXT);";
                rc = sqlite3_exec(db_, createTableQuery.c_str(), nullptr, nullptr, nullptr);
                if (rc != SQLITE_OK) {
                    LOG_ERRO("Error creating table: %s", sqlite3_errmsg(db_));
                    sqlite3_close(db_);
                }
            }
        }
    }
//    LOG_CRIT("Open SQLiteAdapter Backup");
//    rc = sqlite3_open_v2(
//            (config.database_backup_path + config.database_name + ".db").c_str(),
//            &db_backup_,
//            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
//            nullptr
//    );
//    if (rc != SQLITE_OK) {
//        LOG_CRIT("FAIL: %s", sqlite3_errmsg(db_));
//    }
LOG_CRIT("Open SQLiteAdapter Backup Success");
    return rc;
}

int SQLiteAdapter::Close() {
    if(db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
    return 0;
}
Vector<int> SQLiteAdapter::AddRecords(DataBaseCommand database_command) {
    sqlite_result.open("sqlite.txt", ios::app);
sqlite_result.seekg(0, ios::end);
    Vector<int> rcVector;
    auto  start = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(start);

    std::tm *time_info = std::localtime(&now_c);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    sqlite3_exec(db_, "BEGIN TRANSACTION", NULL, 0, NULL);
    for(auto record : database_command.list_record){
        string query = "INSERT INTO " + database_command.name_table + " VALUES('" + record.key + "','" + record.value + "')";
        int rc = sqlite3_exec(db_, query.c_str(), NULL, 0, NULL);
        if (rc != SQLITE_OK) {
            if(rc == SQLITE_CONSTRAINT) {
                string query = "UPDATE " + database_command.name_table + " SET value='" + record.value + "' WHERE key='" + record.key + "'";
                rc = sqlite3_exec(db_, query.c_str(), NULL, 0, NULL);
                if (rc != SQLITE_OK) {

                    LOG_ERRO("--> FAIL: %s", sqlite3_errmsg(db_));
                }
            }else{
                LOG_ERRO("--> FAIL: %s", sqlite3_errmsg(db_));
            }
        }
        rcVector.push_back(rc);
    }
    sqlite3_exec(db_, "END TRANSACTION", NULL, NULL, NULL);
    auto end = std::chrono::system_clock::now();
    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    float time = duration.count()/1000.0f;
    LOG_CRIT("Time: %f", time);
    sqlite_result << time << std::endl;
    sqlite_result.close();
    return rcVector;
}
vector<int> SQLiteAdapter::DeleteRecords(DataBaseCommand database_command) {
    sqlite_result.open("sqlite.txt", ios::app);
sqlite_result.seekg(0, ios::end);
    vector<int> rcVector;
    auto  start = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(start);

    std::tm *time_info = std::localtime(&now_c);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    for (auto record: database_command.list_record) {
        string query = "DELETE FROM " + database_command.name_table + " WHERE key='" + record.key + "'";
        int result = sqlite3_exec(db_, query.c_str(), NULL, 0, NULL);
        if (result != SQLITE_OK) {
            rcVector.push_back(result);
            LOG_ERRO("--> FAIL: %s", sqlite3_errmsg(db_));
        }else {
            rcVector.push_back(result);
        }
    }
    auto end = std::chrono::system_clock::now();
    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    float time = duration.count()/1000.0f;
    LOG_CRIT("Time: %f", time);
    sqlite_result << time << std::endl;
    sqlite_result.close();
    return rcVector;
}
int SQLiteAdapter::DeleteAllRecords(DataBaseCommand database_command) {
    sqlite_result.open("sqlite.txt", ios::app);
sqlite_result.seekg(0, ios::end);
    auto  start = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(start);

    std::tm *time_info = std::localtime(&now_c);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    string query = "DELETE FROM " + database_command.name_table;
    int result = sqlite3_exec(db_, query.c_str(), NULL, 0, NULL);
    String query_vacuum = "VACUUM";
    char *errorMessage = nullptr;
    result = sqlite3_exec(db_, query_vacuum.c_str(), NULL, 0, &errorMessage);
    if (result != SQLITE_OK) {
        LOG_ERRO("--> FAIL: %s", sqlite3_errmsg(db_));
        sqlite3_free(errorMessage);
    }
    auto end = std::chrono::system_clock::now();
    //LOG_ERRO("AddRecord");
    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    float time = duration.count()/1000.0f;
    LOG_CRIT("Time: %f", time);
    sqlite_result << time << std::endl;
    sqlite_result.close();
    if (result != SQLITE_OK) {
        LOG_ERRO("--> FAIL: %s", sqlite3_errmsg(db_));
        return -1;
    }else{
        return 0;
    }
}

vector<Record> SQLiteAdapter::GetRecords(DataBaseCommand database_command) {
    sqlite_result.open("sqlite.txt", ios::app);
sqlite_result.seekg(0, ios::end);
    //cout << __func__ << endl;
    vector<Record> records;
    Record record;
    auto  start = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(start);

    std::tm *time_info = std::localtime(&now_c);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    for(auto key_record: database_command.list_record) {
        string query = "SELECT * FROM " + database_command.name_table + " WHERE key='" +key_record.key + "'";
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            if(sqlite3_step(stmt) == SQLITE_ROW) {
                record.key = std::string((const char*)sqlite3_column_text(stmt, 0));
                record.value = std::string((const char*)sqlite3_column_text(stmt, 1));
            }
        }
        sqlite3_finalize(stmt);
        records.push_back(record);
    }
    auto end = std::chrono::system_clock::now();
    //LOG_ERRO("AddRecord");
    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    float time = duration.count()/1000.0f;
    LOG_CRIT("Time: %f", time);
    sqlite_result << time << std::endl;
    sqlite_result.close();
    return records;
}
vector<Record> SQLiteAdapter::GetAllRecords(DataBaseCommand database_command) {
    //chưa check
    sqlite_result.open("sqlite.txt", ios::app);
    sqlite_result.seekg(0, ios::end);
    vector<Record> records;
    sqlite3_stmt* stmt;
    string query = "SELECT * FROM " + database_command.name_table ;
    auto  start = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(start);
    std::tm *time_info = std::localtime(&now_c);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    if(sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
        while(sqlite3_step(stmt) == SQLITE_ROW) {
            Record record;
            record.key = string((char*)sqlite3_column_text(stmt,0));
            record.value = string((char*)sqlite3_column_text(stmt,1));
            records.push_back(record);
        }
    }
    sqlite3_finalize(stmt);
    auto end = std::chrono::system_clock::now();
    //LOG_ERRO("AddRecord");
    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    float time = duration.count()/1000.0f;
    LOG_CRIT("Time: %f", time);
    sqlite_result << time << std::endl;
    sqlite_result.close();
    return records;
}

int SQLiteAdapter::IterateRecords(function<void(Record&)> callback, DataBaseCommand database_command) {
    //cout << __func__ << endl;
    sqlite3_stmt* stmt;
    string query = "SELECT * FROM " + database_command.name_table;
    if(sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
        while(sqlite3_step(stmt) == SQLITE_ROW) {
            Record record;
            record.key = string((char*)sqlite3_column_text(stmt,0));
            record.value = string((char*)sqlite3_column_text(stmt,1));
            callback(record);
        }
    }
    sqlite3_finalize(stmt);

}

bool SQLiteAdapter::IsOpen(DataBaseCommand database_command) {
    //cout << __func__ << endl;
    return db_ != NULL;
}

bool SQLiteAdapter::IsIntegrity(DataBaseCommand database_command) {
    //cout << __func__ << endl;
    int integrity;
    string query = "PRAGMA integrity_check";
    integrity = sqlite3_exec(db_, query.c_str(), nullptr, nullptr, nullptr);
    if (integrity != SQLITE_OK ) {
        LOG_ERRO("--> FAIL: %s", sqlite3_errmsg(db_));
        return false;
    }else{
        sqlite3_stmt* stmt;
        string query = "SELECT * FROM " + database_command.name_table ;
        if(sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            while(sqlite3_step(stmt) == SQLITE_ROW) {
                Record record;
                record.key = string((char*)sqlite3_column_text(stmt,0));
                record.value = string((char*)sqlite3_column_text(stmt,1));
                if (json::parse(record.value)){
                    LOG_ERRO("--> FAIL: %s", sqlite3_errmsg(db_));
                    return false;
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    return true;
}

int SQLiteAdapter::RecoverFromBackup(database_config config) {
    std::string command;
    command = "cp \"" + config.database_backup_path + config.database_name + ".db" + "\" \"" + config.database_path + "\"";
    int exit_code = std::system(command.c_str());
    if (exit_code == 0) {
        LOG_INFO("Recover DataBase Success");
    } else {
        LOG_ERRO("Recover DataBase Error : %d: %s", exit_code, strerror(errno));
    }
    return exit_code;
}

int SQLiteAdapter::BackupDatabase(database_config config) {
    LOG_CRIT("BACKUPDATABASE");
    sqlite_result.open("sqlite.txt", ios::app);
    sqlite_result.seekg(0, ios::end);
    auto  start = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(start);
    std::tm *time_info = std::localtime(&now_c);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    pBackup_ = sqlite3_backup_init(db_backup_, "main", db_, "main");
    if (pBackup_) {
        (void)sqlite3_backup_step(pBackup_, -1);
        (void)sqlite3_backup_finish(pBackup_);
    }else{
        LOG_ERRO("--> FAIL: %s", sqlite3_errmsg(db_));
    }
    auto end = std::chrono::system_clock::now();
    //LOG_ERRO("AddRecord");
    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    float time = duration.count()/1000.0f;
    LOG_CRIT("Time: %f", time);
    sqlite_result << time << std::endl;
    sqlite_result.close();
//    std::string command;
//    command = "cp \"" + config.database_path + config.database_name + ".db"+ "\" \"" + config.database_backup_path + "\"";
//    int exit_code = std::system(command.c_str());
//    if (exit_code == 0) {
//        LOG_INFO("Backup DataBase Success");
//    } else {
//        LOG_ERRO("Backup DataBase Error : %d: %s", exit_code, strerror(errno));
//    }
//    return exit_code;
}
int SQLiteAdapter::CompressDataBase(const string& database_path_compress, const string& filename, const string& database_path_compress_to){
    std::string command = "tar -czf " + database_path_compress_to + "/" + filename + ".tar.gz" + " -C " + database_path_compress + " " + filename + ".db";
    int exit_code = std::system(command.c_str());
    if (exit_code == 0) {
        LOG_INFO("Compress DataBase Success");
    } else {
        LOG_ERRO("Compress DataBase Error : %d: %s", exit_code, strerror(errno));
    }
    return exit_code;
}
int SQLiteAdapter::ExtractDataBase(const string& database_path_extract, const string& filename, const string& database_path_extract_to) {
    std::string command = "tar -xzf " + database_path_extract + "/" + filename + ".tar.gz"+" -C "+ database_path_extract_to;
    int exit_code = std::system(command.c_str());
    if (exit_code == 0) {
        LOG_INFO("Extract DataBase Success");
    } else {
        LOG_ERRO("Extract DataBase Error : %d: %s", exit_code, strerror(errno));
    }
    return exit_code;
}
int SQLiteAdapter::CompactDatabase(database_config config) {
    String query = "VACUUM";
    char *errorMessage = nullptr;
    int result = sqlite3_exec(db_, query.c_str(), NULL, 0, &errorMessage);
    if (result != SQLITE_OK) {
        LOG_ERRO("--> FAIL: %s", sqlite3_errmsg(db_));
        sqlite3_free(errorMessage);
    }
    return result;
}