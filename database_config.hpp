//
// Created by cuong on 26/10/2023.
//
#include <string>
#include <vector>
#ifndef DATABASE_DATABASE_CONFIG_HPP
#define DATABASE_DATABASE_CONFIG_HPP
typedef struct database_config{
	std::string database_platform = "" ;
	std::string database_path = "" ;
	std::string database_backup_path = "" ;
	std::string database_name = "" ;
	std::vector<std::string> database_table;
};
	extern std::vector<database_config> list_database_config_;
#endif //DATABASE_DATABASE_CONFIG_HPP
