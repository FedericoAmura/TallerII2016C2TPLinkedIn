DELETE_Handler
==========================================

.. cpp:class:: DELETE_Handler : public HTTPRequestHandler

   Clase encargada de gestionar las peticiones HTTP de tipo DELETE.

----------------
Métodos
----------------

.. cpp:function:: DELETE_Handler(http_request* req)

    Constructor. Llama al constructor de la clase base.

    :param req: Petición HTTP que se quiere procesar.
    :returns: Devuelve un objeto de tipo DELETE_Handler.

.. cpp:function:: http_response handleRequest()

    Maneja una petición HTTP de tipo DELETE.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.
              En caso de éxito, retorna una respuesta vacía y un código de respuesta HTTP 204.

.. cpp:function:: http_response handleCloseSession()

    Cierra la conexión establecida.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.
              En caso de éxito, retorna una respuesta vacía y un código de respuesta HTTP 204.

.. cpp:function:: http_response handleRejectContactRequest()

    Maneja rechazos de solicitud de contacto.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.
              En caso de éxito, retorna una respuesta vacía y un código de respuesta HTTP 204.

.. cpp:function:: http_response handleDeleteContact()

    Elimina un contacto.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.
              En caso de éxito, retorna una respuesta vacía y un código de respuesta HTTP 204.
