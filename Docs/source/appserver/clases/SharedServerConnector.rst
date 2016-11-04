SharedServerConnector
==========================================

.. cpp:class:: SharedServerConnector

   Clase estática utilizada para gestionar peticiones desde el AppServer al SharedServer.

----------------
Métodos
----------------

.. cpp:function:: static Json get_categories()

    Obtener categorías.

    :throws CurlInitException: Error al inicial curl.
    :throws CurlGetException: Error al realizar petición GET http.
    :throws InvalidJsonException: Datos recibidos en formato no json.
    :returns: Devuelve un json con las categorías existentes.


.. cpp:function:: static Json get_job_positions()

    Obtener posiciones de trabajo.

    :throws CurlInitException: Error al inicial curl.
    :throws CurlGetException: Error al realizar petición GET http.
    :throws InvalidJsonException: Datos recibidos en formato no json.
    :returns: Devuelve un json con las posiciones de trabajo existentes.

.. cpp:function:: static Json get_skills()

    Obtener skills.

    :throws CurlInitException: Error al inicial curl.
    :throws CurlGetException: Error al realizar petición GET http.
    :throws InvalidJsonException: Datos recibidos en formato no json.
    :returns: Devuelve un json con las skills existentes.
