#include "../../include/database/Foto.h"
#include "../../include/base64/base64.h"

Foto::Foto(const std::string& datosBase64) : data() {
	// TODO: Verificar que sea un jpg, strip metadata
	std::string byteArray = base64_decode(datosBase64);
	data.insert(data.end(), byteArray.data(), byteArray.data()+byteArray.length());
	//std::copy(byteArray.begin(), byteArray.end(), data.begin());
}

Foto::Foto(const char* datosBinarios, size_t length) : data(length) {
	std::copy(datosBinarios, datosBinarios+length, data.begin());
}

Foto::Foto(const Foto &foto) : data(foto.data) {}

std::string Foto::toBase64String() const {
	return base64_encode((unsigned char*)data.data(), data.size());
}

std::vector<char> Foto::toBytes() const {
	return data;
}

Foto Foto::resize(const Foto &foto, size_t width, size_t height, int quality) {
	//TODO: Implementar
	return Foto(*this);
}
