//
// Created by cuong on 05/10/2023.
//
#include "controller.hpp"

#include "Libraries/Timer/SysTemTimer.hpp"
#include "Libraries/Utils/json.hpp"
#include "database/adapters/leveldb-adapter.h"
#include "database/adapters/sqlite-adapter.h"
#include "database_config.hpp"
#include "fstream"
//#include "third_party/bprinter/include/bprinter/table_printer.h"
using json = nlohmann::json;
// void Print(Record& record) {
//	bprinter::TablePrinter tp(&std::cout);
//	tp.AddColumn("Key", 25);
//	tp.AddColumn("Value", 50);
//
//	tp << record.key << record.value;
//}
ControllerDataBase::ControllerDataBase()
    : session_(),
      thread_functor_db(makeFunctor((Func1R<void_p, void_p> *)NULL, *this,
                                    &ControllerDataBase::ProcZigbeeDB)),
      thread_functor_backup_data(
          makeFunctor((Func1R<void_p, void_p> *)NULL, *this,
                      &ControllerDataBase::BackupDataToFlash)),
      thread_db() {
  thread_db.AddFunctor(thread_functor_db);
  thread_backup_data.AddFunctor(thread_functor_backup_data);
  thread_db.Start();
  //thread_backup_data.Start();
  session_.SessionSignal() += Slot(this, &ControllerDataBase::ReceivedFromDbus);
  LOG_CRIT("ControllerDataBase::ControllerDataBase");
}
ControllerDataBase::~ControllerDataBase() {
  free(zigbee_manager);
  free(hc_manager);
}
void_p ControllerDataBase::ProcZigbeeDB(void_p pbyBuffer) {
  LOG_ERRO("ControllerDataBase::ProcZigbeeDB");
  while (TRUE) {
    try {
      DataBaseCommand database_command;
      while (!queue_db.IsEmpty()) {
          if (queue_db.TryPop(database_command)) {
              HandleDB(database_command);
//          					thread_pool.push_task([this,
//          database_command]() {
//          HandleDB(database_command);
//          					});
//        }
          }
          if (queue_db.WaitPop(database_command, 10)) {
              //				thread_pool.push_task([this,
              // database_command]() {
              // HandleDB(database_command);
              //				});
              HandleDB(database_command);
          }
          SYSTIMER().Process();
      }
      // usleep(10000);  //50ms
    } catch (std::exception &ex) {
      LOG_ERRO(ex.what());
    }
  }
}
void ControllerDataBase::Start() {
  for (auto config : list_database_config_) {
    LOG_CRIT("DATABASE NAME: %s", config.database_name.c_str());
    if (config.database_name == "zigbee") {
      zigbee_config = config;
      zigbee_manager = new DatabaseManager();
      zigbee_manager->InitializeDatabase(config);
    } else if (config.database_name == "newhc") {
      LOG_CRIT("HC");
      hc_config = config;
      hc_manager = new DatabaseManager();
      hc_manager->InitializeDatabase(config);
    }
  }
  return;
}
void ControllerDataBase::ReceivedFromDbus(DataBaseCommand database_command) {
  LOG_ERRO("ControllerDataBase::ReceivedFromDbus");
  try {
    if (database_command.name_db == "zigbee" ||
        database_command.name_db == "newhc") {
      queue_db.Push(database_command);
    } else {
      LOG_ERRO("ControllerDataBase::ReceivedFromDbus: name_db not found");
    }
  } catch (Exception &e) {
    LOG_CRIT("ControllerDataBase::ReceivedFromDbus: %s", e.what());
  }
}
void ControllerDataBase::HandleDB(DataBaseCommand database_command) {
  LOG_ERRO("ControllerDataBase::HandleDB");
  vector<int> status;
  vector<Record> records;
  DatabaseManager *database_manager;

  if (database_command.name_db == "zigbee") {
    isZigbeeChange = true;
    database_manager = zigbee_manager;
  } else if (database_command.name_db == "newhc") {
    isHcChange = true;
    database_manager = hc_manager;
  }else {
    LOG_ERRO("ControllerDataBase::HandleDB: name_db not found");
    return;
  }
  if (database_command.flag == DBUS_METHOD_ADD_RECORD) {
    status = database_manager->AddRecords(database_command);
  } else if (database_command.flag == DBUS_METHOD_ADD_MULTI_RECORDS) {
    status = database_manager->AddRecords(database_command);
  } else if (database_command.flag == DBUS_METHOD_DELETE_RECORD ||
             database_command.flag == DBUS_METHOD_DELETE_MULTI_RECORDS) {
    status = database_manager->DeleteRecords(database_command);
  } else if (database_command.flag == DBUS_METHOD_DELETE_ALL_RECORDS) {
    status.push_back(database_manager->DeleteAllRecords(database_command));
  } else if (database_command.flag == DBUS_METHOD_GET_RECORD ||
             database_command.flag == DBUS_METHOD_GET_MULTI_RECORDS) {
    records = database_manager->GetRecords(database_command);
  } else if (database_command.flag == DBUS_METHOD_GET_ALL_RECORDS) {
    records = database_manager->GetAllRecords(database_command);
  } else if (database_command.flag == DBUS_METHOD_SEARCH_IN_VALUE) {
    records = database_manager->GetRecords(database_command);
  } else if (database_command.flag == DBUS_METHOD_BACKUP_DATABASE){
      status.push_back(database_manager->BackupDatabase(zigbee_config));
  }
  else {
    LOG_ERRO("HandleDB: flag not found");
  }
  if (database_command.type_connect == DATABASE_TYPE_CONNECT_DBUS) {
    signal_controller.Emit(status, records, database_command);
  }
}
void ControllerDataBase::Process() {
  try {
    SYSTIMER().Process();
    usleep(50000);  /// Sleep 50 ms
  } catch (std::exception &ex) {
    LOG_ERRO(ex.what());
  }
}
void_p ControllerDataBase::BackupDataToFlash(void_p pbyBuffer) {
  LOG_ERRO("ControllerDataBase::BackupDataToFlash && CompactDatabase");
  while (TRUE) {
    try {
      if (zigbee_manager != NULL && isZigbeeChange) {
        zigbee_manager->CompactDatabase(zigbee_config);
        zigbee_manager->BackupDatabase(zigbee_config);
        isZigbeeChange = false;
      }
      if (hc_manager != NULL && isHcChange) {
        LOG_ERRO("ControllerDataBase::BackupDataToFlash && CompactDatabase");
        hc_manager->CompactDatabase(hc_config);
        hc_manager->BackupDatabase(hc_config);
        isHcChange = false;
      }
      SYSTIMER().Process();
      sleep(1800);  // 30s
    } catch (std::exception &ex) {
      LOG_ERRO(ex.what());
    }
  }
}
SignalControllerToSession &ControllerDataBase::ControllerSignal() {
  return signal_controller;
}