/*
 * DataBaseServer.cpp
 *
 *  Created on: Sep 27, 2016
 *      Author: emanuel
 */

#include "../../include/database/DataBases.h"

DataBases::DataBases() {
	data = "Hola, soy la Base de Datos";
}

std::string DataBases::toString() {
	return data;
}

DataBases::~DataBases() {
}

