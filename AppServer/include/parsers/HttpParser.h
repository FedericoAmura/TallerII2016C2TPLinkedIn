/*
 * HttpParser.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_PARSERS_HTTPPARSER_H_
#define APPSERVER_INCLUDE_PARSERS_HTTPPARSER_H_

#include "../mongoose/mongoose.h"
#include "../server/API_HttpRequest.h"
#include "../common/Exceptions.h"
#include "../common/Utils.h"
#include <stdlib.h>

using json11::Json;

class HttpParser {
private:
	/**
	 * Obtener header de petición HTTP
	 * @return	una estructura mg_str* con información del nombre de la cabecera
	 *			específicada en "name"
	 */
	static struct mg_str* get_header(struct http_message* msg, const char* name);

	/**
	 * Parsear header
	 * @return	true si encuentra el campo "token" en el header "Authorization"
	 *			sino false
	 */
	static int parse_header(struct mg_str *hdr, const char* var_name, char* buffer, size_t buf_size);

public:
	/**
	 * Parsear variable de un header
	 * @return	true si encuentra el campo "var_name" en el header,sino false
	 */
	static bool parse_variable_from_authorization_header(struct http_message* msg, const std::string var_name, std::string &buffer);

	/**
	 * Parsear datos específicados en "query_string"
	 * @return	Un json con información de los datos parseados de "query_string"
	 */
	static Json parse_user_search(struct http_message* msg);

	/**
	 * Parsear json de "body"
	 * @InvalidJsonException	si el "body" no tiene formato json
	 * @return					Json
	 */
	static Json parse_json_from_body(struct http_message* msg);

	/**
	 * Parsear variable de "query_string"
	 * @return	el valor de la variable específicada en "var_name", en caso de
	 *			no encontralo, devuelve una cadena vacía
	 */
	static std::string parse_variable_from_query_string(struct http_message* msg, const std::string var_name);
};

#endif /* APPSERVER_INCLUDE_PARSERS_HTTPPARSER_H_ */
