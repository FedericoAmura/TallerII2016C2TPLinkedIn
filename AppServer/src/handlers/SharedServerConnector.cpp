#include "../../include/handlers/SharedServerConnector.h"

Json SharedServerConnector::get(std::string url) {
    Curl curl;

    int res_code = curl.get(url);
    if (res_code != 200)
        throw CurlGetException();

    std::string buffer = curl.get_buffer(), err;
    Json data = Json::parse(buffer.c_str(), err);
    if (!err.empty())
        throw InvalidJsonException();
    return data;
}

Json SharedServerConnector::get_categories() {
    return SharedServerConnector::get(SHARED_SERVER_CATEGORIES_URL);
}

Json SharedServerConnector::get_job_positions() {
    return SharedServerConnector::get(SHARED_SERVER_JOB_POSITIONS_URL);
}

Json SharedServerConnector::get_skills() {
    return SharedServerConnector::get(SHARED_SERVER_SKILLS_URL);
}
