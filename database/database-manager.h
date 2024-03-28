#ifndef DATABASE_DATABASE_MANAGER_H
#define DATABASE_DATABASE_MANAGER_H

#include "../common.hpp"
#include "database-interface.h"
#include "Libraries/Signal/SignalF.hpp"
#include "Libraries/Utils/Signal.hpp"
#include "database_config.hpp"
#include "database-command.hpp"
#include "database.h"
class DatabaseManager {
public:

//    static DatabaseManager& GetInstance();
    int InitializeDatabase(database_config config);
    int CloseDatabase();
    vector<int> AddRecords(DataBaseCommand database_command);
	vector<int> DeleteRecords(DataBaseCommand database_command);
	int DeleteAllRecords(DataBaseCommand database_command);
    vector<Record> GetRecords(DataBaseCommand database_command);
	vector<Record> GetAllRecords(DataBaseCommand database_command);
    int IterateRecords(function<void(Record& record)> callback,DataBaseCommand database_command);
    bool IsIntegrity(DataBaseCommand database_command);
    int CheckForCorruption();
    int RecoverFromBackup(database_config config);
    int BackupDatabase(database_config config);
	int CompactDatabase(database_config config);
    int CompressDataBase(const string& database_path_compress, const string &filename, const string &database_path_compress_to);
    int ExtractDataBase(const string& database_path_extract, const string &filename, const string &database_path_extract_to);
    void SetBackupPath(const string& path);
    const string& GetBackupPath() const;
    void SetDatabaseType(const string& database_type);
    const string& GetDatabaseType() const;
    void SetDatabasePath(const string& database_path);
    const string& GetDatabasePath() const;
    DatabaseManager(){}
    ~ DatabaseManager(){}
	DatabaseManager& operator=(const DatabaseManager& other) {
		// Implement custom copy logic here
		// Make sure to copy all necessary data members.
		return *this;
	}

private:
    unique_ptr<DatabaseInterface> database_interface_;
    //static DatabaseManager *database_manager_instance_;
    string database_type_;
    string database_path_;
    string backup_database_path_;
};

#endif //DATABASE_DATABASE_MANAGER_H
