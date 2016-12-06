/*
 * GCM_Connector.h
 *
 *  Created on: Nov 6, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_GCM_CONNECTOR_H_
#define APPSERVER_INCLUDE_HANDLERS_GCM_CONNECTOR_H_

#include "../server/API_Server.h"
#include "../common/Curl.h"
#include "../common/Exceptions.h"

class GCM_Connector {
public:
    /**
     * Push Notification
     * Se envía una notificación a GMC
     * @RequestRejected     si la respuesta http de FCM es distinta a 200
     * @return	            1 si el envío de información es correcto, sino 0 
     */
    static int notify(std::string data);
};

#endif /* APPSERVER_INCLUDE_HANDLERS_GCM_CONNECTOR_H_ */
