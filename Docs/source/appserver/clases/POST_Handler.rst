POST_Handler
==========================================

.. cpp:class:: POST_Handler : public HTTPRequestHandler

   Clase encargada de gestionar las peticiones HTTP de tipo POST.

----------------
Métodos
----------------

.. cpp:function:: POST_Handler(http_request* req)

    Constructor. Llama al constructor de la clase base.

    :param req: Petición HTTP que se quiere procesar.
    :returns: Devuelve un objeto de tipo POST_Handler.

.. cpp:function:: http_response handleRequest()

    Maneja una petición HTTP de tipo POST.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.

.. cpp:function:: http_response handleLogIn()

    Maneja una petición HTTP de tipo POST.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.

.. cpp:function:: http_response handleSignUp()

    Maneja una petición HTTP de tipo POST.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.

.. cpp:function:: http_response handleAcceptReqContact()

    Maneja una petición HTTP de tipo POST.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.

.. cpp:function:: http_response handleReqContact()

    Maneja una petición HTTP de tipo POST.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.

.. cpp:function:: http_response handleChatNotifMsgSeen()

    Maneja una petición HTTP de tipo POST.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.

.. cpp:function:: http_response handleChatSendMsg()

    Maneja una petición HTTP de tipo POST.

    :returns: Devuelve un http_response que contiene una respuesta y un código de respuesta.
