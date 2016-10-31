#include "../../include/common/Curl.h"

size_t writeCallback(void* ptr, size_t size, size_t nmemb, std::string* s) {
	size_t newLength = size * nmemb;
	size_t oldLength = s->size();
	try {
		s->resize(oldLength + newLength);
	} catch (std::bad_alloc &e) {
		return 0;
	}
	std::copy((char*)ptr, (char*)ptr + newLength, s->begin() + oldLength);
	return size * nmemb;
}

Curl::Curl() {
	curl = 0;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	buffer = "";
}

bool Curl::init() {
	return (curl != 0);
}

int Curl::response_code(CURLcode res_code) {
	long http_code = 0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	return http_code;
}

void Curl::receive_data(std::string &buffer) {
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
}

std::string Curl::get_buffer() {
	return buffer;
}

int Curl::get(std::string url) {
	buffer = "";
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	receive_data(buffer);
	CURLcode res_code = curl_easy_perform(curl);
	return response_code(res_code);
}

Curl::~Curl() {
	if (curl)
		curl_easy_cleanup(curl);
	curl_global_cleanup();
}