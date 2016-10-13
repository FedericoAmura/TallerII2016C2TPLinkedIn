RequestHandler
==========================================

.. cpp:class:: RequestHandler : public HTTPRequestHandler

   Clase encargada de gestionar las peticiones HTTP según método empleado (GET, POST, PUT, DELETE).

----------------
Miembros
----------------

.. cpp:member::  HTTPRequestHandler* RequestHandler::http_handler

    Dependiendo del tipo de verbo HTTP empleado para la petición, http_handler se comportará de manera diferente
    para atender dicha petición.

----------------
Métodos
----------------

.. cpp:function:: RequestHandler(http_request* req)

    Constructor. Llama al constructor de la clase base.

    :param req: Petición HTTP que será gestionada.
    :returns: Devuelve un objeto de tipo RequestHandler.

.. cpp:function:: http_response handleRequest()

    Maneja una petición HTTP según verbo empleado.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta HTTP.
