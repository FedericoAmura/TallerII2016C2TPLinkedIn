/*
 * DBChat.h
 *
 *  Created on: Oct 21, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_DATABASE_DBCHAT_H_
#define APPSERVER_INCLUDE_DATABASE_DBCHAT_H_

#include "../leveldb/db.h"
#include "DBExceptions.h"

class DBChat {
private:
	leveldb::DB* db;

public:
	DBChat(const std::string& path);

	void saveNewMsg();

	virtual ~DBChat();
};

#endif /* APPSERVER_INCLUDE_DATABASE_DBCHAT_H_ */
