//
// Created by cuong on 26/10/2023.
//

#ifndef DATABASE_DATABASE_COMMAND_HPP
#define DATABASE_DATABASE_COMMAND_HPP
#include "Libraries/Utils/Vector.hpp"
#include "Libraries/Utils/String.hpp"
#include <gio/gio.h>
#include "record.hpp"
class DataBaseCommand{
public:
	String name_table;
	String name_db;
	Vector<Record> list_record;
	String flag;
	Vector<String> list_field;
	String type_connect;
	GDBusMethodInvocation *invocation;
};
#endif //DATABASE_DATABASE_COMMAND_HPP
