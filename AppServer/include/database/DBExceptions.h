#ifndef APPSERVER_INCLUDE_DATABASE_DBEXCEPTIONS_H_
#define APPSERVER_INCLUDE_DATABASE_DBEXCEPTIONS_H_

#include <exception>

/**
 * Para propagar errores de LevelDB
 */
class LevelDBException: public std::runtime_error {
public:
 LevelDBException(const std::string what_arg) : std::runtime_error(what_arg) {}
};

/**
 * Errores de input genericos
 */
class BadInputException: public std::runtime_error {
public:
	BadInputException(const std::string what_arg) : std::runtime_error(what_arg) {}
};

/**
 * Tamaños demasiado grande
 */
class TooBigException: public std::runtime_error {
public:
	TooBigException(const std::string what_arg) : std::runtime_error(what_arg) {}
};

/**
 * Usuario ya existente al registrarse
 */
class PreexistentUsername: public BadInputException {
public:
	PreexistentUsername(const std::string what_arg) : BadInputException(what_arg) {}
};

/**
 * Nombre de usuario no existente al hacer login
 */
class NonexistentUsername: public BadInputException {
public:
	NonexistentUsername(const std::string what_arg) : BadInputException(what_arg) {}
};

/**
 * El email provisto o no es un email o no existe como email
 */
class NonexistentEmail: public BadInputException {
public:
	NonexistentEmail(const std::string what_arg) : BadInputException(what_arg) {}
};

/**
 * Password incorrecto al hacer login
 */
class BadPassword: public BadInputException {
public:
	BadPassword(const std::string what_arg) : BadInputException(what_arg) {}
};

/**
 * Fechas que no se corresponden con el formato estipulado
 * El formato para strings es AAAA/MM/DD
 * El formato para bytes es Año[2] Mes[1] Dia[1]
 * Para tiempo infinito se usa el string "current" o el número 0xffffffff
 */
class MalformedDate: public BadInputException {
public:
	MalformedDate(const std::string what_arg) : BadInputException(what_arg) {}
};

/**
 * Fechas que no se corresponden a otras dadas
 */
class NonexistentDate: public BadInputException {
public:
	NonexistentDate(const std::string what_arg) : BadInputException(what_arg) {}
};

/**
 * Coordenadas geograficas fuera de rango
 */
class BadGeolocation: public BadInputException {
public:
	BadGeolocation(const std::string what_arg) : BadInputException(what_arg) {}
};

/**
 * UID inexistente
 */
class NonexistentUserID: public BadInputException {
public:
	NonexistentUserID(const std::string what_arg) : BadInputException(what_arg) {}
};

/**
 * No existe el skill en el shared
 */
class NonexistentSkill: public BadInputException {
public:
	NonexistentSkill(const std::string what_arg) : BadInputException(what_arg) {}
};

/**
 * No existe la job position en el shared
 */
class NonexistentPosition: public BadInputException {
public:
	NonexistentPosition(const std::string what_arg) : BadInputException(what_arg) {}
};

/**
 * No existe la categoria en el shared
 */
class NonexistentCategory: public BadInputException {
public:
	NonexistentCategory(const std::string what_arg) : BadInputException(what_arg) {}
};

/**
 * No existe el pedido de contacto sobre el cual se quiere operar
 */
class NonexistentRequest: public BadInputException {
public:
	NonexistentRequest(const std::string what_arg) : BadInputException(what_arg) {}
};

/**
 * No existe ningun chat entre dos usuarios para una operacion que requiere exista
 */
class NonexistentChat: public BadInputException {
public:
	NonexistentChat(const std::string what_arg) : BadInputException(what_arg) {}
};

#endif /* APPSERVER_INCLUDE_DATABASE_DBEXCEPTIONS_H_ */
