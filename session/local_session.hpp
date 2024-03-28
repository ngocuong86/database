//
// Created by cuong on 27/10/2023.
//

#ifndef DATABASE_LOCAL_SESSION_HPP
#define DATABASE_LOCAL_SESSION_HPP
#include "controller/controller.hpp"
#include "session.hpp"
class LocalSession{
public:
	LocalSession();
	~LocalSession();
	Func1R<void*, void*>        func1RLocalSession_;
	Thread      threadLocalSession_;
	ControllerDataBase controller;
	void * ConnectLocalSession(void *buffer);
	void ReceivedFromLocal(Vector<int> status, Vector<Record> list_record, DataBaseCommand database_command);
	void Start();
	void Process();
};
#endif //DATABASE_LOCAL_SESSION_HPP
