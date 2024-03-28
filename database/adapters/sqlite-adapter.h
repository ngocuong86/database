#ifndef DATABASE_SQLITE_ADAPTER_H
#define DATABASE_SQLITE_ADAPTER_H

#include "common.hpp"
#include "database/database-interface.h"
#include "Libraries/Utils/Vector.hpp"
#include "database/database-manager.h"
#include "Libraries/Thread/Thread.hpp"
#include "database/database-command.hpp"
#include <sqlite3.h>
#include <fstream>

class SQLiteAdapter : public DatabaseInterface {
public:
    SQLiteAdapter();
    ~SQLiteAdapter() override;
	DatabaseManager database_maneger_;
	int Open(database_config config) override;
	int Close() override;
	vector<int> AddRecords(DataBaseCommand database_command) override;
	vector<int> DeleteRecords(DataBaseCommand database_command) override;
	int DeleteAllRecords(DataBaseCommand database_command) override;
	vector<Record> GetRecords(DataBaseCommand database_command) override;
	vector<Record> GetAllRecords(DataBaseCommand database_command) override;
	int IterateRecords(function<void(Record&)> callback, DataBaseCommand database_command) override;
    bool IsOpen(DataBaseCommand database_command) override;
    bool IsIntegrity(DataBaseCommand database_command) override;
	int RecoverFromBackup(database_config config) override;
	int BackupDatabase(database_config config) override;
	int CompressDataBase(const string& database_path_compress, const string& filename, const string& database_path_compress_to) override;
	int ExtractDataBase(const string& database_path_extract, const string& filename, const string& database_path_extract_to) override;
	int CompactDatabase(database_config config) override;
private:
    sqlite3* db_;
    sqlite3* db_backup_;
    sqlite3_backup *pBackup_;
    std::fstream sqlite_result;
};



#endif //DATABASE_SQLITE_ADAPTER_H
