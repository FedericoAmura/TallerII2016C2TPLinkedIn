/*
 * User.h
 *
 *  Created on: Oct 6, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_COMMON_USER_H_
#define APPSERVER_INCLUDE_COMMON_USER_H_

#include <string>
#include "../database/Geolocacion.h"

/**
 * Estructura para guardar propiedades de usuarios para realizar búsquedas.
 * */
struct user_properties {
	std::string category;
	std::string skill;
	std::string job_position;
	Geolocacion geolocation;
	float distance;
	bool popsort;

	user_properties() {
		category = "";
		skill = "";
		job_position = "";
		distance = 0.0;
		popsort = false;
	}
};
#endif /* APPSERVER_INCLUDE_COMMON_USER_H_ */
