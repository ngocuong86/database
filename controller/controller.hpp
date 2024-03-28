//
// Created by cuong on 05/10/2023.
//
#include <session/session.hpp>
#include "database/database-command.hpp"
#include <database/database.h>
#include <database/database-manager.h>
//#include "Libraries/Thread/ThreadPool/BS_thread_pool_light.hpp"
#ifndef DATABASE_CONTROLLER_HPP
#define DATABASE_CONTROLLER_HPP
typedef Signal<void(Vector<int> , Vector<Record>, DataBaseCommand)> SignalControllerToSession;
class ControllerDataBase{
public:
	SignalControllerToSession signal_controller;
	SessClient_t session_;
	BlockingQueue<DataBaseCommand>      queue_db;
	Locker                              locker_zigbee_db;
	Thread                              thread_db;
	Func1R<void_p, void_p>              thread_functor_db;
	BlockingQueue<DataBaseCommand>      queue_hc_db;
	Locker                              locker_hc_db;
	Thread                              thread_hc_db;
	Func1R<void_p, void_p>              thread_functor_backup_data;
	Thread                              thread_backup_data;
	ControllerDataBase();
	~ControllerDataBase();
	void_p ProcZigbeeDB(void_p pbyBuffer);
	DatabaseManager* zigbee_manager;
	DatabaseManager* hc_manager;
    DatabaseManager* rockdb_manager;
	void ReceivedFromDbus(DataBaseCommand database_command);
	void HandleDB(DataBaseCommand database_command);
	void_p BackupDataToFlash(void_p pbyBuffer);
	void Start();
	void Process();
	SignalControllerToSession & ControllerSignal();
	bool isZigbeeChange = false;
	bool isHcChange = false;
    //bool isRockdbChange = false;
	database_config zigbee_config;
	database_config hc_config;
    //database_config rockdb_config;
	//BS::thread_pool_light thread_pool;
};
#endif //DATABASE_CONTROLLER_HPP
