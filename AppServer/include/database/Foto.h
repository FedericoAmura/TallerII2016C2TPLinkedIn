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
	Foto(const char* datosBinarios);
	std::string toBase64String();
	const char* toUint32_t();
 private:
	char* datosBinarios;
};

#endif /* APPSERVER_INCLUDE_DATABASE_FOTO_H_ */
