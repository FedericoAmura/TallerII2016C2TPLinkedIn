#ifndef APPSERVER_INCLUDE_HANDLERS_SHAREDSERVERCONNECTOR_H_
#define APPSERVER_INCLUDE_HANDLERS_SHAREDSERVERCONNECTOR_H_

#include "../json11/json11.hpp"
#include "../common/Curl.h"
#include "../common/Exceptions.h"

using json11::Json;

static const std::string SHARED_SERVER_URL	            = "http://jobifyg2.herokuapp.com"; //"http://127.0.0.1:5000"
static const std::string SHARED_SERVER_CATEGORIES_URL   = SHARED_SERVER_URL + "/categories";
static const std::string SHARED_SERVER_JOB_POSITIONS_URL= SHARED_SERVER_URL + "/job_positions";
static const std::string SHARED_SERVER_SKILLS_URL       = SHARED_SERVER_URL + "/skills";


class SharedServerConnector {
 private:
    static Json get(std::string url);

 public:
     /**
      * Obtener categorías
      * @exception CurlInitException     Error al iniciar cURL
      * @exception CurlGetException      Error al hacer un pedido GET
      * @exception InvalidJsonException  Datos recibidos en formato inválido
      * @return                          Ver documentacion API
      */
    static Json get_categories();

    /**
     * Obtener posiciones de trabajo
     * @exception CurlInitException     Error al iniciar cURL
     * @exception CurlGetException      Error al hacer un pedido GET
     * @exception InvalidJsonException  Datos recibidos en formato inválido
     * @return                          Ver documentacion API
     */
    static Json get_job_positions();

    /**
     * Obtener skills
     * @exception CurlInitException     Error al iniciar cURL
     * @exception CurlGetException      Error al hacer un pedido GET
     * @exception InvalidJsonException  Datos recibidos en formato inválido
     * @return                          Ver documentacion API
     */
    static Json get_skills();
};

#endif  // APPSERVER_INCLUDE_HANDLERS_SHAREDSERVERCONNECTOR_H_
