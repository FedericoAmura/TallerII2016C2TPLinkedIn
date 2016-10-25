#ifndef APPSERVER_INCLUDE_DATABASE_FOTO_H_
#define APPSERVER_INCLUDE_DATABASE_FOTO_H_

#include <string>
#include <vector>

/**
 * Clase que representa la información de una foto
 * Hay un máximo arbitrario de 4MB
 */
class Foto
{
 public:
	/**
	 * Crea una foto a partir de de un string encodeado en base64
	 * Remueve la metadata de la foto, si es que existe
	 * @param datosBase64				Datos
	 * @exception TooBigException		Tamaño mayor a 4MB
	 * @exception BadInputException		La foto no es un jpg
	 */
	Foto(const std::string &datosBase64);
	/**
	 * Crea una foto a partir de una tira de bytes
	 * @param datosBinarios				Byte array
	 * @param length					Tamaño
	 */
	Foto(const char* datosBinarios, size_t length);
	/**
	 * Constructor por copia
	 * @param foto						Foto fuente
	 */
	Foto(const Foto &foto);
	/**
	 * Devuelve la informacion de la foto como un string encodeado en base64
	 * @return
	 */
	std::string toBase64String() const;
	/**
	 * Devuelve la tira de bytes que representa la imagen
	 * @return
	 */
	std::vector<char> toBytes() const;
	/**
	 * Devuelve una foto con el nuevo tamaño y calidad pedido
	 * @param foto					Foto origen
	 * @param width					Ancho deseado
	 * @param height				Alto deseado
	 * @param quality				Calidad
	 * @return
	 */
	Foto resize(const Foto &foto, size_t width, size_t height, int quality);
 private:
	std::vector<char> data;
};

#endif /* APPSERVER_INCLUDE_DATABASE_FOTO_H_ */
