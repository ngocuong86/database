#ifndef DATABASE_DATABASE_H
#define DATABASE_DATABASE_H

#include <utility>

#include "../common.hpp"
#include "database-interface.h"
#include "database-manager.h"

class Database {
public:
    Database(const Database&) = delete;

    static Database& GetInstance();
    static void Initialize(const string& database_type, const string& database_path, const string& backup_database_path, const string& tablename);

private:
    Database() {};
    ~ Database() = default;

    static Database* database_instance_;
};

inline Database* Database::database_instance_ = nullptr;

inline Database& Database::GetInstance() {
    if(!database_instance_) {
        database_instance_ = new Database();
    }
    return *database_instance_;
}

inline void Database::Initialize(const string& database_type, const string& database_path, const string& backup_database_path, const string& tablename) {
//    DatabaseManager.InitializeDatabase(database_type, database_path, backup_database_path,  tablename);
}

#endif //DATABASE_DATABASE_H
