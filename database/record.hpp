//
// Created by cuong on 27/10/2023.
//

#ifndef DATABASE_RECORD_HPP
#define DATABASE_RECORD_HPP
#include <string>
class Record {
public:
	std::string key;
	std::string value;

	Record(std::string k = "",
	       std::string v = "")
			:   key(std::move(k)),
			    value(std::move(v)) {}
};
#endif //DATABASE_RECORD_HPP
