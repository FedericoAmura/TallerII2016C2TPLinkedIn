#ifndef APPSERVER_INCLUDE_DATABASE_FOTO_H_
#define APPSERVER_INCLUDE_DATABASE_FOTO_H_

/**
 * Clase que representa la información de una foto
 * TODO: Metadata stripping
 */
class Foto
{
 public:
	Foto(const std::string &datosBase64);
	Foto(const uint8_t* datosBinarios);
	std::string toBase64String();
	const uint32_t* toUint32_t();
 private:
	uint8_t* datosBinarios;
};

#endif /* APPSERVER_INCLUDE_DATABASE_FOTO_H_ */
