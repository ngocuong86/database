#include "Libraries/Log/Log.hpp"
#include "Libraries/Utils/json.hpp"
#include "common.hpp"
#include "controller/controller.hpp"
#include "database/adapters/sqlite-adapter.h"
#include "database/database.h"
#include "database_config.hpp"
#include "leveldb/db.h"
#include "session/local_session.hpp"
#include <fstream>
#include <getopt.h>
#include <iostream>
using json = nlohmann::json;
std::string database_platform = "";
std::vector<database_config> list_database_config_;
void PrintHelp(const char *appName, const String &txt) {
  std::cout << txt
            << "\n"
               "Usage: "
            << appName
            << "\t[-o]\n"
               "\t\t[--config_platform  platform config sqlite or leveldb]\n"
               "\t\t[--help   Help menu]\n";
  exit(1);
}
void LoadFileConfig(String database_path_config) {
  std::cout << "Load file config database" << std::endl;
  std::ifstream file(database_path_config);
  if (!file.is_open()) {
    std::cerr << "Can't open file config" << std::endl;
  }
  json json_data;
  file >> json_data;
  for (auto &item : json_data.items()) {
    std::cout << "Get value" << std::endl;
    database_config config;
    if (item.value().contains("database_platform")) {
      std::cout << "database_platform" << std::endl;
      config.database_platform = item.value()["database_platform"];
    }
    if (item.value().contains("database_path")) {
      std::cout << "database_path" << std::endl;
      config.database_path = item.value()["database_path"];
    }
    if (item.value().contains("database_backup_path")) {
      std::cout << "database_backup_path" << std::endl;
      config.database_backup_path = item.value()["database_backup_path"];
    }
    if (item.value().contains("database_name")) {
      std::cout << "database_name" << std::endl;
      config.database_name = item.value()["database_name"];
    }

    if (item.value().contains("database_table")) {
      std::cout << "database_table" << std::endl;
      for (auto &table : item.value()["database_table"].items()) {
        config.database_table.push_back(table.value());
      }
    }
    list_database_config_.push_back(config);
  }

  // Kiểm tra xem dữ liệu đã được đọc thành công hay chưa
  for (const auto &config : list_database_config_) {
    std::cout << "Database Platform: " << config.database_platform << std::endl;
    std::cout << "Database Path: " << config.database_path << std::endl;
    std::cout << "Database Backup Path: " << config.database_backup_path
              << std::endl;
    std::cout << "Database Name: " << config.database_name << std::endl;
    std::cout << "Database Tables: ";
    for (const auto &table : config.database_table) {
      std::cout << table << " ";
    }
    std::cout << std::endl;
  }
}
int main(int argc, char *argv[]) {
  String database_config_path;
  const char *const short_opts = "c:h";
  const struct option longopts[] = {
      {"config_platform", required_argument, nullptr, 'c'},
      {"help", no_argument, nullptr, 'h'},
      {nullptr, 0, nullptr, 0},
  };
  int_t index;
  int_t iarg = 0;

  while (TRUE) {
    iarg = getopt_long(argc, argv, short_opts, longopts, &index);
    if (argc == 1) {
      PrintHelp(argv[0], "Please set config");
      break;
    }
    if (iarg == -1) {
      break;
    }
    switch (iarg) {
    case 'c':
      database_config_path = String(optarg);
      break;
    case 'h':
      PrintHelp(argv[0], "Display help!");
      break;
    default:
      PrintHelp(argv[0], "Invalid argument!");
      break;
    }
  }
  Log::Create(DATABASE_LOG_PATH, TRUE, TRUE, Log::INFO, Log::ALL);
  LoadFileConfig(database_config_path);
  // LOG_INFO("==============DATABASE SERVICE WORK WITH PLATFORM
  // %s==================",database_platform.c_str());
  LocalSession local_session;
  local_session.Start();
  while (TRUE) {
    local_session.Process();
  }
  return 0;
}
