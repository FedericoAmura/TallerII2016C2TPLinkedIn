#ifndef CURL_H_
#define CURL_H_

#include <curl/curl.h>
#include <string>

class Curl {
private:
	CURL* curl;
	struct curl_slist *header_list;
	std::string buffer;

	int response_code(CURLcode res_code);
	void receive_data(std::string &buffer);

public:
	Curl();

	std::string get_buffer();

	void add_header(std::string header, std::string value);

	int get(std::string url);

	int post(std::string url, std::string data);

	virtual ~Curl();
};

#endif /* CURL_H_ */
