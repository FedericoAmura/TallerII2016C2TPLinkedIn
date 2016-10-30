#include "../../include/database/Foto.h"
#include "../../include/base64/base64.h"

Foto::Foto(const std::string& datosBase64) : data() {
	// TODO: Verificar que sea un jpg, strip metadata
	std::string byteArray = base64_decode(datosBase64);
	data.insert(data.end(), byteArray.data(), byteArray.data()+byteArray.length());
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

Foto Foto::resize(int quality, size_t width, size_t height) const {
	//TODO: Implementar
	return Foto(*this);
}

leveldb::Slice Foto::toSlice() {
	return leveldb::Slice(data.data(), data.size());
}
