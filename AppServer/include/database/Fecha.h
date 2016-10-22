#ifndef APPSERVER_INCLUDE_DATABASE_FECHA_H_
#define APPSERVER_INCLUDE_DATABASE_FECHA_H_

#include <string>

/**
 * Clase para representar fechas de una forma que permita una facil transicion entre strings
 * Y el tipo compacto en 4 bytes
 * Las fechas como string seran en formato AAAA/MM/DD
 * Para fecha actual/infinito se tomara el string "current"
 * Las fechas uint32_t son en el siguiente formato:
 * 32 bits = 16 bits año, 8 bits mes, 8 bits día
 * En binario: AAAAAAAA AAAAAAAA MMMMMMMM DDDDDDDD
 * Para fecha actual/infinito se tomara la fecha 0xffffffff
 */
class Fecha
{
 public:
	/**
	 * Constructor a partir de string AAAA/MM/DD
	 * @param fecha						El string de caracteres textuales
	 * @exception MalformedDate			Fecha malformada
	 */
	Fecha(const std::string &fecha);
	/**
	 * Constructor a partir de un byte array (toma 4 bytes a partir del marcado)
	 * @param byteArray
	 * @exception MalformedDate			Fecha malformada
	 */
	Fecha(const char* byteArray);
	/**
	 * Constructor por copia
	 * @param fecha
	 */
	Fecha(const Fecha &fecha);
	/**
	 * Constructor por defecto con miembros no inicializados
	 */
	Fecha();
	/**
	 * Setear data a partir de un byte array (toma 4 bytes a partir del marcado)
	 * @param byteArray
	 * @exception MalformedDate			Fecha malformada
	 */
	void assign(const char *byteArray);
	/**
	 * Devuelve un string de formato AAAA/MM/DD
	 * @return
	 */
	std::string toString();
	/**
	 * Devuelve un valor de 4 bytes que representa una fecha
	 * @return
	 */
	uint32_t toUint32_t();
 private:
	uint32_t fecha;
};

#endif /* APPSERVER_INCLUDE_DATABASE_FECHA_H_ */
