#ifndef APPSERVER_INCLUDE_DATABASE_JSONCHECKER_H_
#define APPSERVER_INCLUDE_DATABASE_JSONCHECKER_H_

#include "../json11/json11.hpp"
#include "DBExceptions.h"

/**
 * Chequea que todos los args existan como campos del json
 * @param json				El json
 * @param args				const char*s
 */
template<typename... Args> void
	camposExisten(const json11::Json &json, const Args ...args) {
	const size_t size = sizeof...(args);
	const char* campos[size] = {args...};
	for (int i = 0; i < size; ++i)
	{
		if (json[campos[i]].is_null()) throw BadInputException(campos[i]);
	}
}

#endif /* APPSERVER_INCLUDE_DATABASE_JSONCHECKER_H_ */
