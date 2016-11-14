/*
 * GCM_Connector.cpp
 *
 *  Created on: Nov 6, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/GCM_Connector.h"

int GCM_Connector::notify(std::string data) {
    Curl curl;

    curl.add_header("Authorization", "key=" + GOOGLE_API_KEY_APPSERVER);
    curl.add_header("Content-Type", "application/json");
    int status_code = curl.post(GOOGLE_CLOUD_MESSAGING_URL, data);
    if (status_code != 200)
        throw RequestRejected();
    return status_code;
}
