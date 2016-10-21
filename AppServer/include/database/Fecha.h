#ifndef APPSERVER_INCLUDE_DATABASE_FECHA_H_
#define APPSERVER_INCLUDE_DATABASE_FECHA_H_

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
	Fecha(const std::string &fecha);
	Fecha(uint32_t fecha);
	std::string toString();
	uint32_t toUint32_t();
 private:
	uint32_t fecha;
};

#endif /* APPSERVER_INCLUDE_DATABASE_FECHA_H_ */
