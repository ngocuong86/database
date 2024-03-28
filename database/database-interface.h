#ifndef DATABASE_DATABASEINTERFACE_HPP
#define DATABASE_DATABASEINTERFACE_HPP

#include <utility>

#include "../common.hpp"
#include "database_config.hpp"
#include "database-command.hpp"
#include "record.hpp"
//#include "database-command.hpp"


class DatabaseInterface {
public:
    virtual ~DatabaseInterface() = default;

    virtual int Open(database_config config) = 0;
    virtual int Close() = 0;
	virtual vector<int> AddRecords(DataBaseCommand database_command) = 0;
	virtual vector<int> DeleteRecords(DataBaseCommand database_command) = 0;
	virtual int DeleteAllRecords(DataBaseCommand database_command) = 0;
	virtual vector<Record> GetRecords(DataBaseCommand database_command) = 0;
	virtual vector<Record> GetAllRecords(DataBaseCommand database_command)  = 0;
    virtual int IterateRecords(function<void(Record& record)> callback,DataBaseCommand database_command) = 0;
    virtual bool IsOpen(DataBaseCommand database_command) = 0;
    virtual bool IsIntegrity(DataBaseCommand database_command) = 0;
    virtual int RecoverFromBackup(database_config config) = 0;
    virtual int BackupDatabase(database_config config) = 0;
	virtual int CompactDatabase(database_config config) = 0;
    virtual int CompressDataBase(const string& database_path_compress, const string& filename, const string& database_path_compress_to) = 0;
    virtual int ExtractDataBase(const string& database_path_extract, const string& filename, const string& database_path_extract_to) = 0;
};


#endif //DATABASE_DATABASEINTERFACE_HPP
