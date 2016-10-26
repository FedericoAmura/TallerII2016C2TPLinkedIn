#include "../../include/database/Puesto.h"

Puesto::Puesto(const std::string& puesto, Fecha fechaInicio, Fecha fechaFin)
	: puesto(puesto), fechaInicio(fechaInicio), fechaFin(fechaFin) {
	if (this->fechaFin < this->fechaInicio) throw TimeTravel("Error al crear puesto.");
}

Puesto::Puesto(const char* datosBinarios) {
	uint8_t len = datosBinarios[0];
	puesto = std::string(datosBinarios+1, len);
	fechaInicio.assign(datosBinarios+1+len);
	fechaFin.assign(datosBinarios+5+len);
}

size_t Puesto::length() const
{
	return puesto.length()+sizeof(fechaInicio)+sizeof(fechaFin);
}

std::vector<char> Puesto::toBytes() const {
	std::vector<char> result(this->length()+1);
	result[0] = puesto.length();
	std::vector<char>::iterator it = result.begin();
	*it = (uint8_t) puesto.length();
	++it;
	it = std::copy(puesto.begin(), puesto.end(), it);
	uint32_t fechaIBytes = fechaInicio.toUint32_t();
	it = std::copy((char*)&fechaIBytes, (char*)(&fechaIBytes+1), it);
	uint32_t fechaFBytes = fechaFin.toUint32_t();
	it = std::copy((char*)&fechaFBytes, (char*)(&fechaFBytes+1), it);
	return result;
}
