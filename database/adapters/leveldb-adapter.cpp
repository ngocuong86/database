#include "leveldb-adapter.h"
#include "Libraries/Log/LogPlus.hpp"
#include <leveldb/write_batch.h>
#include <leveldb/cache.h>
#include <leveldb/filter_policy.h>
#include <Libraries/Utils/json.hpp>
#include <leveldb/table.h>
#include <fstream>
using json = nlohmann::json;
database_config config_;
LevelDBAdapter::LevelDBAdapter():
level_result(){
    // db_ = nullptr;
}

LevelDBAdapter::~LevelDBAdapter() {
    LevelDBAdapter::Close();
    //delete db_;
}

int LevelDBAdapter::Open(database_config config) {
    leveldb::Options options;
//    options.write_buffer_size = 8 * 1048576;
//    options.create_if_missing = true;
//    options.block_cache = leveldb::NewLRUCache(10 * 1048576);
//    options.block_size = 2048;
//    options.compression = leveldb::kNoCompression;
//    options.filter_policy = leveldb::NewBloomFilterPolicy(40);
    write_opts.sync = false;
    config_ = config;
//    if(!level_result.open("levelDB.txt")){
//        level_result << "Open File Result error" << endl;
//        return -1;
//    }
    for(auto table : config_.database_table){
        leveldb::DB *db_;
        leveldb::DB *db_backup_;
        leveldb::Options options;
        options.create_if_missing = true;
        leveldb::Status status = leveldb::DB::Open(options,(config_.database_path + config_.database_name + "/" + table + ".db").c_str(), &db_);
        //leveldb::Status status_backup = leveldb::DB::Open(options,(config_.database_backup_path + config_.database_name + "/" + table + ".db").c_str(), &db_backup_);
        list_table[table] = db_;
        //list_table_backup[table] = db_backup_;
        if (!status.ok()) {
            cout << "--> FAIL: " << status.ToString() << endl;
            return -1;
        }
    }
}
int LevelDBAdapter::Close() {
    for(auto table : config_.database_table){
        if (list_table[table]) {
            delete list_table[table];
            list_table[table] = nullptr;
        }
    }
    return 0;
}
vector<int> LevelDBAdapter::AddRecords(DataBaseCommand database_command) {
    //LOG_ERRO("AddRecord");
    level_result.open("levelDB.txt", ios::app);
    level_result.seekg(0, ios::end);
    auto  start = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(start);

    std::tm *time_info = std::localtime(&now_c);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
           time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
           time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    leveldb::WriteBatch batch;
    vector<int> statusVector;
    write_opts.sync = false;
    for (const Record& record : database_command.list_record) {
        batch.Put(record.key, record.value);
    }
    leveldb::Status status = list_table[database_command.name_table]->Write(write_opts, &batch);
    if (!status.ok()) {
        LOG_ERRO("--> FAIL: %s", status.ToString().c_str());
        statusVector.push_back(-1);
    }else{
        statusVector.push_back(0);
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
    level_result << time << endl;
    level_result.close();
    return statusVector;
}
vector<int> LevelDBAdapter::DeleteRecords(DataBaseCommand database_command) {
    level_result.open("levelDB.txt", ios::app);
    level_result.seekg(0, ios::end);
    vector<int> statusVector;
    auto  start = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(start);

    std::tm *time_info = std::localtime(&now_c);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    // leveldb::WriteOptions write_opts;
    // write_opts.sync = true;
    for (Record record : database_command.list_record) {
        leveldb::Status status = list_table[database_command.name_table]->Delete(write_opts, record.key);
        if (!status.ok()) {
            LOG_ERRO("--> FAIL: %s", status.ToString().c_str());
            statusVector.push_back(-1);
        }else{
            statusVector.push_back(0);
        }
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
    level_result << time << endl;
    level_result.close();
    return statusVector;
}
int LevelDBAdapter::DeleteAllRecords(DataBaseCommand database_command) {
    // leveldb::WriteOptions write_opts;
    // write_opts.sync = true;
    level_result.open("levelDB.txt", ios::app);
    level_result.seekg(0, ios::end);
    leveldb::WriteBatch write_batch;
    leveldb::ReadOptions options;
    auto  start = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(start);

    std::tm *time_info = std::localtime(&now_c);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    leveldb::Iterator* iterator = list_table[database_command.name_table]->NewIterator(options);
    for(iterator->SeekToFirst(); iterator->Valid(); iterator->Next()) {
        write_batch.Delete(iterator->key());
    }
    delete iterator;
    leveldb::Status status = list_table[database_command.name_table]->Write(write_opts, &write_batch);
    if (!status.ok()) {
        LOG_ERRO("--> FAIL: %s", status.ToString().c_str());
        return -1;
    }
    list_table[database_command.name_table]->CompactRange( nullptr, nullptr);
    auto end = std::chrono::system_clock::now();
    //LOG_ERRO("AddRecord");
    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    float time = duration.count()/1000.0f;
    LOG_CRIT("Time: %f", time);
    level_result << time << endl;
    level_result.close();
    return 0;
}
vector<Record> LevelDBAdapter::GetRecords(DataBaseCommand database_command) {
    level_result.open("levelDB.txt", ios::app);
    level_result.seekg(0, ios::end);
    vector<Record> records;
    leveldb::ReadOptions read_opts;
    auto  start = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(start);

    std::tm *time_info = std::localtime(&now_c);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    for (Record record : database_command.list_record) {
        leveldb::Status status = list_table[database_command.name_table]->Get(read_opts, record.key, &record.value);
        if (status.ok()) {
            records.push_back(record);
        } else {
            LOG_ERRO("--> FAIL: %s", status.ToString().c_str());
            records.push_back(record);
        }
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
    level_result << time << endl;
    level_result.close();
    return records;
}
vector<Record> LevelDBAdapter::GetAllRecords(DataBaseCommand database_command){
    level_result.open("levelDB.txt", ios::app);
    level_result.seekg(0, ios::end);
    vector<Record> records;
    leveldb::ReadOptions read_opts;
    auto  start = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(start);

    std::tm *time_info = std::localtime(&now_c);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    leveldb::Iterator* it = list_table[database_command.name_table.c_str()]->NewIterator(read_opts);
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        string key = it->key().ToString();
        string value = it->value().ToString();
        Record record(key, value);
        records.push_back(record);
    }
    if (!it->status().ok()) {
        LOG_ERRO("--> FAIL: %s", it->status().ToString().c_str());
    }
    delete it;
    auto end = std::chrono::system_clock::now();
    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() % 1000;
    LOG_CRIT("Time: %02d:%02d:%02d.%03lld %02d/%02d/%d\n",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec, static_cast<long long>(milliseconds),
             time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    float time = duration.count()/1000.0f;
    LOG_CRIT("Time: %f", time);
    level_result << time << endl;
    level_result.close();
    return records;
}
int LevelDBAdapter::IterateRecords(function<void(Record&)> callback, DataBaseCommand database_command) {
    leveldb::ReadOptions read_opts;
    leveldb::Iterator* it = list_table[database_command.name_table.c_str()]->NewIterator(read_opts);
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        string key = it->key().ToString();
        string value = it->value().ToString();
        Record record(key, value);
        callback(record);
    }
    if (!it->status().ok()) {
        LOG_ERRO("--> FAIL: %s", it->status().ToString().c_str());
        return -1;
    }
    delete it;
    return 0;
}

bool LevelDBAdapter::IsOpen(DataBaseCommand database_command) {

}

bool LevelDBAdapter::IsIntegrity(DataBaseCommand database_command) {
    leveldb::ReadOptions read_opts;
    leveldb::Iterator* iterator = list_table[database_command.name_table.c_str()]->NewIterator(read_opts);
    bool integrity = true;
    for (iterator->SeekToFirst(); iterator->Valid(); iterator->Next()) {
        std::string key = iterator->key().ToString();
        std::string value = iterator->value().ToString();
        if (json::parse(value)){
            LOG_ERRO("--> FAIL: %s", iterator->status().ToString().c_str());
            integrity = false;
            break;
        }
    }
    if (!integrity) {
        LOG_ERRO("--> FAIL: %s", iterator->status().ToString().c_str());
    } else if (!iterator->status().ok()) {
        LOG_ERRO("--> FAIL: %s", iterator->status().ToString().c_str());
        integrity = false;
    }
    delete iterator;
    return integrity;
}

int LevelDBAdapter::RecoverFromBackup(database_config config) {
    leveldb::DB* backup_db;
    leveldb::Options options;
    options.error_if_exists = false;
    leveldb::Status status = leveldb::DB::Open(options, (config.database_backup_path + config.database_name), &backup_db);
    if (!status.ok()) {
        cout << "Error opening backup database" << status.ToString();
        return -1;
    }
    const leveldb::Snapshot* snapshot = backup_db->GetSnapshot();
    leveldb::WriteBatch recovery_batch;
    leveldb::ReadOptions recovery_read_options;
    leveldb::Iterator* iter = backup_db->NewIterator(recovery_read_options);
    for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
        recovery_batch.Put(iter->key(), iter->value());
    }
    backup_db->ReleaseSnapshot(snapshot);
    delete iter;

  //  LevelDBAdapter::DeleteAllRecords();

    leveldb::WriteOptions write_opts;
    write_opts.sync = true;
    status = db_->Write(write_opts, &recovery_batch);
    if (!status.ok()) {
        cout << "Error recovering database" << status.ToString();
		return -1;
    }
    delete backup_db;
	return 0;
//    std::string command;
//    command = "cp -r \"" + config.database_backup_path + config.database_name+ "\" \"" + config.database_path + "\"";
//    int exit_code = std::system(command.c_str());
//    if (exit_code == 0) {
//        LOG_INFO("Recover DataBase Success");
//    } else {
//        LOG_ERRO("Recover DataBase Error : %d: %s", exit_code, strerror(errno));
//    }
//    return exit_code;
}

int LevelDBAdapter::BackupDatabase(database_config config) {
    //leveldb::DB* db;
//    leveldb::Options options;
//    options.create_if_missing = true;  // Assuming the DB already exists
//
//    //leveldb::Status status = leveldb::DB::Open(options, db_path, &db);
//
////    if (!status.ok()) {
////        std::cout << "Error opening database: " << status.ToString() << std::endl;
////        return -1;
////    }
//    LOG_ERRO("BackupDatabase");
//    // Create a snapshot
//    const leveldb::Snapshot* snapshot = list_table["NEW_FLOORS"]->GetSnapshot();
//
//    // Create an iterator with the snapshot
//    leveldb::ReadOptions read_options;
//    read_options.snapshot = snapshot;
//    leveldb::Iterator* iter = list_table["NEW_FLOORS"]->NewIterator(read_options);
//    LOG_ERRO("BackupDatabase");
//    // Create a WriteBatch for backup
//    leveldb::WriteBatch backup_batch;
//
//    // Iterate through the database and add entries to the WriteBatch
//    for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
//        backup_batch.Put(iter->key(), iter->value());
//    }
//
//    // Release the snapshot and iterator
//    list_table["NEW_FLOORS"]->ReleaseSnapshot(snapshot);
//    delete iter;
//    LOG_ERRO("BackupDatabase");
//    // Open the backup database
////    leveldb::DB* backup_db;
////    leveldb::Status status = leveldb::DB::Open(options, (config.database_backup_path + "NEW_FLOORS.db"), &backup_db);
//
////    if (!status.ok()) {
////        std::cout << "Error opening backup database: " << status.ToString() << std::endl;
////        delete db;
////        return -1;
////    }
//
//    // Write the backup WriteBatch to the backup database
//    leveldb::Status status = list_table_backup["NEW_FLOORS"]->Write(leveldb::WriteOptions(), &backup_batch);
//
//    if (!status.ok()) {
//        std::cout << "Error writing backup database: " << status.ToString() << std::endl;
//    }
//    LOG_ERRO("BackupDatabase");
//    return 0;
    // Close the databases
//    delete db;
//    delete backup_db;

//    return 0;
//    std::string command;
//    command = "cp -r \"" + config.database_path + config.database_name + "\" \"" + config.database_backup_path + "\"";
//    int exit_code = std::system(command.c_str());
//    if (exit_code == 0) {
//        LOG_INFO("Backup DataBase Success");
//    } else {
//        LOG_ERRO("Backup DataBase Error : %d: %s", exit_code, strerror(errno));
//    }
//    return exit_code;
}
//int LevelDBAdapter::BackupDatabase(database_config config) {
//    try {
//        LOG_ERRO("LevelDBAdapter::BackupDatabase");
//
//        leveldb::Options options;
//        options.create_if_missing = true;
//
//        // Use unique_ptr for automatic cleanup
//        leveldb::DB* backup_db;
//        leveldb::Status status = leveldb::DB::Open(options,(config_.database_backup_path + "NEW_FLOORS.db"), &backup_db);
//
//        if (!status.ok()) {
//            LOG_ERRO("Error opening backup database");
//            throw std::runtime_error("Error opening backup database");
//        }
//
//        // RAII for Snapshot
//        struct SnapshotRAII {
//            const leveldb::Snapshot* snapshot;
//            const std::unordered_map<std::string, leveldb::Table*>& list_table_ref;
//
//            ~SnapshotRAII() {
//                list_table_ref.at("NEW_FLOORS")->ReleaseSnapshot(snapshot);
//            }
//        } snapshotRAII{ list_table["NEW_FLOORS"]->GetSnapshot(), list_table };
//
//
//        leveldb::WriteBatch backup_batch;
//
//        std::unique_ptr<leveldb::Iterator> iter(list_table["NEW_FLOORS"]->NewIterator(leveldb::ReadOptions()));
//
//        for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
//            backup_batch.Put(iter->key(), iter->value());
//        }
//
//        LOG_ERRO("LevelDBAdapter::BackupDatabase");
//
//        leveldb::WriteOptions write_opts;
//        write_opts.sync = true;
//
//        status = backup_db->Write(write_opts, &backup_batch);
//
//        if (!status.ok()) {
//            LOG_ERRO("Error writing backup database: ");
//            throw std::runtime_error("Error writing backup database");
//        }
//
//        LOG_ERRO("LevelDBAdapter::BackupDatabase");
//    } catch (const std::exception &e) {
//        LOG_ERRO("Exception: ");
//        return -1;
//    }
//
//    return 0;
//}
int LevelDBAdapter::CompressDataBase(const string &database_path_compress, const string &filename, const string &database_path_compress_to) {
    return 0;
}
int LevelDBAdapter::ExtractDataBase(const string &database_path_extract, const string &filename, const string &database_path_extract_to) {
    return 0;
}
int LevelDBAdapter::CompactDatabase(database_config config) {
    for(auto table : config.database_table){
        list_table[table]->CompactRange( nullptr, nullptr);
    }
}