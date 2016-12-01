#ifndef CURL_H_
#define CURL_H_

#include <curl/curl.h>
#include <string>

/**
 * Wrapper de curl para realizar peticiones HTTP
 */
class Curl {
private:
	CURL* curl;
	struct curl_slist *header_list;
	std::string buffer;

	int response_code(CURLcode res_code);
	void receive_data(std::string &buffer);

public:
	Curl();

	/**
	 * Get Buffer
	 * @return		buffer con datos en caso de que se haya utilizado para realizar un petición HTTP
	 */
	std::string get_buffer();

	/**
	 * Add header
	 * @param header	Cabecera que se desea agregar a la petición HTTP
	 * @param value		valor de la cabecera que se desea agregar		
	 * @return			buffer con datos en caso de que se haya utilizado para realizar un petición HTTP
	 */
	void add_header(std::string header, std::string value);

	int get(std::string url);

	int post(std::string url, std::string data);

	virtual ~Curl();
};

#endif /* CURL_H_ */
