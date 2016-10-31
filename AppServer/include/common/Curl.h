#ifndef CURL_H_
#define CURL_H_

#include <curl/curl.h>
#include <string>

class Curl {
private:
	CURL* curl;
	std::string buffer;

	int response_code(CURLcode res_code);
	void receive_data(std::string &buffer);

public:
	Curl();

	bool init();

	std::string get_buffer();

	int get(std::string url);

	virtual ~Curl();
};

#endif /* CURL_H_ */
