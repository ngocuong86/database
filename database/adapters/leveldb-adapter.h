#ifndef DATABASE_LEVELDBADAPTER_HPP
#define DATABASE_LEVELDBADAPTER_HPP

#include "../../common.hpp"
#include "../database-interface.h"
#include "database/database-command.hpp"
#include "Libraries/Utils/Map.hpp"
#include "leveldb/db.h"
#include <fstream>

class LevelDBAdapter : public DatabaseInterface {
public:
	LevelDBAdapter();

	~LevelDBAdapter() override;

	int Open(database_config config) override;

	int Close() override;

	vector<int> AddRecords(DataBaseCommand database_command) override;

	vector<int> DeleteRecords(DataBaseCommand database_command) override;

	//int DeleteARecordKeyword(const string& record_keyword) override;
	int DeleteAllRecords(DataBaseCommand database_command) override;

	//Record GetRecord(DataBaseCommand database_command) override;
	vector<Record> GetRecords(DataBaseCommand database_command) override;

	vector<Record> GetAllRecords(DataBaseCommand database_command) override;

	int IterateRecords(function<void(Record &)> callback, DataBaseCommand database_command) override;

	bool IsOpen(DataBaseCommand database_command) override;

	bool IsIntegrity(DataBaseCommand database_command) override;

	int RecoverFromBackup(database_config config) override;

	int BackupDatabase(database_config config) override;

	int CompressDataBase(const string &database_path_compress, const string &filename,
	                     const string &database_path_compress_to) override;

	int ExtractDataBase(const string &database_path_extract, const string &filename,
	                    const string &database_path_extract_to) override;
	int CompactDatabase(database_config config) override;
private:
	Map<String,leveldb::DB*> list_table;
    std::fstream level_result;
    struct timeval tv;
    //Map<String,leveldb::DB*> list_table_backup;
	leveldb::DB *db_;
    leveldb::WriteOptions write_opts;
};
#endif //DATABASE_LEVELDBADAPTER_HPP
