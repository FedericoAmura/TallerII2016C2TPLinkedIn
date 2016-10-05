PUT_Handler
==========================================

.. cpp:class:: PUT_Handler : public HTTPRequestHandler

   Clase encargada de gestionar las peticiones HTTP de tipo PUT.

----------------
Métodos
----------------

.. cpp:function:: PUT_Handler(http_request* req)

    Constructor. Llama al constructor de la clase base.

    :param req: Petición HTTP que se quiere procesar.
    :returns: Devuelve un objeto de tipo PUT_Handler.

.. cpp:function:: http_response handleRequest()

    Maneja una petición HTTP de tipo PUT.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.
              En caso de éxito, retorna una respuesta vacía y un código de respuesta HTTP 200.

.. cpp:function:: http_response handleProfile()

    Actualización de perfil

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.
              En caso de éxito, retorna una respuesta vacía y un código de respuesta HTTP 200.

.. cpp:function:: http_response handleSummary()

    Actualización de resumen de usuario

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.
              En caso de éxito, retorna una respuesta vacía y un código de respuesta HTTP 200.

.. cpp:function:: http_response handlePhoto()

    Maneja una petición HTTP de tipo PUT.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.

.. cpp:function:: http_response handleRecommend()

    Maneja una petición HTTP de tipo PUT.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.
