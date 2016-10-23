/*
 * DBChat.cpp
 *
 *  Created on: Oct 21, 2016
 *      Author: emanuel
 */

#include "../../include/database/DBChat.h"

DBChat::DBChat(const std::string& path) {
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, path, &db);
	if (!status.ok()) {
		db = 0;
		throw LevelDBException(status.ToString());
	}
}

DBChat::~DBChat() {
	if (db != 0)
		delete db;
}

